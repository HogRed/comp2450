// COMP 2450 — Floor 1 starter
// bestiary/Bestiary.cpp — provided by the framework. Do not edit.
#include "Bestiary.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace dungeon {

std::vector<Monster> loadBestiary(const std::string& path) {
    std::vector<Monster> result;
    std::ifstream in(path);
    if (!in) {
        std::cerr << "The Bestiary tablet is missing or unreadable: "
                  << path << "\n";
        return result;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;

        Monster m;
        std::istringstream ss(line);
        std::string field;

        if (!std::getline(ss, m.name, '|')) continue;
        if (!std::getline(ss, field,  '|')) continue;
        m.hp = std::stoi(field);
        if (!std::getline(ss, field,  '|')) continue;
        m.attack = std::stoi(field);
        if (!std::getline(ss, m.weakness, '|')) continue;

        result.push_back(m);
    }
    return result;
}

void sortBestiary(std::vector<Monster>& bestiary) {
    std::sort(bestiary.begin(), bestiary.end(),
              [](const Monster& a, const Monster& b) {
                  return a.name < b.name;
              });
}

void printMonster(const Monster& m) {
    std::cout << m.name
              << "   HP " << m.hp
              << "   ATK " << m.attack
              << "   weakness: " << m.weakness
              << "\n";
}

}
