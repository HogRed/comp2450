// COMP 2450 — Floor 4 starter (post-Warden reference state + this week's stubs)
// hero/Hero.h — Floor 4 grows the hero by one field: a Chain<std::string>
// event log. Every notable thing the hero does or suffers is prepended
// to the chain in main.cpp; the `log` command displays the most recent N
// entries by walking the chain head-first.
//
// Note that Hero now owns a Chain, which (by Friday) will have its copy
// operations deleted. You should be passing Hero by reference everywhere
// already; if you weren't, today's the day.

#pragma once

#include <string>
#include <iomanip>

#include "Bag.h"
#include "Chain.h"
#include "Item.h"

namespace dungeon {

struct Hero {
    std::string         heroName;
    Bag<Item>           inventory;   // Floor 3: Bag<Item> (sortable, indexable).
    Chain<std::string>  eventLog;    // Floor 4: prepend-only history, newest at head.
};

// Load a starting inventory from a pipe-delimited text file.
// File format, one per line:  name|weight|value
// Blank lines and lines starting with '#' are ignored.
// On read failure returns an empty Bag.
Bag<Item> loadInventory(const std::string& path);

// Pretty-print the hero's inventory, numbered, to stdout.
void printInventory(const Hero& hero);

// Print the most recent `n` entries of the hero's event log, newest first.
// If `n` is 0 or larger than the chain, prints the whole chain.
//
// TODO Floor 4 (Monday) — implement in Hero.cpp. Walk the chain from
// hero.eventLog.head() forward, printing as you go, and stop after n
// entries. Print "(newest first; chain length K)" at the end where
// K = eventLog.size().
void printLog(const Hero& hero, std::size_t n) {
    if (hero.eventLog.empty()) {
        std::cout << "The chain is empty - nothing to remember yet. \n";
        return;
    }
    std::size_t printed = 0;
    for (const auto* p = hero.eventLog.head();
        p != nullptr && (n == 0 || printed < n);
        p = p->next, ++printed) {
        // (*p).next == p->next
        std::cout << " " << std::setw(2) << std::right
            << (printed + 1) << ". " << p->data << "\n";
    }
    std::cout << " (newest first; chain lenght "
        << hero.eventLog.size() << ")\n";
}

}  // namespace dungeon
