// COMP 2450 — Floor 4½ starter
// hero/Hero.cpp — printLog is Floor 4's finished version, kept as-is.
// printLogOldest is the new Floor 4½ renderer that walks the chain
// BACKWARD using the new `prev` pointers — its stub is your Monday job.

#include "Hero.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace dungeon {

// Read the starter inventory. Same shape as Bestiary's loader; skip
// blank/commented lines, abandon any malformed row rather than abort.
Bag<Item> loadInventory(const std::string& path) {
    Bag<Item> result;
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
        std::cout.unsetf(std::ios::fixed);
        std::cout.unsetf(std::ios::left);
    }
}

// Render the most recent `n` event-log entries, newest first. Floor 4's
// finished version — walks from head() following `next`.
void printLog(const Hero& hero, std::size_t n) {
    if (hero.eventLog.empty()) {
        std::cout << "  (the chain is empty — nothing to remember yet)\n";
        return;
    }
    std::size_t printed = 0;
    for (const auto* p = hero.eventLog.head();
         p != nullptr && (n == 0 || printed < n);
         p = p->next, ++printed) {
        std::cout << "  " << std::setw(2) << std::right
                  << (printed + 1) << ".  " << p->data << "\n";
    }
    std::cout << "  (newest first; chain length "
              << hero.eventLog.size() << ")\n";
}

// Render the OLDEST `n` event-log entries, oldest first. Floor 4½'s
// new renderer — walks from tail() following `prev`. Mirrors printLog.
//
// TODO Floor 4½ (Monday) — replace the body with a backward hand-walk:
//
//     std::size_t printed = 0;
//     for (const auto* p = hero.eventLog.tail();
//          p != nullptr && (n == 0 || printed < n);
//          p = p->prev, ++printed) {
//         std::cout << "  " << std::setw(2) << std::right
//                   << (printed + 1) << ".  " << p->data << "\n";
//     }
//     std::cout << "  (oldest first; chain length "
//               << hero.eventLog.size() << ")\n";
//
// Until you implement this, `log --oldest` will print a placeholder
// and the build will stay green so the rest of the game still runs.
void printLogOldest(const Hero& hero, std::size_t n) {
    (void)n;
    if (hero.eventLog.empty()) {
        std::cout << "  (the chain is empty — nothing to remember yet)\n";
        return;
    }
    std::size_t printed = 0;
    for (const auto* p = hero.eventLog.tail();
        p != nullptr && (n == 0 || printed < n);
        p = p->prev, ++printed) {
        std::cout << " " << std::setw(2) << std::right
            << (printed + 1) << ". " << p->data << "\n";
    }
    std::cout << "oldest first; chain length "
        << hero.eventLog.size() << "\n";
}

}  // namespace dungeon
