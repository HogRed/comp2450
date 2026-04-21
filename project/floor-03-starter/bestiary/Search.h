// COMP 2450 — Floor 3 starter
// bestiary/Search.h — Monster-specific searches from Floor 1 (kept as
// reference) PLUS the new function template you write this week.
//
// Floor 1's three searches are still here. The `search` command still
// calls them. The `benchmark` harness still times them. You are not
// editing them this week.
//
// What's NEW on Floor 3 is a function template that generalizes the
// linear-search pattern: ONE source body, the compiler stamps a fresh
// copy for every T you instantiate it with. Monday's lesson, in code.
//
// Notice that findByName<T>'s body lives in THIS header. Template bodies
// must be visible to every caller so the compiler can stamp the right
// version for each T. A .cpp file would only build it for the types it
// happened to name inside itself — useless. Function templates in .h.
// That's a language rule, not a style choice.

#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "Monster.h"

namespace dungeon {

// ---- Floor 1 reference: Monster-specific searches ---------------------
//
// Linear scan. Works on any order.    O(n).
const Monster* linearSearch(const std::vector<Monster>& bestiary,
                            const std::string&         name);

// Iterative binary search.            O(log n). Pre: sorted.
const Monster* binarySearch(const std::vector<Monster>& bestiary,
                            const std::string&         name);

// Recursive binary search.            O(log n). Pre: sorted.
const Monster* binarySearchRecursive(const std::vector<Monster>& bestiary,
                                     const std::string&         name);

// The seam Floor 1 introduced: one place the rest of the game calls.
const Monster* findMonster(const std::vector<Monster>& bestiary,
                           const std::string&         name);


// ---- Floor 3 (Mon): function template ---------------------------------
//
// Generalize Floor 1's linearSearch. The old version was HARDCODED to
// std::vector<Monster> and compared against m.name. findByName<T> is a
// TEMPLATE — you write ONE source body and the compiler stamps it out
// for every type T you call it with, as long as T has a member called
// `name` comparable to a std::string via ==.
//
// In main.cpp the `search` command will call BOTH of these, same source
// template, two instantiations:
//
//     findByName(bestiary,       rest)   →  const Monster*
//     findByName(hero.inventory, rest)   →  const Item*
//
// Big-O:    O(n). Linear scan, same as Floor 1's linearSearch.
// Returns:  pointer into the container at the first match, or nullptr.
// Pre:      T must have a data member `name` comparable to std::string.
//
// Signature note: we take `const std::vector<T>&` this week. On
// Wednesday, after you write Bag<T>, change this parameter type to
// `const Bag<T>&`. The BODY will not have to change — that's the whole
// point of Wednesday's class-template lesson.

template <typename T>
const T* findByName(const std::vector<T>& items, const std::string& name) {
    // TODO Floor 3 (Mon):
    //   Walk the container. Return the address of the first element
    //   whose .name equals `name`. Return nullptr if none match.
    //
    //   Your body is about three lines. Resist the urge to write more.
    //   The whole lesson is that this ONE body works for Monster AND
    //   Item (and anything else with a .name) without you writing it
    //   twice.
    //
    //   Try this in your head before you type:
    //     - What does `for (const auto& it : items)` give you, since
    //       `items` is a std::vector<T>?
    //     - What expression takes the address of the current element?
    //     - Floor 1's linearSearch is literally this function with T
    //       hardcoded to Monster. Copy its SHAPE; drop the hardcode.
    (void)items;
    (void)name;
    return nullptr;
}

}  // namespace dungeon
