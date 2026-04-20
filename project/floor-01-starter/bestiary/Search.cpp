// COMP 2450 — Floor 1 starter
// bestiary/Search.cpp — YOU implement this file.
//
// Four functions to write. Read Search.h for their contracts.
//
// Tips for the Unsorted Lich:
//   * binarySearch (and binarySearchRecursive) only work if the bestiary
//     is sorted by name.
//   * main.cpp already calls sortBestiary() right after loading,
//     so you can assume the precondition holds when these run.
//   * If you ever doubt, scan the vector and assert it is sorted.
//
// Submit when:  `search Goblin` returns the goblin's stats and `search Ghost`
//               reports "no such creature." Then run `benchmark` and capture
//               the output for your lab-notes.md.

#include "Search.h"

namespace dungeon {

const Monster* linearSearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {
    // TODO Floor 1 (Mon): walk every entry; return its address when name matches.
    //                     If you reach the end without a match, return nullptr.
    (void)bestiary;
    (void)name;
    return nullptr;
}

const Monster* binarySearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {
    // TODO Floor 1 (Wed): iterative binary search.
    //   Maintain low / high indices and compare the middle entry's name
    //   against the target. Beware off-by-one — use the half you mean.
    //   PRECONDITION: bestiary is sorted ascending by name.
    (void)bestiary;
    (void)name;
    return nullptr;
}

const Monster* binarySearchRecursive(const std::vector<Monster>& bestiary,
                                     const std::string&         name) {
    // TODO Floor 1 (Fri): same contract as binarySearch, but recursive.
    //   Recommended pattern: write a static helper in this file with extra
    //   (low, high) parameters, and have this public function call it with
    //   the initial range. Same precondition: bestiary must be sorted.
    (void)bestiary;
    (void)name;
    return nullptr;
}

const Monster* findMonster(const std::vector<Monster>& bestiary,
                           const std::string&         name) {
    // TODO Floor 1: pick ONE of the three searches above and delegate.
    //   Why that one? Put the answer in your commit message.
    return linearSearch(bestiary, name);
}

}
