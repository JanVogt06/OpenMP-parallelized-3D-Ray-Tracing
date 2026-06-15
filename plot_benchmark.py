"""
Benchmark visualisation for OpenMP ray tracer (Aufgabe 7.2).
Reads bench_results.json, prints tables, fits Amdahl's law, and saves a plot.
"""

import json, math
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

def curve_fit_amdahl(ps, sp):
    """Least-squares fit for Amdahl's f via grid search (no scipy needed)."""
    best_f, best_err = 0.0, float("inf")
    for f in np.linspace(1e-6, 0.9999, 10000):
        pred = 1.0 / (f + (1.0 - f) / ps)
        err  = np.sum((pred - sp) ** 2)
        if err < best_err:
            best_err, best_f = err, f
    return best_f

# ---------- load data ----------
with open("bench_results.json") as f:
    raw = json.load(f)

resolutions = [(512, 512), (1024, 1024), (2048, 2048)]
threads     = [1, 2, 4, 8, 16, 24]

data = {}   # (W,H) -> {p: ms}
for entry in raw.values():
    key = (entry["W"], entry["H"])
    data.setdefault(key, {})[entry["p"]] = entry["ms"]

# ---------- print tables ----------
for (W, H) in resolutions:
    t1 = data[(W, H)][1]
    print(f"\n=== {W}x{H} ===")
    print(f"{'p':>4}  {'T(p) [ms]':>12}  {'S(p)':>8}  {'E(p)':>8}")
    print("-" * 38)
    for p in threads:
        tp = data[(W, H)][p]
        sp = t1 / tp
        ep = sp / p
        print(f"{p:4d}  {tp:12.1f}  {sp:8.3f}  {ep:8.3f}")

# ---------- Amdahl fit ----------
def amdahl(p, f):
    return 1.0 / (f + (1.0 - f) / np.asarray(p, dtype=float))

print("\n=== Amdahl's law fit (serial fraction f) ===")
for (W, H) in resolutions:
    t1 = data[(W, H)][1]
    ps = np.array(threads, dtype=float)
    sp = np.array([t1 / data[(W, H)][p] for p in threads])
    f_est = curve_fit_amdahl(ps, sp)
    s_max = 1.0 / f_est if f_est > 0 else float("inf")
    print(f"  {W}x{H}: f = {f_est:.4f}  ->  S_max = {s_max:.1f}x")

# ---------- plot ----------
fig, axes = plt.subplots(1, 3, figsize=(13, 4), sharey=False)
ideal_p  = np.array([1, 2, 4, 8, 16, 24])
ideal_sp = ideal_p.astype(float)

markers = ["o", "s", "^"]
colors  = ["steelblue", "darkorange", "seagreen"]

for ax, (W, H), marker, color in zip(axes, resolutions, markers, colors):
    t1 = data[(W, H)][1]
    ps = np.array(threads)
    sp = np.array([t1 / data[(W, H)][p] for p in threads])

    ax.plot(ideal_p, ideal_sp, "k--", linewidth=1, label="Ideal")
    ax.plot(ps, sp, marker=marker, color=color, linewidth=1.8,
            markersize=6, label=f"{W}×{H}")

    # Amdahl curve
    f_fit  = curve_fit_amdahl(ps, sp)
    p_fine = np.linspace(1, 24, 200)
    ax.plot(p_fine, amdahl(p_fine, f_fit), color=color,
            linestyle=":", linewidth=1.2, label=f"Amdahl (f={f_fit:.3f})")

    ax.set_title(f"{W}×{H}")
    ax.set_xlabel("Threads p")
    ax.set_ylabel("Speedup S(p)")
    ax.set_xticks(threads)
    ax.set_xlim(0, 25)
    ax.set_ylim(0, max(ideal_sp) + 1)
    ax.legend(fontsize=8)
    ax.grid(True, linestyle="--", alpha=0.4)

fig.suptitle("OpenMP Ray Tracer — Speedup vs. Thread Count", fontsize=13)
fig.tight_layout()
fig.savefig("speedup_plot.png", dpi=150)
print("\nPlot saved to speedup_plot.png")
