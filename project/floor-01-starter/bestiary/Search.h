// COMP 2450 — Floor 1 starter
// bestiary/Search.h — interface only. Provided by the framework.
//
// You implement Search.cpp this week. The rest of the game calls only
// through these three functions; if your code is correct, the `search`
// command works.
#pragma once
#include <string>
#include <vector>
#include "Monster.h"

namespace dungeon {

// Linear scan over the bestiary.
//   Returns: pointer into the vector at the matching Monster, or nullptr.
//   Pre:     none — works on any order.
//   Big-O:   O(n) worst case.
const Monster* linearSearch(const std::vector<Monster>& bestiary,
                            const std::string&         name);

// Iterative binary search over the bestiary by name.
//   Returns: pointer into the vector at the matching Monster, or nullptr.
//   Pre:     bestiary MUST be sorted ascending by name. If it isn't, you
//            will meet the Unsorted Lich (silent wrong answers).
//   Big-O:   O(log n) worst case.
const Monster* binarySearch(const std::vector<Monster>& bestiary,
                            const std::string&         name);

// The function the rest of the game calls when it needs to look up a monster.
// You decide which of the two searches above to delegate to.
// Defend your choice in your commit message.
const Monster* findMonster(const std::vector<Monster>& bestiary,
                           const std::string&         name);

}
