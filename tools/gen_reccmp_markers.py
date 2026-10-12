#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import re
import sys
from pathlib import Path, PureWindowsPath

try:
    from reccmp.cvdump import Cvdump, CvdumpAnalysis, CvdumpParser
except ImportError:  # pragma: no cover - dependency is provided by reccmp
    print("reccmp is required: pip install reccmp", file=sys.stderr)
    raise

# Combined into a PDB/CSV pair for ``--all``.
# reccmp target id -> (annotation module, built binary, built PDB, original image base).
# Paths are relative to the VC6 build directory. The image base comes from the
# original 1.10f binary and is used to discard malformed addresses in annotations.
TARGETS: dict[str, tuple[str, str, str, int]] = {
    "D2COMMON": ("D2Common", "source/D2Common/D2Common.dll", "source/D2Common/D2Common.pdb", 0x6FD40000),
    "D2GAME": ("D2Game", "source/D2Game/D2Game.dll", "source/D2Game/D2Game.pdb", 0x6FC30000),
    "D2LANG": ("D2Lang", "source/D2Lang/D2Lang.dll", "source/D2Lang/D2Lang.pdb", 0x6FC10000),
    "D2WIN": ("D2Win", "source/D2Win/D2Win.dll", "source/D2Win/D2Win.pdb", 0x6F8A0000),
    "D2GFX": ("D2Gfx", "source/D2Gfx/D2Gfx.dll", "source/D2Gfx/D2Gfx.pdb", 0x6FA70000),
    "D2NET": ("D2Net", "source/D2Net/D2Net.dll", "source/D2Net/D2Net.pdb", 0x6FC00000),
    "D2CMP": ("D2CMP", "source/D2CMP/D2CMP.dll", "source/D2CMP/D2CMP.pdb", 0x6FDF0000),
    "GAME": ("Game", "source/Game/Game.D2MOO.exe", "source/Game/Game.D2MOO.pdb", 0x00400000),
}

# A generated original address must fall inside the module's image. The DLLs are
# well under 16 MiB, which is generous and still rejects truncated addresses.
_MAX_IMAGE_SIZE = 0x1000000

# Comment lines that can sit between an annotation and the entity it describes.
_PREPROCESSOR_RE = re.compile(r"^\s*#")
_COMMENT_RE = re.compile(r"^\s*//")
_BLOCK_COMMENT_RE = re.compile(r"^\s*(/\*|\*)")

# Matches an annotation for a single module using a 1.10f address. Versions other
# than 1.10/1.10f (e.g. 1.11, 1.13c, 1.14d) use different address spaces and must
# be ignored. Text may follow the address, e.g. an ordinal or a decorated name.
_ANNOTATION_TEMPLATE = (
    r"^\s*//\s*(?:(?:1\.10f?)\s*:\s*)?"
    r"(?<![0-9A-Za-z_])(?P<module>{module})\s*\.\s*(?P<address>0x[0-9A-Fa-f]+)"
)

# How far above the entity definition to look for a signature.
_LOOKBACK_LINES = 40


def _module_pattern(module: str) -> re.Pattern[str]:
    return re.compile(_ANNOTATION_TEMPLATE.format(module=re.escape(module)))


# Map a PDB source path (e.g. Z:\\hdd\\D2MOO2\\...) to a local file."
def _local_path(foreign: PureWindowsPath, source_root: Path) -> Path | None:
    parts = list(foreign.parts)
    # Find the longest suffix of the PDB path that exists under the source root.
    for index in range(len(parts)):
        candidate = source_root.joinpath(*parts[index:])
        if candidate.is_file():
            return candidate
    return None


# Find the index of the definition line for the function.
def _signature_index(lines: list[str], body_index: int, name: str) -> int | None:
    # The PDB records the line of the opening brace (the first line with code), so
    # the signature is found by searching upward for the function name.
    base = name.split("::")[-1].lstrip("~")
    if not base or base.startswith("$") or base.startswith("?"):
        return None
    needle = re.compile(r"\b" + re.escape(base) + r"\s*(?:<[^>]*>\s*)?\(")
    # First look around and above
    for index in range(body_index, min(len(lines), body_index + 5)):
        if needle.search(lines[index]):
            return index
    for index in range(body_index, max(-1, body_index - _LOOKBACK_LINES), -1):
        if needle.search(lines[index]):
            return index
    return None


# Walk upward from the signature looking for an annotation matching pattern.
def _find_annotation(lines: list[str], signature_index: int, pattern: re.Pattern[str]) -> int | None:
    index = signature_index - 1
    while index >= 0:
        text = lines[index]
        match = pattern.match(text)
        if match is not None:
            return int(match.group("address"), 16)
        stripped = text.strip()
        if (
            stripped == ""
            or _COMMENT_RE.match(text)
            or _BLOCK_COMMENT_RE.match(text)
            or _PREPROCESSOR_RE.match(text)
        ):
            index -= 1
            continue
        # Reached another code line: the entity is not annotated.
        break
    return None


