// COMP 2450 — Floor 1 starter
// bestiary/Bestiary.cpp — provided by the framework. Do not edit.
//
// Three framework services the rest of the game calls:
//   loadBestiary  — parse monsters.txt into a vector<Monster>
//   sortBestiary  — std::sort the vector by name (binary search's precondition)
//   printMonster  — pretty-print one monster
//
// You are reading this file, not writing it. The part worth internalizing
// is the division of labor: file I/O lives here, searching lives in
// Search.cpp, timing lives in Bench.cpp. One responsibility per .cpp is
// the discipline that keeps C++ projects from becoming unreadable.

#include "Bestiary.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace dungeon {

// Read a pipe-delimited file into a vector<Monster>. Format per line:
//     name|hp|attack|weakness
// Blank lines and lines starting with '#' are ignored (comments in data).
// On any parse failure we skip that line rather than abort — a single bad
// row should not kill the whole load.
std::vector<Monster> loadBestiary(const std::string& path) {
    std::vector<Monster> result;
    std::ifstream in(path);
    if (!in) {
        // ifstream is false when the file couldn't be opened. Print once
        // and return empty; main.cpp will notice and bail.
        std::cerr << "The Bestiary tablet is missing or unreadable: "
                  << path << "\n";
        return result;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;

        Monster m;
        std::istringstream ss(line);  // treat the line as its own stream
        std::string field;

        // `getline(ss, out, '|')` reads up to the next '|' into `out`.
        // Returning false means "stream is exhausted"; we abandon this
        // row and move on. The chained ifs short-circuit on the first
        // missing field, which is the right thing for a malformed row.
        if (!std::getline(ss, m.name, '|')) continue;
        if (!std::getline(ss, field,  '|')) continue;
        m.hp = std::stoi(field);  // throws on non-integer; caught higher up if needed
        if (!std::getline(ss, field,  '|')) continue;
        m.attack = std::stoi(field);
        if (!std::getline(ss, m.weakness, '|')) continue;

        result.push_back(m);
    }
    return result;
    // NOTE: the return is a by-value vector, but C++ move semantics mean
    // we do NOT copy all the monsters back out. The vector's internal
    // heap buffer is moved into the caller's variable. This is why
    // "return by value" is idiomatic in modern C++ — it's free.
}

// Sort by name ascending. This is the function that UPHOLDS the
// precondition of binarySearch / binarySearchRecursive. main.cpp calls
// it once, right after loadBestiary, and never again — the vector is
// never re-shuffled, so the precondition stays true forever.
//
// The lambda `[](const Monster& a, const Monster& b) { return a.name < b.name; }`
// is the comparator. This is the same shape you'll meet in Floor 2 —
// except there you'll be *writing* the sort, not calling std::sort.
void sortBestiary(std::vector<Monster>& bestiary) {
    std::sort(bestiary.begin(), bestiary.end(),
              [](const Monster& a, const Monster& b) {
                  return a.name < b.name;
              });
}

// Pretty-print one monster. `const Monster&` is the no-copy, read-only
// argument shape. If we took `Monster m` we'd copy; if we took `Monster&`
// we'd say "I might modify it," which is a lie. `const&` is honest.
void printMonster(const Monster& m) {
    std::cout << m.name
              << "   HP " << m.hp
              << "   ATK " << m.attack
              << "   weakness: " << m.weakness
              << "\n";
}

}
