// COMP 2450 — Floor 1 starter
// bestiary/Search.cpp — YOU implement this file.
//
// Three functions to write. Read Search.h for their contracts.
//
// Tips for the Unsorted Lich:
//   * binarySearch only works if the bestiary is sorted by name.
//   * main.cpp already calls sortBestiary() right after loading,
//     so you can assume the precondition holds when binarySearch runs.
//   * If you ever doubt, scan the vector and assert it is sorted.
//
// Submit when:  `search goblin` returns the goblin's stats and `search ghost`
//               reports "no such creature."

#include "Search.h"

namespace dungeon {

const Monster* linearSearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {
    // TODO Floor 1: walk every entry; return its address when name matches.
    //               If you reach the end without a match, return nullptr.
    (void)bestiary;
    (void)name;
    return nullptr;
}

const Monster* binarySearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {
    // TODO Floor 1: iterative binary search.
    //   Maintain low / high indices and compare the middle entry's name
    //   against the target. Beware off-by-one — use the half you mean.
    //   PRECONDITION: bestiary is sorted ascending by name.
    (void)bestiary;
    (void)name;
    return nullptr;
}

const Monster* findMonster(const std::vector<Monster>& bestiary,
                           const std::string&         name) {
    // TODO Floor 1: pick ONE of the two searches above and delegate.
    //   Why that one? Put the answer in your commit message.
    return linearSearch(bestiary, name);
}

}
