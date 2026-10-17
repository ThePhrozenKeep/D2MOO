#!/usr/bin/env python3

# Usage:
#     tools/run_reccmp.py                    # compare every target that was built
#     tools/run_reccmp.py D2COMMON D2GAME    # compare specific targets
#     tools/run_reccmp.py D2COMMON --diff    # pass extra options to asmcmp

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from pathlib import Path

# Importing the generator requires reccmp to be importable, which is also
# required to run the comparison below.
from gen_reccmp_markers import TARGETS, generate, write_csv

REPO_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_BUILD_DIR = REPO_ROOT / "out" / "build" / "VC6"
DATA_DIR = REPO_ROOT / "reccmp-data"


# Return the (binary, pdb) paths for a target if both were built.
def _built_binary(build_dir: Path, target_id: str) -> tuple[Path, Path] | None:
    _module, binary_rel, pdb_rel, _base = TARGETS[target_id]
    binary = (build_dir / binary_rel).resolve()
    pdb = (build_dir / pdb_rel).resolve()
    if not binary.is_file() or not pdb.is_file():
        return None
    return binary, pdb


# Write reccmp-build.yml with local paths. Returns the valid target ids.
def write_build_config(build_dir: Path, target_ids: list[str]) -> list[str]:
    lines = [f"project: '{REPO_ROOT}'", "targets:"]
    valid: list[str] = []
    for target_id in target_ids:
        outputs = _built_binary(build_dir, target_id)
        if outputs is None:
            print(f"skip {target_id}: build output missing", file=sys.stderr)
            continue
        binary, pdb = outputs
        lines.append(f"  {target_id}:")
        lines.append(f"    path: '{binary}'")
        lines.append(f"    pdb: '{pdb}'")
        valid.append(target_id)
    (REPO_ROOT / "reccmp-build.yml").write_text("\n".join(lines) + "\n")
    return valid


def generate_markers(build_dir: Path, target_ids: list[str]) -> None:
    DATA_DIR.mkdir(parents=True, exist_ok=True)
    for target_id in target_ids:
        module, _binary, pdb_rel, base = TARGETS[target_id]
        pdb = build_dir / pdb_rel
        if not pdb.is_file():
            continue
        rows = generate(pdb, module, REPO_ROOT, "function", base)
        write_csv(rows, DATA_DIR / f"{target_id}.csv")
        print(f"{target_id}: {len(rows)} annotated functions")



def resolve_symbol(build_dir: Path, target_id: str, symbol: str) -> str | None:
    try:
        from gen_reccmp_markers import TARGETS, generate
        module, _binary, pdb_rel, base = TARGETS[target_id]
        pdb = build_dir / pdb_rel
        if not pdb.is_file():
            return None
        rows = generate(pdb, module, REPO_ROOT, "function", base)
        # rows are (addr, type, name)
        for addr, typ, name in rows:
            if name == symbol or name.endswith(symbol) or symbol in name:
                return hex(addr)
        return None
    except Exception:
        return None


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "targets", nargs="*", help="reccmp target ids (default: every built target)"
    )
    parser.add_argument(
        "--all", action="store_true", help="compare every target that was built"
    )
    parser.add_argument(
        "--build-dir", type=Path, default=DEFAULT_BUILD_DIR, help="VC6 build directory"
    )
    parser.add_argument(
        "--no-markers", action="store_true", help="do not regenerate marker CSVs"
    )
    parser.add_argument(
        "--no-config",
        action="store_true",
        help="do not rewrite reccmp-build.yml (use an existing one)",
    )
    parser.add_argument(
        "--diff",
        metavar="<address>",
        help="Show verbose diff for a specific function by address (hex)",
    )
    parser.add_argument(
        "--json-dir",
        type=Path,
        metavar="<dir>",
        help=(
            "Write one compact (--json-diet) match summary per target, "
            "e.g. <dir>/D2COMMON.json, for regression comparison"
        ),
    )
    parser.add_argument(
        "--html-dir",
        type=Path,
        metavar="<dir>",
        help=(
            "Write a self-contained HTML report per target, "
            "e.g. <dir>/D2COMMON.html"
        ),
    )
    parser.add_argument(
        "--copy-builds",
        type=Path,
        metavar="<dir>",
        help=(
            "Copy each compared binary (our build) into <dir>, so it can be "
            "published alongside the accuracy report"
        ),
    )
    # Everything else is forwarded to asmcmp.
    args, extra = parser.parse_known_args()

    build_dir = args.build_dir.resolve()
    if args.targets:
        requested = [t.upper() for t in args.targets]
        unknown = [t for t in requested if t not in TARGETS]
        if unknown:
            parser.error(f"unknown target(s): {', '.join(unknown)}")
    else:
        requested = list(TARGETS)

    if not (REPO_ROOT / "reccmp-project.yml").is_file():
        print("reccmp-project.yml is missing from the repository root", file=sys.stderr)
        return 1
    if not (REPO_ROOT / "reccmp-user.yml").is_file():
        print(
            "reccmp-user.yml is missing. It records the local paths to your "
            "original 1.10(f) binaries and is intentionally not tracked.\n"
            "Create it with:\n"
            "  reccmp-project create --originals <your D2Common.dll ...> "
            f"--path {REPO_ROOT}\n"
            "or copy reccmp-user.yml from a checkout that already has it.",
            file=sys.stderr,
        )
        return 1

    if args.no_config:
        valid = [t for t in requested if _built_binary(build_dir, t) is not None]
    else:
        valid = write_build_config(build_dir, requested)

    if args.copy_builds is not None:
        args.copy_builds.mkdir(parents=True, exist_ok=True)
        copied = 0
        for target_id in valid:
            outputs = _built_binary(build_dir, target_id)
            if outputs is None:
                continue
            binary, _pdb = outputs
            shutil.copy2(binary, args.copy_builds / binary.name)
            copied += 1
        print(f"copied {copied} compared binaries to {args.copy_builds}", file=sys.stderr)

    if not args.no_markers:
        generate_markers(build_dir, valid)

    if not valid:
        print("no targets to compare", file=sys.stderr)
        return 1

    failed = 0
    if args.json_dir is not None:
        args.json_dir.mkdir(parents=True, exist_ok=True)
    if args.html_dir is not None:
        args.html_dir.mkdir(parents=True, exist_ok=True)
    for target_id in valid:
        cmd = [
            sys.executable,
            "-m",
            "reccmp.tools.asmcmp",
            "--target",
            target_id,
        ]
        if args.json_dir is not None:
            cmd.extend(
                ["--json", str(args.json_dir / f"{target_id}.json"), "--json-diet"]
            )
        if args.html_dir is not None:
            cmd.extend(["--html", str(args.html_dir / f"{target_id}.html")])
        if args.diff:
            diff_arg = args.diff
            # try to resolve if not hex
            try:
                # check if hex
                int(diff_arg, 16)
            except ValueError:
                # try to resolve as symbol for each target
                resolved = None
                for tid in valid:
                    resolved = resolve_symbol(build_dir, tid, diff_arg)
                    if resolved:
                        break
                if resolved:
                    diff_arg = resolved
                else:
                    print(f"Error: Could not resolve symbol '{diff_arg}'", flush=True)
                    failed += 1
                    continue
            cmd.extend(["--verbose", diff_arg])
        cmd.extend(extra)
        print(f"\n=== {target_id} ===", flush=True)
        result = subprocess.run(cmd, cwd=REPO_ROOT)
        failed += result.returncode != 0
    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())