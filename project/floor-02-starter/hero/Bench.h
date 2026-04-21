// COMP 2450 — Floor 2 starter
// hero/Bench.h — provided by the framework. Do not edit.
//
// The benchmark harness for Lab 2. Generates a synthetic inventory of
// the requested size, then times your mergeSort, your quicksort, and
// std::sort. Supports two optional flags for the lab questions:
//
//   --sorted       run on pre-sorted input (what happens when the
//                  data you're handed is ALREADY in order?)
//   --bad-pivot    replace the middle-pivot quicksort column with a
//                  first-element-pivot variant of quicksort that the
//                  harness ships itself. Summons the Pivot Wraith on
//                  sorted input — try combining this with --sorted.
#pragma once
#include <cstddef>

namespace dungeon {

struct SortBenchOptions {
    bool presorted = false;
    bool badPivot  = false;
};

// Run the sort benchmark for one inventory size.
// Each sort is executed `iterations` times and the per-run average
// (in milliseconds) is reported.
void runSortBenchmark(std::size_t n,
                      SortBenchOptions opts = {},
                      std::size_t iterations = 3);

// Run the default sort sweep: N = 10, 100, 1'000, 10'000, 100'000.
void runSortBenchmarkSweep(SortBenchOptions opts = {});

}
