// COMP 2450 — Floor 2 starter
// hero/Sort.h — interface only. Provided by the framework.
//
// You implement Sort.cpp this week. The rest of the game calls only
// through these three functions; if your code is correct, the
// `sort inventory by <key>` command works AND the `benchmark sort`
// command produces meaningful numbers.
#pragma once
#include <functional>
#include <string>
#include <vector>
#include "Hero.h"
#include "Item.h"

namespace dungeon {

// A Comparator takes two Items and returns true iff the first should
// come before the second in the sorted order. This is the SAME shape
// as the third argument to std::sort — so your mergeSort and quicksort
// accept the exact same lambdas that std::sort does. That's the whole
// point of taking a comparator: one sort routine, every ordering.
//
// (We use std::function here because templates don't land until
// Floor 3. The production version — what std::sort uses — is a
// template. Same idea, lower overhead.)
using Comparator = std::function<bool(const Item&, const Item&)>;


// Merge sort: stable, O(n log n) always, uses O(n) extra memory.
//   Side effect: sorts `inventory` in place.
//   Pre:         none.
//   Big-O:       O(n log n) best / average / worst.
void mergeSort(std::vector<Item>& inventory, const Comparator& cmp);

// Quicksort: in-place, O(n log n) average, but O(n^2) worst case.
//   Side effect: sorts `inventory` in place.
//   Pre:         none.
//   Big-O:       O(n log n) average, O(n^2) worst.
//   Use the MIDDLE element as the pivot. (A first- or last-element
//   pivot hits O(n^2) on already-sorted data — you will meet the
//   Pivot Wraith.)
void quicksort(std::vector<Item>& inventory, const Comparator& cmp);


// The function the rest of the game calls when it needs to sort the
// hero's inventory. Parses the criterion string the user typed after
// `sort inventory by`, builds the right comparator, dispatches to
// whichever sort you pick.
//
// Criterion forms the user might type:
//   "name"   "name asc"   "name desc"
//   "weight" "weight asc" "weight desc"
//   "value"  "value asc"  "value desc"
//
// Returns true on success, false on an unknown key (so main.cpp can
// print a helpful error).
bool sortInventory(Hero& hero, const std::string& criterion);

}
