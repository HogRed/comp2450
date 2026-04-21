// COMP 2450 — The Descent
// Floor 3: The Forgemaster's Vault
//
// You descend from the Sorting Crucible into a vaulted stone chamber —
// the Forgemaster's Vault — where empty molds stand on cold racks.
// Every command you unlocked on Floors 1 and 2 still works: search, list,
// inventory, sort, benchmark. This week you add THREE things:
//
//   (Mon) a FUNCTION template — findByName<T> — that replaces
//         Floor 1's monster-specific linearSearch with one source body
//         that works on Items too.
//   (Wed) a CLASS template — Bag<T> — that replaces std::vector as the
//         storage behind BOTH hero.inventory AND the keep's bestiary.
//         One class, two instantiations.
//   (Fri) USER-DEFINED EXCEPTIONS — BagException, a safer at() method
//         on Bag, and a try/catch around this main loop so the game
//         keeps running when a caller asks for an index that isn't
//         there.
//
// Read this file. On MON and WED you do not need to edit it — your
// work lives in Search.h and Bag.h. On FRIDAY you add one small piece
// here: a try/catch around the command dispatch. There is a TODO marker
// where it goes.

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

void splitFirst(const std::string& line, std::string& cmd, std::string& rest) {
    auto sp = line.find(' ');
    if (sp == std::string::npos) { cmd = line; rest.clear(); }
    else { cmd = line.substr(0, sp); rest = line.substr(sp + 1); }
}

void printHelp() {
    std::cout << "(commands:\n"
              << "   search <name>                 — look up by name in bestiary or inventory\n"
              << "   list                          — list the bestiary\n"
              << "   inventory                     — list your inventory\n"
              << "   inspect <n>                   — show the nth item in your inventory\n"
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
    std::cout << "\n=== THE FORGEMASTER'S VAULT ===\n\n";

    std::cout << "What is your name, adventurer? ";
    Hero hero;
    std::getline(std::cin, hero.heroName);
    if (hero.heroName.empty()) hero.heroName = "Nameless One";

    std::cout << "\nWelcome back, " << hero.heroName << ".\n"
              << "Empty molds line the walls. The Forgemaster watches, silent.\n"
              << "This week you press one mold to hold every list the keep remembers.\n\n";

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

    // TODO Floor 3 (Fri): wrap the body of the while-loop below in a
    // try/catch. Catch `const BagException&` (or `const std::exception&`
    // — BagException IS-A std::exception, so either works) and print
    // "No such item. (" + e.what() + ")" followed by a newline. The
    // catch body ends, the loop iterates, the prompt reappears. The
    // game keeps going.
    //
    // Without this catch block, a bad `inspect 99` on Friday will throw
    // past main(), std::terminate runs, and the game dies. Try it first
    // with no catch (watch what the runtime prints to stderr), then add
    // the catch and try again. That A/B IS the exceptions lesson.
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        std::string cmd, rest;
        splitFirst(line, cmd, rest);

        if (cmd == "quit" || cmd == "exit") {
            std::cout << "The forge cools. The molds stand empty again.\n";
            break;
        }
        else if (cmd == "help") {
            printHelp();
        }
        else if (cmd == "list") {
            for (const auto& m : bestiary) printMonster(m);
        }
        else if (cmd == "search") {
            // Floor 3 (Mon): this is where your function template earns
            // its keep. Replace the monster-only findMonster call below
            // with calls to findByName<T> against BOTH the bestiary and
            // hero.inventory — same source template, two instantiations.
            // Print the first match; fall through to the "no such" line
            // if nothing matches in either.
            if (rest.empty()) {
                std::cout << "Speak a name:  search <monster-or-item>\n";
                continue;
            }
            // TODO Floor 3 (Mon): wire this to findByName<T>. For now
            // it still calls Floor 1's monster-only findMonster.
            const Monster* m = findMonster(bestiary, rest);
            if (m) { printMonster(*m); continue; }
            std::cout << "No such creature stalks this Keep.\n";
        }
        else if (cmd == "inventory") {
            printInventory(hero);
        }
        else if (cmd == "inspect") {
            // Floor 3 (Fri): this command exists to stress-test your
            // exception story. RIGHT NOW, if the user asks for an index
            // that isn't there, the marked line below is undefined
            // behavior — maybe garbage, maybe a crash, maybe "works"
            // by accident. On Friday you change `[` to `.at(` and that
            // line will THROW a BagException, which the try/catch you
            // added around the loop will catch.
            if (rest.empty()) {
                std::cout << "Usage: inspect <n>\n";
                continue;
            }
            std::size_t n;
            try { n = std::stoull(rest); }
            catch (...) {
                std::cout << "Usage: inspect <n>\n";
                continue;
            }
            if (n == 0) {
                std::cout << "Items are numbered from 1, adventurer.\n";
                continue;
            }
            // Users type 1-based indices; internally we are 0-based.
            //
            // TODO Floor 3 (Fri): change `hero.inventory[n - 1]` below
            // to `hero.inventory.at(n - 1)`. With the try/catch above,
            // the bad input becomes a clean error message instead of
            // a crash.
            const Item& it = hero.inventory[n - 1];
            std::cout << "  " << it.name
                      << "  (wt " << it.weight
                      << ", val " << it.value << ")\n";
        }
        else if (cmd == "sort") {
            // Parsing logic unchanged from Floor 2; see comments there.
            std::istringstream in(rest);
            std::string what, by, key, dir;
            in >> what >> by >> key >> dir;
            if (what != "inventory" || by != "by" || key.empty()) {
                std::cout << "Usage: sort inventory by <name|weight|value> [asc|desc]\n";
                continue;
            }
            std::string criterion = key;
            if (!dir.empty()) criterion += " " + dir;
            if (!sortInventory(hero, criterion)) {
                std::cout << "Unknown sort key. Try: name, weight, or value.\n";
                continue;
            }
            printInventory(hero);
        }
        else if (cmd == "benchmark") {
            std::istringstream in(rest);
            std::string first;
            in >> first;
            if (first == "sort") {
                SortBenchOptions opts;
                std::size_t n = 0;
                std::string tok;
                while (in >> tok) {
                    if      (tok == "--sorted")    opts.presorted = true;
                    else if (tok == "--bad-pivot") opts.badPivot  = true;
                    else {
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
            std::cout << "The Vault does not understand '" << cmd << "'.\n";
        }
    }

    return 0;
}
