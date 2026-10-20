#!/usr/bin/env python3
# Compare reccmp match summaries (the compact JSON written by
# `tools/run_reccmp.py --json-dir`) and fail if any function's accuracy has
# regressed relative to a saved baseline.
# Usage:
#     tools/reccmp_regression.py --baseline reccmp-baseline --current reccmp-json
#
# Pass --commit-message to honor the "[reccmp skip]" marker.
#
# Exit codes:
#     0  no regressions (or no baseline to compare against)
#     1  at least one function regressed
#     2  bad usage / could not read a report
#
# The baseline directory is expected to contain one `<TARGET>.json` per
# reccmp target, as produced by a previous run on the base branch.

from __future__ import annotations

import argparse
import sys
from pathlib import Path

from reccmp.compare.report import deserialize_reccmp_report
from reccmp.utils import ReccmpDiffJudgement, entity_diff_change


SKIP_MARKER = "[reccmp skip]"


def has_skip_marker(message: str | None) -> bool:
    """True if ``message`` carries the skip marker (case-insensitive)."""
    if not message:
        return False
    return SKIP_MARKER in message.lower()


def _pct(accuracy: float, is_stub: bool, is_effective: bool) -> str:
    if is_stub:
        return "stub"
    if is_effective:
        return "effective"
    return f"{accuracy * 100:.2f}%"


def compare_reports(baseline_path: Path, current_path: Path, tolerance: float):
    """Return (regressions, summary) for one target.

    A regression is a function that the baseline matched better than the
    current build. ``entity_diff_change`` is reccmp's own classifier, so we
    inherit its handling of stubs and effective (compiler-entropy) matches.
    """
    baseline = deserialize_reccmp_report(
        baseline_path.read_text(encoding="utf-8")
    )
    current = deserialize_reccmp_report(current_path.read_text(encoding="utf-8"))

    if not baseline.has_same_source(current):
        return None, (
            f"baseline {baseline_path.name} is for '{baseline.filename}' "
            f"but current is '{current.filename}' - skipping"
        )

    regressions = []
    counts: dict[ReccmpDiffJudgement, int] = {}
    for addr in sorted(set(baseline.entities) | set(current.entities)):
        old = baseline.entities.get(addr)
        new = current.entities.get(addr)
        judgement = entity_diff_change(old, new)
        counts[judgement] = counts.get(judgement, 0) + 1

        if judgement is not ReccmpDiffJudgement.DECREASE:
            continue

        old_acc = old.accuracy if old is not None else 0.0
        new_acc = new.accuracy if new is not None else 0.0
        if old_acc - new_acc <= tolerance:
            # Within tolerance: treat as noise, not a regression.
            counts[judgement] -= 1
            continue

        regressions.append(
            {
                "address": hex(addr),
                "name": new.name if new is not None else (old.name if old else "?"),
                "old": _pct(
                    old_acc,
                    bool(old and old.is_stub),
                    bool(old and old.is_effective_match),
                ),
                "new": _pct(
                    new_acc,
                    bool(new and new.is_stub),
                    bool(new and new.is_effective_match),
                ),
                "old_raw": old_acc,
                "new_raw": new_acc,
            }
        )

    return regressions, counts


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--baseline",
        type=Path,
        default=Path("reccmp-baseline"),
        help="directory of baseline <TARGET>.json reports (default: reccmp-baseline)",
    )
    parser.add_argument(
        "--current",
        type=Path,
        default=Path("reccmp-json"),
        help="directory of current <TARGET>.json reports (default: reccmp-json)",
    )
    parser.add_argument(
        "--targets",
        nargs="*",
        help="targets to compare (default: every report in --current)",
    )
    parser.add_argument(
        "--tolerance",
        type=float,
        default=0.0,
        help="accuracy drop smaller than this is ignored (default: 0.0)",
    )
    parser.add_argument(
        "--commit-message",
        default=None,
        help=(
            f"commit message to scan for the '{SKIP_MARKER}' marker, which "
            "skips the comparison entirely (exit 0)"
        ),
    )
    args = parser.parse_args()

    if has_skip_marker(args.commit_message):
        print(
            f"'{SKIP_MARKER}' found in the commit message - "
            "skipping the accuracy regression check."
        )
        return 0

    if not args.current.is_dir():
        print(
            f"error: current report directory '{args.current}' does not exist",
            file=sys.stderr,
        )
        return 2

    current_files = sorted(args.current.glob("*.json"))
    if args.targets:
        wanted = {t.upper() for t in args.targets}
        current_files = [f for f in current_files if f.stem.upper() in wanted]
    if not current_files:
        print(f"error: no reports found in '{args.current}'", file=sys.stderr)
        return 2

    if not args.baseline.is_dir():
        print(
            f"no baseline at '{args.baseline}' - skipping regression check "
            "(this is expected on the first run for a branch)."
        )
        return 0

    total_regressions = 0
    checked = 0
    for current_path in current_files:
        target = current_path.stem
        baseline_path = args.baseline / current_path.name
        if not baseline_path.is_file():
            print(f"skip {target}: no baseline report ({baseline_path.name})")
            continue

        result, summary = compare_reports(baseline_path, current_path, args.tolerance)
        checked += 1
        if result is None:
            print(f"skip {target}: {summary}")
            continue

        counts = summary
        decreased = counts.get(ReccmpDiffJudgement.DECREASE, 0)
        increased = counts.get(ReccmpDiffJudgement.INCREASE, 0)
        dropped = counts.get(ReccmpDiffJudgement.DROPPED, 0)
        print(
            f"{target}: {decreased} decreased, {increased} increased, "
            f"{counts.get(ReccmpDiffJudgement.NEW, 0)} new, {dropped} dropped"
        )
        for entry in result:
            print(
                f"  REGRESSION {entry['address']} {entry['name']}: "
                f"{entry['old']} -> {entry['new']}"
            )
        total_regressions += len(result)

    if checked == 0:
        print("no comparable reports (baseline is empty) - skipping regression check")
        return 0

    if total_regressions:
        print(
            f"\nFAILED: {total_regressions} function(s) regressed against the baseline."
        )
        return 1

    print("\nOK: no accuracy regressions.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
