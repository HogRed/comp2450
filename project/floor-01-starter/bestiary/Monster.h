// COMP 2450 — Floor 1 starter
// bestiary/Monster.h — provided by the framework. Do not edit.
//
// A `struct` in C++ is a `class` whose members default to public. For a
// plain data record like this one (no invariants, no private state),
// struct is the right choice and the convention across the C++ community.
// Later floors may promote Monster to a class when we give it behavior.
#pragma once
#include <string>

namespace dungeon {

struct Monster {
    std::string name;      // lookup key. Sorted lexicographically for binarySearch.
    int         hp;        // placeholder for combat (Floor 9+).
    int         attack;
    std::string weakness;  // "fire", "ice", etc. Future floors use this for damage multipliers.
};

}
