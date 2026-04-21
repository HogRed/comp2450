// COMP 2450 — Floor 2 starter
// hero/Hero.cpp — provided by the framework. Do not edit.
#include "Hero.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace dungeon {

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
        it.weight = std::stod(field);
        if (!std::getline(ss, field,   '|')) continue;
        it.value = std::stoi(field);

        result.push_back(it);
    }
    return result;
}

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
        // restore default flags so the next call starts clean
        std::cout.unsetf(std::ios::fixed);
        std::cout.unsetf(std::ios::left);
    }
}

}  // namespace dungeon
