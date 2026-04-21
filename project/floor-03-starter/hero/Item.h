// COMP 2450 — Floor 2 starter
// hero/Item.h — provided by the framework. Do not edit.
//
// An Item is a plain value type — name, weight, value. Nothing fancier
// than what you could express in Python as a dataclass. The shape of
// the data is small on purpose: this week's lesson is sorting, not
// modeling. Later floors will inherit from Item (Weapon, Consumable,
// Scroll) once we tackle polymorphism.
#pragma once
#include <string>

namespace dungeon {

struct Item {
    std::string name;     // the sort key when sorting by "name"
    double      weight;   // in hypothetical encumbrance units (sort by "weight")
    int         value;    // in copper pieces (sort by "value")
};

}
