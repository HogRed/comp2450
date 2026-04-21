// COMP 2450 — Floor 1 starter
// bestiary/Monster.h — provided by the framework. Do not edit.
#pragma once
#include <string>

namespace dungeon {

struct Monster {
    std::string name;
    int         hp;
    int         attack;
    std::string weakness;
};

}
