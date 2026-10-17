#!/usr/bin/env python3
# Reads the files produced by ``tools/run_reccmp.py`` and writes a small,
# self-contained directory:

#     _site/index.html      summary table, linking each report
#     _site/<TARGET>.html   the per-target reccmp HTML reports
#     _site/<TARGET>.svg    the per-target reccmp progress badges

# CI deploys this directory with ``actions/upload-pages-artifact`` so it is served
# from a stable URL, e.g. ``https://<owner>.github.io/<repo>/accuracy-report/``.
# README.md embeds ``.../accuracy-report/<TARGET>.svg`` and links each badge to its
# ``<TARGET>.html`` report, so the README is static and never has to be
# regenerated. The compared binaries are linked through nightly.link (the
# ``reccmp-builds`` artifact of the latest successful run), not hosted here.

# Usage:
#     tools/reccmp_site.py --json-dir reccmp-json --html-dir reccmp-html \\
#         --svg-dir reccmp-svg --out-dir _site/accuracy-report \\
#         [--run-url <url>] [--binaries-url <url>]

from __future__ import annotations

import argparse
import html
import shutil
import sys
from datetime import datetime, timezone
from pathlib import Path

from gen_reccmp_markers import TARGETS
from reccmp.compare.report import deserialize_reccmp_report, report_function_accuracy

_STYLE = """
:root { color-scheme: light dark; }
* { box-sizing: border-box; }
body {
  margin: 0 auto; max-width: 52rem; padding: 2rem 1rem;
  font: 16px/1.5 -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif;
  color: #1f2328; background: #ffffff;
}
h1 { font-size: 1.6rem; margin: 0 0 .25rem; }
p { margin: .25rem 0; }
ul { line-height: 1.8; }
.meta { color: #59636e; font-size: .85rem; }
table { border-collapse: collapse; width: 100%; margin: 1.25rem 0; }
th, td { padding: .4rem .6rem; border-bottom: 1px solid #d1d9e0; text-align: left; }
th { border-bottom-width: 2px; }
td.num, th.num { text-align: right; font-variant-numeric: tabular-nums; }
.mono { font-family: ui-monospace, SFMono-Regular, Menlo, Consolas, monospace; }
.badge { display: block; width: 100%; max-width: 18rem; }
a { color: #0969da; text-decoration: none; }
a:hover { text-decoration: underline; }
@media (prefers-color-scheme: dark) {
  body { color: #e6edf3; background: #0d1117; }
  .meta { color: #9198a1; }
  th, td { border-color: #30363d; }
  a { color: #4493f8; }
}
"""


def _page(title: str, body: str) -> str:
    return f"""<!doctype html>
<html lang='en'><head><meta charset='utf-8'>
<meta name='viewport' content='width=device-width, initial-scale=1'>
<title>{html.escape(title)}</title>
<style>{_STYLE}</style></head><body><main>
{body}
</main></body></html>
"""


def _rows(json_dir: Path) -> list[dict]:
    """Return one stats dict per target that has a compact JSON report."""
    rows = []
    for target_id in TARGETS:
        path = json_dir / f"{target_id}.json"
        if not path.is_file():
            continue
        report = deserialize_reccmp_report(path.read_text(encoding="utf-8"))
        report.update_function_count()
        implemented, _total, total_effective = report_function_accuracy(report)
        rows.append(
            {
                "target": target_id,
                "name": report.filename,
                "accuracy": (total_effective / implemented * 100.0)
                if implemented
                else 0.0,
                "implemented": implemented,
                "total": report.function_count,
            }
        )
    return rows


def _implemented_text(row: dict) -> str:
    if not row["total"]:
        return "n/a"
    pct = row["implemented"] / row["total"] * 100.0
    return f'{row["implemented"]} / {row["total"]} ({pct:.2f}%)'


def _write_index(
    path: Path,
    rows: list[dict],
    caption: str,
    run_url: str | None,
    binaries_url: str | None,
) -> None:
    head = [
        "<h1>D2MOO reccmp accuracy</h1>",
        "<p>1.10f retail binaries compared against our VC6 build by "
        '<a href="https://github.com/reccmp/reccmp">reccmp</a>.</p>',
        f'<p class="meta">{html.escape(caption)}'
        + (f' &middot; <a href="{html.escape(run_url)}">workflow run</a>'
           if run_url else "")
        + "</p>",
        "<table><tbody>",
    ]
    body = [
        f'<tr><td><a href="{r["target"]}.html">'
        f'<img class="badge" src="{r["target"]}.svg" alt="{html.escape(r["name"])}"></a></td>'
        f'<td class="num">{r["accuracy"]:.2f}%</td>'
        f'<td class="num">{html.escape(_implemented_text(r))}</td></tr>'
        for r in rows
    ]
    tail = ["</tbody></table>"]
    if binaries_url:
        tail.append(
            f'<p><a href="{html.escape(binaries_url)}">Binaries that were compared</a>'
            " (nightly.link, latest successful run)</p>"
        )
    path.write_text(
        _page("D2MOO reccmp accuracy", "\n".join(head + body + tail)),
        encoding="utf-8",
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json-dir", type=Path, default=Path("reccmp-json"))
    parser.add_argument("--html-dir", type=Path, default=Path("reccmp-html"))
    parser.add_argument("--svg-dir", type=Path, default=Path("reccmp-svg"))
    parser.add_argument("--out-dir", type=Path, default=Path("_site"))
    parser.add_argument("--run-url", help="link to the CI run")
    parser.add_argument(
        "--binaries-url",
        help="stable URL for the compared binaries (e.g. a nightly.link artifact)",
    )
    parser.add_argument("--caption", default=None, help="caption shown under the title")
    args = parser.parse_args()

    rows = _rows(args.json_dir)
    if not rows:
        print(f"error: no reports found in {args.json_dir}", file=sys.stderr)
        return 1

    caption = args.caption or datetime.now(timezone.utc).strftime("Updated %Y-%m-%d %H:%M UTC")

    args.out_dir.mkdir(parents=True, exist_ok=True)
    for target_id in TARGETS:
        report = args.html_dir / f"{target_id}.html"
        if report.is_file():
            shutil.copy2(report, args.out_dir / report.name)
        badge = args.svg_dir / f"{target_id}.svg"
        if badge.is_file():
            shutil.copy2(badge, args.out_dir / badge.name)

    _write_index(
        args.out_dir / "index.html", rows, caption, args.run_url, args.binaries_url
    )

    print(f"wrote {args.out_dir} ({len(rows)} targets)", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
