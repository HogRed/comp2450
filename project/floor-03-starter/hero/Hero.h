// COMP 2450 — Floor 3 starter
// hero/Hero.h — you edit this ONCE, on Wednesday.
//
// Right now `inventory` is a std::vector<Item> — the Floor 2 baseline,
// which compiles and runs as-is. On WEDNESDAY, after you build Bag<T>,
// you flip the one token `std::vector<Item>` to `Bag<Item>` (and
// loadInventory's return type with it). The Hero struct doesn't care
// what container holds its loot — that's the whole point of the flip.
//
// This is the hero the rest of the game will grow around. Later floors
// will bolt on more: spell book (Floor 6, stack), action queue
// (Floor 7, queue), skill tree (Floor 8).
#pragma once
#include <string>
#include "Bag.h"
#include "Item.h"

namespace dungeon {

struct Hero {
    std::string       heroName;
    Bag<Item> inventory;
};

// Load a starting inventory from a pipe-delimited text file.
// File format, one per line:  name|weight|value
// Blank lines and lines starting with '#' are ignored.
// On read failure returns an empty vector.
Bag<Item> loadInventory(const std::string& path);

// Pretty-print the hero's inventory, numbered, to stdout.
// Matches the demo target on the Floor 2 student page.
void printInventory(const Hero& hero);

}
