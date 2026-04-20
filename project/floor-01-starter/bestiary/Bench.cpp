// COMP 2450 — Floor 1 starter
// bestiary/Bench.cpp — provided by the framework. Do not edit.
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

template <typename F>
double avgMicros(F fn, std::size_t iterations) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < iterations; ++i) {
        auto r = fn();
        // Defeat over-aggressive optimization: store the pointer somewhere
        // observable. (Compiler can't prove the loop is dead now.)
        asm volatile("" : : "r"(r) : "memory");
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
    sortBestiary(bestiary);

    const std::string last_name   = bestiary.back().name;
    const std::string absent_name = "ZZZZZ_NO_SUCH_MONSTER";

    auto runOnce = [&](const std::string& target) {
        double l = avgMicros([&]{ return linearSearch          (bestiary, target); }, iterations);
        double b = avgMicros([&]{ return binarySearch          (bestiary, target); }, iterations);
        double r = avgMicros([&]{ return binarySearchRecursive(bestiary, target); }, iterations);
        return std::tuple<double,double,double>{l, b, r};
    };

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
