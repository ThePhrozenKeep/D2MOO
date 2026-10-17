#!/usr/bin/env python3
# Reads the files produced by ``tools/run_reccmp.py`` and writes a small,
# self-contained directory:

#     _site/index.html      summary table, linking each report
#     _site/accuracy.svg    the same table as an image, embedded by README.md
#     _site/<TARGET>.html   the per-target reccmp HTML reports

# CI deploys this directory with ``actions/upload-pages-artifact`` so it is served
# from a stable URL, e.g. ``https://<owner>.github.io/<repo>/accuracy-report/``.
# README.md embeds ``.../accuracy-report/accuracy.svg`` and links the report page,
# so the README is static and never has to be regenerated. The compared binaries
# are linked through nightly.link (the ``reccmp-builds`` artifact of the latest
# successful run), not hosted here.

# Usage:
#     tools/reccmp_site.py --json-dir reccmp-json --html-dir reccmp-html \\
#         --out-dir _site/accuracy-report [--run-url <url>] [--binaries-url <url>]

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
a { color: #0969da; text-decoration: none; }
a:hover { text-decoration: underline; }
@media (prefers-color-scheme: dark) {
  body { color: #e6edf3; background: #0d1117; }
  .meta { color: #9198a1; }
  th, td { border-color: #30363d; }
  a { color: #4493f8; }
}
"""

_SVG_STYLE = (
    "text{font-family:ui-monospace,SFMono-Regular,Menlo,Consolas,monospace;}"
    ".t{font-size:17px;font-weight:700;fill:#1f2328;}"
    ".s{font-size:12px;fill:#59636e;}"
    ".c{font-size:13px;fill:#1f2328;}"
)


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


def _write_svg(path: Path, rows: list[dict], caption: str) -> None:
    row_h, head_h, foot_h, width = 26, 100, 24, 780
    height = head_h + row_h * len(rows) + foot_h
    bin_x, acc_x, impl_x = 24, 560, width - 24
    out = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" '
        f'height="{height}" viewBox="0 0 {width} {height}" role="img">',
        "<title>D2MOO reccmp accuracy</title>",
        f"<style>{_SVG_STYLE}</style>",
        f'<rect x="0.5" y="0.5" width="{width - 1}" height="{height - 1}" '
        'rx="8" fill="#ffffff" stroke="#d1d9e0"/>',
        f'<text x="{bin_x}" y="34" class="t">D2MOO - reccmp accuracy</text>',
        f'<text x="{bin_x}" y="54" class="s">1.10f retail binaries vs our VC6 build</text>',
        f'<text x="{bin_x}" y="74" class="s">{html.escape(caption)}</text>',
        f'<text x="{bin_x}" y="{head_h - 10}" class="s">Binary</text>',
        f'<text x="{acc_x}" y="{head_h - 10}" class="s" text-anchor="end">Accuracy</text>',
        f'<text x="{impl_x}" y="{head_h - 10}" class="s" text-anchor="end">Implemented</text>',
        f'<line x1="{bin_x}" y1="{head_h}" x2="{impl_x}" y2="{head_h}" stroke="#d1d9e0"/>',
    ]
    y = head_h + 18
    for row in rows:
        out.append(f'<text x="{bin_x}" y="{y}" class="c">{html.escape(row["name"])}</text>')
        out.append(
            f'<text x="{acc_x}" y="{y}" class="c" text-anchor="end">'
            f'{row["accuracy"]:.2f}%</text>'
        )
        out.append(
            f'<text x="{impl_x}" y="{y}" class="c" text-anchor="end">'
            f'{html.escape(_implemented_text(row))}</text>'
        )
        y += row_h
    out.append("</svg>")
    path.write_text("\n".join(out) + "\n", encoding="utf-8")


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
        + (f' &middot; <a href="{html.escape(run_url)}">workflow run</a>' if run_url else "")
        + "</p>",
        "<table><thead><tr><th>Binary</th>"
        '<th class="num">Accuracy</th><th class="num">Implemented</th><th></th>'
        "</tr></thead><tbody>",
    ]
    body = [f'<tr><td class="mono">{html.escape(r["name"])}</td>'
            f'<td class="num">{r["accuracy"]:.2f}%</td>'
            f'<td class="num">{html.escape(_implemented_text(r))}</td>'
            f'<td><a href="{r["target"]}.html">report</a></td></tr>'
            for r in rows]
    tail = ["</tbody></table>"]
    if binaries_url:
        tail.append(
            f'<p><a href="{html.escape(binaries_url)}">Binaries that were compared</a>'
            " (nightly.link, latest successful run)</p>"
        )
    path.write_text(_page("D2MOO reccmp accuracy", "\n".join(head + body + tail)), encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json-dir", type=Path, default=Path("reccmp-json"))
    parser.add_argument("--html-dir", type=Path, default=Path("reccmp-html"))
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

    _write_svg(args.out_dir / "accuracy.svg", rows, caption)
    _write_index(
        args.out_dir / "index.html", rows, caption, args.run_url, args.binaries_url
    )

    print(f"wrote {args.out_dir} ({len(rows)} targets)", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
