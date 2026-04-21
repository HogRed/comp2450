// COMP 2450 — Floor 2 starter
// hero/Item.h — provided by the framework. Do not edit.
#pragma once
#include <string>

namespace dungeon {

struct Item {
    std::string name;
    double      weight;   // in hypothetical encumbrance units
    int         value;    // in copper pieces
};

}
