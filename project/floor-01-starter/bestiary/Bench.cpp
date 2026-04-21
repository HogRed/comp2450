// COMP 2450 — Floor 1 starter
// bestiary/Bench.cpp — provided by the framework. Do not edit.
//
// The benchmark harness. This is where Big-O stops being abstract.
//
// It builds a synthetic bestiary at a chosen size N, sorts it, then times
// all three search functions against two worst-case queries: the LAST
// element (forces linear to scan everything) and an ABSENT name (forces
// every algorithm to confirm "not here"). Each query runs many iterations
// so the per-call average is larger than the clock's resolution.
//
// Three things to pay attention to when you read a benchmark:
//   1. What's being measured? (Per-call microseconds.)
//   2. What's NOT being measured? (The sort. We amortize that by running
//      it once, outside the timed region.)
//   3. Can the optimizer cheat? (Yes — it'll delete work whose result
//      isn't used. The `g_benchSink` below is how we stop it.)

#include "Bench.h"
#include "Bestiary.h"
#include "Search.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace dungeon {

namespace {

// Build a synthetic bestiary of N monsters with names "Monster_0000001",
// "Monster_0000002", ... so lexicographic order matches numeric order.
// All stats are placeholders; this bestiary is for timing only.
std::vector<Monster> makeSynthetic(std::size_t n) {
    std::vector<Monster> v;
    v.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::ostringstream oss;
        oss << "Monster_" << std::setfill('0') << std::setw(7) << i;
        v.push_back({oss.str(), 10, 2, "fire"});
    }
    return v;
}

// A volatile sink the compiler cannot prove unused. WHY we need this:
// modern compilers aggressively delete work whose result is never
// observed. A loop like `for (i...) search(bestiary, name);` looks
// dead if the caller ignores the return value — the whole loop might
// disappear, and the benchmark reports 0.000 microseconds. Writing the
// return into a `volatile` location forces the compiler to keep the
// work. Portable across GCC, Clang, and MSVC; no inline assembly needed.
static volatile const void* g_benchSink = nullptr;

// Time `fn` across `iterations` calls, return the per-call average in
// microseconds. `template <typename F>` lets us accept any callable
// (a lambda, a function pointer, ...). The high_resolution_clock is
// usually nanosecond-resolution on modern machines — plenty for this.
template <typename F>
double avgMicros(F fn, std::size_t iterations) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < iterations; ++i) {
        g_benchSink = fn();  // result parked in the sink; compiler can't delete
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    double total = std::chrono::duration<double, std::micro>(t1 - t0).count();
    return total / static_cast<double>(iterations);
}

void printRow(std::size_t n, const std::string& which,
              double l, double b, double r) {
    std::cout << "  N=" << std::setw(7) << n
              << "  query=" << std::setw(6) << std::left << which << std::right
              << "  linear=" << std::setw(10) << std::fixed << std::setprecision(3) << l << " us"
              << "  binary=" << std::setw(8) << b << " us"
              << "  recursive=" << std::setw(8) << r << " us"
              << "\n";
}

}  // anonymous namespace

void runBenchmark(std::size_t n, std::size_t iterations) {
    auto bestiary = makeSynthetic(n);
    sortBestiary(bestiary);  // sort ONCE, outside the timed region

    // Two worst-case queries for the comparison:
    //   - "last" forces linearSearch to walk the entire vector
    //   - "absent" forces every algorithm to confirm the miss
    // Best case ("first element") would mislead you — linearSearch wins
    // in one step. We want the shape of the growth curve, which shows up
    // in worst-case comparisons.
    const std::string last_name   = bestiary.back().name;
    const std::string absent_name = "ZZZZZ_NO_SUCH_MONSTER";

    auto runOnce = [&](const std::string& target) {
        double l = avgMicros([&]{ return linearSearch          (bestiary, target); }, iterations);
        double b = avgMicros([&]{ return binarySearch          (bestiary, target); }, iterations);
        double r = avgMicros([&]{ return binarySearchRecursive(bestiary, target); }, iterations);
        return std::tuple<double,double,double>{l, b, r};
    };

    // C++17 structured bindings: `auto [a, b, c] = tuple` unpacks in one
    // line what used to take three std::get<> calls.
    auto [l1, b1, r1] = runOnce(last_name);
    printRow(n, "last",   l1, b1, r1);

    auto [l2, b2, r2] = runOnce(absent_name);
    printRow(n, "absent", l2, b2, r2);
}

void runBenchmarkSweep() {
    std::cout << "\n-- Race the Bestiary (1000 iterations per cell) --\n";
    for (std::size_t n : {std::size_t{10}, std::size_t{100}, std::size_t{1000},
                          std::size_t{10000}, std::size_t{100000}}) {
        runBenchmark(n);
    }
    std::cout << "\nNotice the curve: linear grows with N, binary grows with log N.\n"
              << "For tiny N the constants matter and the gap is small.\n"
              << "For large N the gap is enormous.\n\n";
}

}  // namespace dungeon
