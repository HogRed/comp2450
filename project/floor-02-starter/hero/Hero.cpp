// COMP 2450 — Floor 2 starter
// hero/Hero.cpp — provided by the framework. Do not edit.
//
// Two framework services: load the starter inventory from disk, and
// pretty-print the hero's current inventory. This mirrors Bestiary.cpp
// by design — once you've read one, you've read the other. The point
// is that EVERY on-disk collection in this project uses the same
// "pipe-delimited, one record per line, # for comments" format, so
// adding new data types later is cheap.

#include "Hero.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace dungeon {

// Read the starter inventory. Same shape as Bestiary's loader; skip
// blank/commented lines, abandon any malformed row rather than abort.
std::vector<Item> loadInventory(const std::string& path) {
    std::vector<Item> result;
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Your satchel is missing: " << path << "\n";
        return result;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;

        Item it;
        std::istringstream ss(line);
        std::string field;

        if (!std::getline(ss, it.name, '|')) continue;
        if (!std::getline(ss, field,   '|')) continue;
        it.weight = std::stod(field);       // stod = "string to double"
        if (!std::getline(ss, field,   '|')) continue;
        it.value = std::stoi(field);        // stoi = "string to int"

        result.push_back(it);
    }
    return result;
}

// Pretty-print the inventory. The std::setw / std::left dance aligns
// the columns so a visual scan catches ties (Iron key and Loaf of bread
// both at wt 0.1, for instance — the stability demo).
void printInventory(const Hero& hero) {
    if (hero.inventory.empty()) {
        std::cout << "Your satchel is empty.\n";
        return;
    }
    for (std::size_t i = 0; i < hero.inventory.size(); ++i) {
        const Item& it = hero.inventory[i];
        std::cout << "  " << std::setw(2) << std::right << (i + 1) << ".  "
                  << std::setw(18) << std::left << it.name
                  << "(wt " << std::fixed << std::setprecision(1) << it.weight
                  << ", val " << it.value << ")\n";
        // std::cout is a GLOBAL object. If we left `std::fixed` and
        // `std::left` set, they'd leak into the NEXT thing anyone prints.
        // Always unset formatting flags you set inside a local function.
        std::cout.unsetf(std::ios::fixed);
        std::cout.unsetf(std::ios::left);
    }
}

}  // namespace dungeon
