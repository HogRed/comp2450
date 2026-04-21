// COMP 2450 — Floor 1 starter
// bestiary/Bench.h — provided by the framework. Do not edit.
//
// The benchmark harness for Lab 1. Generates a synthetic bestiary at the
// requested size, sorts it, and times all three of your search functions
// against the worst-case query (last element) and an absent query.
#pragma once
#include <cstddef>

namespace dungeon {

// Run the benchmark for one bestiary size. Prints two rows to stdout:
// one for the "last element" query, one for the "absent" query.
// Each search is executed `iterations` times and the per-call average
// (in microseconds) is reported.
void runBenchmark(std::size_t n, std::size_t iterations = 1000);

// Run the default sweep: N = 10, 100, 1'000, 10'000, 100'000.
// Useful for a single-command Lab 1 deliverable.
void runBenchmarkSweep();

}
