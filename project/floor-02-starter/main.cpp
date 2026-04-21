// COMP 2450 — The Descent
// Floor 2: The Sorting Crucible
//
// You descend from the Scrying Well into a forge-lit chamber — the
// Crucible — where your loot from Floor 1 is heaped, unsorted. This
// week you add sorting. The Bestiary is still here; the search command
// you wrote (or the reference version we ship) is still here.
//
// Read this file. You will not edit it. Its job is the same as Floor 1's
// main.cpp — parse commands, dispatch to functions in other .cpp files.
// Notice the pattern: we keep adding commands, but the CORE of this
// file (the REPL loop) looks identical to Floor 1. That is the payoff
// of the seam discipline. New features arrive as new dispatch branches;
// old features don't break.

#include <iostream>
#include <sstream>
#include <string>

#include "bestiary/Bench.h"
#include "bestiary/Bestiary.h"
#include "bestiary/Search.h"
#include "hero/Bench.h"
#include "hero/Hero.h"
#include "hero/Sort.h"

using namespace dungeon;

namespace {

// Split "sort inventory by name desc" into ("sort", "inventory by name desc").
// We do argument parsing rough here; the commands below re-split as needed.
void splitFirst(const std::string& line, std::string& cmd, std::string& rest) {
    auto sp = line.find(' ');
    if (sp == std::string::npos) { cmd = line; rest.clear(); }
    else { cmd = line.substr(0, sp); rest = line.substr(sp + 1); }
}

void printHelp() {
    std::cout << "(commands:\n"
              << "   search <name>                 — look up a monster in the bestiary\n"
              << "   list                          — list the bestiary\n"
              << "   inventory                     — list your inventory\n"
              << "   sort inventory by <key> [asc|desc]\n"
              << "                                 — key is name, weight, or value\n"
              << "   benchmark [N]                 — race the search algorithms\n"
              << "   benchmark sort [N] [--sorted] [--bad-pivot]\n"
              << "                                 — race the sorting algorithms\n"
              << "   help                          — this screen\n"
              << "   quit                          — leave the dungeon)\n";
}

}  // anonymous namespace

int main() {
    std::cout << "\n=== THE SORTING CRUCIBLE ===\n\n";

    std::cout << "What is your name, adventurer? ";
    Hero hero;
    std::getline(std::cin, hero.heroName);
    if (hero.heroName.empty()) hero.heroName = "Nameless One";

    std::cout << "\nWelcome back, " << hero.heroName << ".\n"
              << "Your trophies from Floor 1 are heaped on the floor, unsorted.\n"
              << "The forge at the center of the Crucible burns hot.\n\n";

    // Floor 1 state — the bestiary, sorted (so binarySearch's precondition holds).
    auto bestiary = loadBestiary("data/monsters.txt");
    if (bestiary.empty()) {
        std::cerr << "The Bestiary tablet is blank. Cannot proceed.\n";
        return 1;
    }
    sortBestiary(bestiary);

    // Floor 2 state — the hero's inventory, intentionally unsorted.
    hero.inventory = loadInventory("data/starter-inventory.txt");
    if (hero.inventory.empty()) {
        std::cerr << "Your satchel is empty. Cannot proceed.\n";
        return 1;
    }

    printHelp();
    std::cout << "\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        std::string cmd, rest;
        splitFirst(line, cmd, rest);

        if (cmd == "quit" || cmd == "exit") {
            std::cout << "The forge dies down to embers.\n";
            break;
        }
        else if (cmd == "help") {
            printHelp();
        }
        else if (cmd == "list") {
            for (const auto& m : bestiary) printMonster(m);
        }
        else if (cmd == "search") {
            if (rest.empty()) {
                std::cout << "Speak a name:  search <monster>\n";
                continue;
            }
            const Monster* m = findMonster(bestiary, rest);
            if (m) printMonster(*m);
            else   std::cout << "No such creature stalks this Keep.\n";
        }
        else if (cmd == "inventory") {
            printInventory(hero);
        }
        else if (cmd == "sort") {
            // Expect: "inventory by <key> [asc|desc]"
            // Re-parse the rest of the line with an istringstream so we
            // can extract tokens by whitespace — simpler than substring
            // arithmetic. `in >> tok` returns false (sets failbit) when
            // the stream runs out, which naturally leaves `dir` empty
            // for the two-word form "sort inventory by weight".
            std::istringstream in(rest);
            std::string what, by, key, dir;
            in >> what >> by >> key >> dir;
            if (what != "inventory" || by != "by" || key.empty()) {
                std::cout << "Usage: sort inventory by <name|weight|value> [asc|desc]\n";
                continue;
            }
            // Glue key and direction back into one string so sortInventory
            // (which is YOUR code) only has to parse one argument shape.
            // This file stays stable; the parsing details belong to Sort.cpp.
            std::string criterion = key;
            if (!dir.empty()) criterion += " " + dir;
            if (!sortInventory(hero, criterion)) {
                std::cout << "Unknown sort key. Try: name, weight, or value.\n";
                continue;
            }
            printInventory(hero);
        }
        else if (cmd == "benchmark") {
            // Two forms:
            //   benchmark [N]               — Floor 1 search benchmark
            //   benchmark sort [N] [flags]  — Floor 2 sort benchmark
            //
            // We dispatch on whether the first word is literally "sort".
            // The sort-flag parser accepts flags in any order (N before
            // or after --sorted) because students WILL type them in both
            // orders, and correcting them is friction we don't need.
            std::istringstream in(rest);
            std::string first;
            in >> first;
            if (first == "sort") {
                SortBenchOptions opts;
                std::size_t n = 0;  // 0 = "no N given" → run the full sweep
                std::string tok;
                while (in >> tok) {
                    if      (tok == "--sorted")    opts.presorted = true;
                    else if (tok == "--bad-pivot") opts.badPivot  = true;
                    else {
                        // Not a flag and not a number? That's user error.
                        // Sentinel value (size_t max) signals "abort" to
                        // the branch below; we can't just `break` because
                        // we still need to skip the run.
                        try { n = std::stoull(tok); }
                        catch (...) {
                            std::cout << "Usage: benchmark sort [N] [--sorted] [--bad-pivot]\n";
                            n = static_cast<std::size_t>(-1);
                            break;
                        }
                    }
                }
                if (n == static_cast<std::size_t>(-1)) continue;
                if (n == 0) runSortBenchmarkSweep(opts);
                else        runSortBenchmark(n, opts);
            }
            else if (first.empty()) {
                runBenchmarkSweep();
            }
            else {
                try {
                    runBenchmark(std::stoull(first));
                } catch (const std::exception&) {
                    std::cout << "Usage: benchmark [N]   or   benchmark sort [N] [flags]\n";
                }
            }
        }
        else {
            std::cout << "The Crucible does not understand '" << cmd << "'.\n";
        }
    }

    return 0;
}