# Return (address, type, name) rows for one PDB/module pair.
# When base is given, annotations whose address falls outside the module image
# are discarded.
def generate(pdb: Path, module: str, source_root: Path, entity_type: str, base: int | None = None) -> list[tuple[int, str, str]]:
    cvdump = Cvdump(pdb).lines().globals().publics().symbols()
    parser = cvdump.run()
    analysis = CvdumpAnalysis(parser)

    # (section, offset) -> (source file, line number) for every recorded line.
    address_lines: dict[tuple[int, int], tuple[PureWindowsPath, int]] = {}
    for filename, values in analysis.lines.items():
        for value in values:
            address_lines.setdefault(
                (value.section, value.offset), (filename, value.line_number)
            )

    pattern = _module_pattern(module)
    lines_cache: dict[Path, list[str]] = {}
    seen_addresses: dict[int, str] = {}
    rows: list[tuple[int, str, str]] = []

    wanted = "FUNCTION" if entity_type == "function" else "DATA"
    for node in analysis.nodes:
        if node.node_type is None or node.node_type.name != wanted:
            continue
        if node.section is None or node.offset is None:
            continue
        location = address_lines.get((node.section, node.offset))
        if location is None:
            continue
        foreign, line_number = location
        local = _local_path(foreign, source_root)
        if local is None:
            continue
        if local not in lines_cache:
            try:
                lines_cache[local] = local.read_text(
                    encoding="utf-8", errors="surrogateescape"
                ).splitlines()
            except OSError:
                lines_cache[local] = []
        name = node.name()
        if not name:
            continue
        entity_index = line_number - 1
        if wanted == "FUNCTION":
            signature = _signature_index(lines_cache[local], entity_index, name)
            if signature is None:
                continue
            entity_index = signature
        annotation = _find_annotation(lines_cache[local], entity_index, pattern)
        if annotation is None:
            # Also look in header files under source root
            for header in source_root.rglob("*.h"):
                try:
                    hlines = header.read_text(encoding="utf-8", errors="surrogateescape").splitlines()
                except Exception:
                    continue
                hannot = _find_annotation(hlines, len(hlines) - 1, pattern)
                if hannot is not None:
                    # Check if this annotation is for the right function name?
                    # Just use it if found
                    annotation = hannot
                    break
            if annotation is None:
                continue
        if base is not None and not (base <= annotation < base + _MAX_IMAGE_SIZE):
            continue
        if annotation in seen_addresses:
            # Keep the first name for an address to avoid ambiguity.
            continue
        seen_addresses[annotation] = name
        rows.append((annotation, entity_type, name))

    return rows


def write_csv(rows: list[tuple[int, str, str]], output: Path) -> None:
    rows = sorted(rows, key=lambda row: row[0])
    output.parent.mkdir(parents=True, exist_ok=True)
    with output.open("w", newline="") as handle:
        writer = csv.writer(handle)
        writer.writerow(["address", "type", "name"])
        for address, entity_type, name in rows:
            writer.writerow([f"0x{address:08X}", entity_type, name])


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--all", action="store_true", help="Generate every known target")
    parser.add_argument("--pdb", type=Path, help="Path to the recompiled PDB")
    parser.add_argument("--module", help="Annotation module, e.g. D2Common")
    parser.add_argument("--source-root", type=Path, default=Path("."))
    parser.add_argument("--output", type=Path, help="Output CSV path")
    parser.add_argument(
        "--type",
        default="function",
        choices=("function", "global", "both"),
        help="Which entity kinds to emit",
    )
    parser.add_argument(
        "--build-dir",
        type=Path,
        default=Path("out/build/VC6"),
        help="VC6 build directory (used with --all)",
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=Path("reccmp-data"),
        help="Where to write the CSVs when using --all",
    )
    parser.add_argument(
        "--base",
        type=lambda value: int(value, 0),
        default=None,
        help="Original image base; annotations outside [base, base+16MiB) are dropped",
    )
    args = parser.parse_args()

    source_root = args.source_root.resolve()

    if args.all:
        for target_id, (module, _binary, pdb_rel, base) in TARGETS.items():
            pdb = args.build_dir / pdb_rel
            if not pdb.is_file():
                print(f"skip {target_id}: {pdb} does not exist", file=sys.stderr)
                continue
            rows = generate(pdb, module, source_root, "function", base)
            output = args.output_dir / f"{target_id}.csv"
            write_csv(rows, output)
            print(f"{target_id}: {len(rows)} functions -> {output}")
        return 0

    if not args.pdb or not args.module or not args.output:
        parser.error("--pdb, --module and --output are required without --all")

    rows: list[tuple[int, str, str]] = []
    kinds = ("function", "global") if args.type == "both" else (args.type,)
    for entity_type in kinds:
        rows.extend(
            generate(args.pdb, args.module, source_root, entity_type, args.base)
        )
    write_csv(rows, args.output)
    print(f"Wrote {len(rows)} entities to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())