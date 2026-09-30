// COMP 2450 — Warden of the Foundations starter (post-Floor-3 reference state)
// bestiary/Search.h — post-Floor-3 reference.
//
// Floor 1's three Monster-specific searches are kept here as a
// historical reference (and still drive bestiary/Bench.cpp, which
// builds its own synthetic std::vector<Monster> internally). They are
// no longer called from main.cpp — the `search` command now calls the
// Floor 3 function template findByName<T> against both the bestiary
// (a Bag<Monster>) and the hero's inventory (a Bag<Item>).
//
// findByName<T>'s body lives in this header. Template bodies must be
// visible to every caller so the compiler can stamp the right version
// for each T. A .cpp file would only build it for the types it
// happened to name inside itself — useless for a generic search.
// Function templates in .h. That is a language rule, not a style choice.

#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <algorithm> //std::find_if

#include "../hero/Bag.h"
#include "Monster.h"

namespace dungeon {

// ---- Floor 1 reference: Monster-specific searches ---------------------
// Kept for the bestiary benchmark only. main.cpp uses findByName<Monster>.

// Linear scan. Works on any order.    O(n).
const Monster* linearSearch(const std::vector<Monster>& bestiary,
                            const std::string&         name);

// Iterative binary search.            O(log n). Pre: sorted.
const Monster* binarySearch(const std::vector<Monster>& bestiary,
                            const std::string&         name);

// Recursive binary search.            O(log n). Pre: sorted.
const Monster* binarySearchRecursive(const std::vector<Monster>& bestiary,
                                     const std::string&         name);

// Floor 1's seam. Kept for completeness; superseded by findByName<T>.
const Monster* findMonster(const std::vector<Monster>& bestiary,
                           const std::string&         name);


// ---- Floor 3 (Mon → Wed): function template ---------------------------
//
// Generalises Floor 1's linearSearch. ONE source body; the compiler
// stamps a fresh copy for every type T you instantiate it with, as long
// as T has a member called `name` comparable to a std::string via ==.
//
// In main.cpp the `search` command calls BOTH instantiations from the
// same source template:
//
//     findByName(bestiary,       rest)   →  const Monster*
//     findByName(hero.inventory, rest)   →  const Item*
//
// Big-O:    O(n). Linear scan, same as Floor 1's linearSearch.
// Returns:  pointer into the container at the first match, or nullptr.
// Pre:      T must have a data member `name` comparable to std::string.

template <typename T>
const T* findByName(const Bag<T>& items, const std::string& name) {
    // return a pointer to the first match or nullptr
    // &*it --> dereference the iterator to a reference,
    // then takes its address
    // standard for turning our iterator back into
    // raw pointer
    auto it = std::find_if(items.begin(), items.end()),
        [&name](const T& x) {return x.name == name; });
    return (it != items.end()) ? &*it : nullptr;
    // main.cpp
    // 3 branches
    // findByName<Monster>(bestiary, rest) <-- Bag
    // findByName<Item>(hero.inventory, rest) <-- Bag
    // std::find_if(hero.eventLog.begin(),...) <-- our chain
}

}  // namespace dungeon
