// COMP 2450 — Floor 2 starter
// bestiary/Search.cpp — reference implementation from Floor 1.
//
// This file IS already done. You wrote it last week (or something like it);
// the starter ships the reference version so the `search` command still
// works while you focus on Floor 2. If you want to understand or tweak
// it, read Search.h for the contracts.

#include "Search.h"

namespace dungeon {

// ---- 1. Linear search ---------------------------------------------------

const Monster* linearSearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {
    for (const auto& m : bestiary) {
        if (m.name == name) return &m;
    }
    return nullptr;
}

// ---- 2. Iterative binary search -----------------------------------------
//
// Half-open interval [low, high). Half-open avoids the size_t underflow
// trap that the closed-interval version hits when the target is smaller
// than every element in the vector.

const Monster* binarySearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {
    std::size_t low  = 0;
    std::size_t high = bestiary.size();
    while (low < high) {
        std::size_t mid = low + (high - low) / 2;
        const std::string& here = bestiary[mid].name;
        if (here == name) return &bestiary[mid];
        if (here <  name) low  = mid + 1;
        else              high = mid;
    }
    return nullptr;
}

// ---- 3. Recursive binary search -----------------------------------------

namespace {
const Monster* binSearchRec(const std::vector<Monster>& bestiary,
                            const std::string&         name,
                            std::size_t                low,
                            std::size_t                high) {
    if (low >= high) return nullptr;
    std::size_t mid = low + (high - low) / 2;
    const std::string& here = bestiary[mid].name;
    if (here == name) return &bestiary[mid];
    if (here <  name) return binSearchRec(bestiary, name, mid + 1, high);
    return                   binSearchRec(bestiary, name, low,     mid);
}
}  // namespace

const Monster* binarySearchRecursive(const std::vector<Monster>& bestiary,
                                     const std::string&         name) {
    return binSearchRec(bestiary, name, 0, bestiary.size());
}

// ---- 4. The seam --------------------------------------------------------
//
// Iterative binary is the default: same Big-O as recursive, no call
// overhead per step, no stack risk on huge bestiaries.

const Monster* findMonster(const std::vector<Monster>& bestiary,
                           const std::string&         name) {
    return binarySearch(bestiary, name);
}

}  // namespace dungeon
