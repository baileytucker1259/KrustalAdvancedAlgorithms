#!/usr/bin/env python3
"""Plot PA1 benchmark CSV. Usage: python plot_results.py results.csv"""

import sys
from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


def main() -> None:
    if len(sys.argv) != 2:
        print("Usage: python plot_results.py <results.csv>", file=sys.stderr)
        sys.exit(1)

    df = pd.read_csv(sys.argv[1])
    out_dir = Path(sys.argv[1]).resolve().parent
    out_dir.mkdir(exist_ok=True)

    uf = df[df["benchmark"] == "uf_unite"]
    if not uf.empty:
        fig, ax = plt.subplots(figsize=(8, 5))
        order = ["Naive", "PathCompression", "Rank"]
        for variant in order:
            g = uf[uf["variant"] == variant].sort_values("n")
            ax.plot(g["n"], g["median_ns"] / 1e6, marker="o", label=variant)
        ax.set_xlabel("n")
        ax.set_ylabel("Median time (ms)")
        ax.set_title("Union-Find variants, random unite sequence (m = 4n)")
        ax.legend()
        ax.grid(True, alpha=0.3)
        fig.tight_layout()
        fig.savefig(out_dir / "uf_variants.png", dpi=150)
        ax.set_yscale("log")
        ax.set_title("Union-Find variants, log scale (m = 4n)")
        fig.tight_layout()
        fig.savefig(out_dir / "uf_variants_log.png", dpi=150)

    order = ["Naive", "PathCompression", "Rank"]
    titles = {
        "kruskal": "Kruskal wall time (sort + Union-Find)",
        "kruskal_uf": "Kruskal Union-Find scan only",
        "kruskal_sort": "Kruskal sort only",
    }
    for benchmark, title in titles.items():
        block = df[df["benchmark"] == benchmark]
        for family in block["family"].unique():
            sub = block[block["family"] == family]
            if sub.empty:
                continue
            fig, ax = plt.subplots(figsize=(8, 5))
            for variant in order:
                group = sub[sub["variant"] == variant].sort_values("n")
                if group.empty:
                    continue
                ax.plot(group["n"], group["median_ns"] / 1e6, marker="o", label=variant)
            ax.set_xlabel("n")
            ax.set_ylabel("Median time (ms)")
            ax.set_title(f"{title}, {family}")
            ax.legend()
            ax.grid(True, alpha=0.3)
            fig.tight_layout()
            fig.savefig(out_dir / f"{benchmark}_{family}.png", dpi=150)

    print(f"Wrote plots to {out_dir}/")


if __name__ == "__main__":
    main()
