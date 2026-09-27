// COMP 2450 — Floor 2 starter
// hero/Bench.cpp — provided by the framework. Do not edit.
//
// The sort benchmark. Mirrors bestiary/Bench.cpp in shape: build a
// synthetic dataset, time three implementations, report averages.
//
// Two things this harness does that the Floor 1 one did not:
//   1. Fresh copy per iteration. Sorting is destructive — sorting an
//      already-sorted vector would measure "detect already-sorted" on
//      the second run, not the actual algorithm. We copy `base` into
//      a new vector inside each timed iteration.
//   2. First-element-pivot quicksort, baked in. The --bad-pivot flag
//      swaps out YOUR (middle-pivot) quicksort for this one, so you
//      can see the Pivot Wraith without having to go break your own
//      code. Do not imitate this. Its ONLY job is to be slow.

#include "Bench.h"
#include "Sort.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include "Bag.h"

namespace dungeon {

namespace {

// Build a synthetic inventory of N items with reproducible pseudo-random
// names, weights, and values. Deterministic across runs because we seed
// the RNG with a FIXED value (0xC0FFEE). Determinism matters in a
// benchmark — students on different machines should be comparing runs
// on the SAME data, not on different random draws.
Bag<Item> makeSynthetic(std::size_t n) {
    std::mt19937_64 rng(0xC0FFEE);  // 64-bit Mersenne Twister, fixed seed
    std::uniform_real_distribution<double> weightDist(0.1, 50.0);
    std::uniform_int_distribution<int>     valueDist(0, 1000);

    Bag<Item> v;
    for (std::size_t i = 0; i < n; ++i) {
        std::ostringstream oss;
        oss << "Item_" << std::setfill('0') << std::setw(7) << i;
        v.push_back({oss.str(), weightDist(rng), valueDist(rng)});
    }
    return v;
}

// The default ascending-weight comparator used throughout the bench.
const Comparator kCmpWeight = [](const Item& a, const Item& b) {
    return a.weight < b.weight;
};

// A first-element-pivot quicksort that lives ONLY inside the benchmark
// harness, so we can demonstrate the Pivot Wraith without relying on
// the student's own (middle-pivot) implementation. Lomuto partition.
//
// Why first-element pivot is pathological: on already-sorted input, the
// smallest element IS the first element. Lomuto partitions everything
// else onto the right side, so each level peels off exactly one element
// and the total work degenerates from O(n log n) to O(n^2). Try
// `benchmark sort --sorted --bad-pivot 10000` against a middle-pivot run
// at the same size to feel it.
void badQuicksortImpl(Bag<Item>& v,
                      std::size_t lo, std::size_t hi,
                      const Comparator& cmp) {
    // Recurse into the SMALLER side and iterate on the larger one. Naively
    // this recurses once per element on sorted input - N frames deep - which
    // overflows the stack and CRASHES long before the timing gets
    // interesting. This caps stack depth at O(log n) while leaving the
    // COMPARISON count untouched: the pathology is fully preserved, only the
    // crash is gone. A production quicksort uses the same trick.
    while (lo < hi) {
        // Always pick the FIRST element as pivot (the pathology). Swap it to
        // the end so the standard Lomuto scan - which assumes the pivot is at
        // v[hi] - still works.
        std::swap(v[lo], v[hi]);
        const Item pivot = v[hi];
        std::size_t store = lo;
        for (std::size_t i = lo; i < hi; ++i) {
            if (cmp(v[i], pivot)) {
                std::swap(v[store], v[i]);
                ++store;
            }
        }
        std::swap(v[store], v[hi]);

        const std::size_t p = store;
        // Left is [lo, p-1] (size p - lo); right is [p+1, hi] (size hi - p).
        // `p - 1` with std::size_t underflows when p == lo, so both branches
        // guard it - the same trap your own quicksort has to navigate.
        if (p - lo < hi - p) {
            if (p > lo) badQuicksortImpl(v, lo, p - 1, cmp);
            lo = p + 1;
        } else {
            badQuicksortImpl(v, p + 1, hi, cmp);
            if (p == lo) return;
            hi = p - 1;
        }
    }
}

void badQuicksort(Bag<Item>& v, const Comparator& cmp) {
    if (v.size() < 2) return;
    badQuicksortImpl(v, 0, v.size() - 1, cmp);
}

// A volatile sink the compiler cannot prove unused. We write the sorted
// vector's data pointer into it so the timing loop doesn't get eliminated
// by dead-store optimization. Portable across GCC, Clang, and MSVC;
// no inline assembly needed.
static volatile const void* g_benchSink = nullptr;

// Time a sort callable (which receives a fresh copy of `base` each run)
// across `iterations` iterations and return the average wall-clock ms.
//
// Notice the copy INSIDE the loop: `std::vector<Item> v = base;`. This
// is deliberate. If we sorted `base` once and reused it, the second
// iteration would already be sorted — we'd be timing "detect sorted,"
// not "perform sort." Every timed run starts from identical state.
//
// Notice also that the copy itself is OUTSIDE the timed region
// (before `t0`). We are measuring the sort, not the memcpy.
double avgMillis(const Bag<Item>& base,
                 const std::function<void(Bag<Item>&)>& sortFn,
                 std::size_t iterations) {
    double totalMs = 0.0;
    for (std::size_t i = 0; i < iterations; ++i) {
        Bag<Item> v = base;      // fresh unsorted copy per run (untimed)
        auto t0 = std::chrono::high_resolution_clock::now();
        sortFn(v);
        auto t1 = std::chrono::high_resolution_clock::now();
        totalMs += std::chrono::duration<double, std::milli>(t1 - t0).count();
        if (!v.empty()) g_benchSink = &v[0];          // defeat dead-store elimination
    }
    return totalMs / static_cast<double>(iterations);
}

void printRow(std::size_t n,
              double mergeMs, double quickMs, double stdMs,
              bool badPivot) {
    std::cout << "  N=" << std::setw(7) << std::right << n
              << "  mergeSort=" << std::setw(8) << std::fixed
                                << std::setprecision(3) << mergeMs << " ms"
              << "  " << (badPivot ? "quicksort*=" : "quicksort= ")
              << std::setw(8) << quickMs << " ms"
              << "  std::sort=" << std::setw(8) << stdMs << " ms"
              << "\n";
    std::cout.unsetf(std::ios::fixed);
}

}  // anonymous namespace

void runSortBenchmark(std::size_t n,
                      SortBenchOptions opts,
                      std::size_t iterations) {
    Bag<Item> base = makeSynthetic(n);
    if (opts.presorted) {
        std::sort(base.begin(), base.end(), kCmpWeight);
    }

    double m  = avgMillis(base,
                          [&](Bag<Item>& v) { mergeSort(v, kCmpWeight); },
                          iterations);
    double q  = avgMillis(base,
                          [&](Bag<Item>& v) {
                              if (opts.badPivot) badQuicksort(v, kCmpWeight);
                              else               quickSort(v, kCmpWeight);
                          },
                          iterations);
    double s  = avgMillis(base,
                          [&](Bag<Item>& v) {
                              std::sort(v.begin(), v.end(), kCmpWeight);
                          },
                          iterations);

    printRow(n, m, q, s, opts.badPivot);
}

void runSortBenchmarkSweep(SortBenchOptions opts) {
    std::cout << "\n-- Race the Sorts (" << (opts.presorted ? "pre-sorted input" : "random input")
              << (opts.badPivot ? ", BAD PIVOT quicksort" : "")
              << ") --\n";

    // With a bad pivot on sorted input the run is quadratic, so the top of
    // the sweep is not worth waiting for: N=100000 takes minutes and tells
    // you nothing N=10000 hasn't. Cap it, and say so, rather than appearing
    // to hang.
    const std::size_t badPivotCap = 10000;

    for (std::size_t n : {std::size_t{10}, std::size_t{100}, std::size_t{1000},
                          std::size_t{10000}, std::size_t{100000}}) {
        if (opts.badPivot && n > badPivotCap) {
            std::cout << "  N=" << std::setw(7) << std::right << n
                      << "  (skipped: quadratic with --bad-pivot, would take minutes)\n";
            continue;
        }
        runSortBenchmark(n, opts);
    }

    if (opts.badPivot) {
        std::cout << "\n(quicksort* = first-element pivot. Compare to a middle-pivot\n"
                  << " run, especially on --sorted input. The Pivot Wraith is real.)\n\n";
    } else {
        std::cout << "\nNotice: std::sort usually beats both of yours — smaller constant,\n"
                  << "cache-aware inner loops, introsort fallback. Your two exist to\n"
                  << "TEACH the shape; std::sort exists to SHIP.\n\n";
    }
}

}  // namespace dungeon
