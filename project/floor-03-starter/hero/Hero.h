// COMP 2450 — Floor 2 starter
// hero/Hero.h — provided by the framework. Do not edit.
//
// This is the hero the rest of the game will grow around. Right now it's
// a name and an inventory. Later floors will bolt on more: spell book
// (Floor 6, stack), action queue (Floor 7, queue), skill tree (Floor 8).
#pragma once
#include <string>
#include <vector>
#include "Item.h"

namespace dungeon {

struct Hero {
    std::string       heroName;
    std::vector<Item> inventory;
};

// Load a starting inventory from a pipe-delimited text file.
// File format, one per line:  name|weight|value
// Blank lines and lines starting with '#' are ignored.
// On read failure returns an empty vector.
std::vector<Item> loadInventory(const std::string& path);

// Pretty-print the hero's inventory, numbered, to stdout.
// Matches the demo target on the Floor 2 student page.
void printInventory(const Hero& hero);

}
