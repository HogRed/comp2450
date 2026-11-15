// COMP 2450 — The Descent
// Floor 5 — The Eye of Scrying — starter
//
// This is the post-Floor-4½ reference codebase plus this week's Floor-5
// stubs.  Everything Floors 0–4½ delivered is in place: Bag<T>,
// BagException + at(), findByName<T>, mergeSort, quicksort, std::sort,
// the boss battle, doubly-linked Chain<T> with the full Rule of Three,
// clone hero, pop_front, pop_back.
//
// What is NEW for Floor 5:
//   * hero/Chain.h          — nested iterator and const_iterator types,
//                             begin/end/cbegin/cend, plus pre-wired
//                             rbegin/rend (using std::reverse_iterator).
//                             The five operators on each iterator are
//                             TODOs.
//   * hero/Hero.h           — printLog is now a template:
//                               template<It> printLog(first, last, n, label, total).
//                             The Floor 4½ printLog / printLogOldest pair
//                             is gone, replaced by this one template.
//   * hero/IteratorTests.*  — `selftest iterator` exercises range-for,
//                             std::find, std::distance, const-ref walk,
//                             and std::reverse against your iterators.
//   * `search` command      — extended to ALSO look in the event log,
//                             using std::find_if directly on a lambda.
//                             Pre-wired in this file; starts working as
//                             soon as your iterators do.
//
// What you do this week (see the Floor 5 page):
//   Mon — iterator's 5 operators + begin/end.
//   Wed — const_iterator + cbegin/cend; rewrite findByName to std::find_if.
//   Fri — operator-- on both iterators; std::reverse + rbegin/rend work.

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "battle/Battle.h"
#include "bestiary/Bench.h"
#include "bestiary/Bestiary.h"
#include "bestiary/Search.h"
#include "hero/Bench.h"
#include "hero/ChainBench.h"
#include "hero/ChainTests.h"
#include "hero/Hero.h"
#include "hero/IteratorTests.h"
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
              << "   search <name>                 — look up by name in bestiary, inventory, OR event log\n"
              << "   list                          — list the bestiary\n"
              << "   inventory                     — list your inventory\n"
              << "   inspect <n>                   — show the nth item in your inventory\n"
              << "   sort inventory by <key> [asc|desc]\n"
              << "                                 — key is name, weight, or value\n"
              << "   log [n]                       — show the last n events, newest first\n"
              << "   log --oldest [n]              — show the first n events, oldest first\n"
              << "   clone hero                    — deep-copy the hero, print both logs, let the copy die\n"
              << "   selftest chain                — Chain<T> leak + deep-copy harness\n"
              << "   selftest iterator             — Chain<T>::iterator + const_iterator + std::reverse harness\n"
              << "   benchmark [N]                 — race the search algorithms\n"
              << "   benchmark sort [N] [--sorted] [--bad-pivot]\n"
              << "                                 — race the sorting algorithms\n"
              << "   benchmark log [N]             — race Chain::push_front vs vector insert(begin)\n"
              << "   battle warden                 — face the Warden of the Foundations\n"
              << "   help                          — this screen\n"
              << "   quit                          — leave the dungeon)\n";
}

}  // anonymous namespace

int main() {
    std::cout << "\n=== THE EYE OF SCRYING ===\n\n";

    std::cout << "What is your name, adventurer? ";
    Hero hero;
    std::getline(std::cin, hero.heroName);
    if (hero.heroName.empty()) hero.heroName = "Nameless One";

    std::cout << "\nWelcome back, " << hero.heroName << ".\n"
              << "Sister Vael waits at the centre of the round chamber, lens at her sternum.\n"
              << "She will lend you a lens — once you have built one — that walks any container.\n\n";

    Bag<Monster> bestiary = loadBestiary("data/monsters.txt");
    if (bestiary.empty()) {
        std::cerr << "The Bestiary tablet is blank. Cannot proceed.\n";
        return 1;
    }
    sortBestiary(bestiary);

    hero.inventory = loadInventory("data/starter-inventory.txt");
    if (hero.inventory.empty()) {
        std::cerr << "Your satchel is empty. Cannot proceed.\n";
        return 1;
    }

    hero.eventLog.push_front("began session as \"" + hero.heroName + "\"");
    
    printHelp();
    std::cout << "\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        try {
            std::string cmd, rest;
            splitFirst(line, cmd, rest);

            if (cmd == "quit" || cmd == "exit") {
                std::cout << "The lens dims. The lens does not remember what it saw — only how it moved.\n";
                break;
            }
            else if (cmd == "help") {
                printHelp();
            }
            else if (cmd == "list") {
                for (const auto& m : bestiary) printMonster(m);
                hero.eventLog.push_front("list — bestiary printed");
            }
            else if (cmd == "search") {
                if (rest.empty()) {
                    std::cout << "Speak a name:  search <monster, item, or substring of a past event>\n";
                    continue;
                }
                // 1) bestiary — by exact name
                if (const Monster* m = findByName<Monster>(bestiary, rest)) {
                    printMonster(*m);
                    std::cout << "  (found in bestiary)\n";
                    hero.eventLog.push_front("search " + rest + " — found in bestiary");
                    continue;
                }
                // 2) inventory — by exact name
                if (const Item* it = findByName<Item>(hero.inventory, rest)) {
                    std::cout << "  " << it->name
                              << "  (wt " << it->weight
                              << ", val " << it->value << ")\n"
                              << "  (found in inventory)\n";
                    hero.eventLog.push_front("search " + rest + " — found in inventory");
                    continue;
                }
                // 3) event log — by SUBSTRING match. This is the new
                //    Floor-5 branch: one std::find_if call, on YOUR
                //    Chain<std::string>::iterator. Starts working as
                //    soon as your iterators do.
                auto eit = std::find_if(
                    hero.eventLog.begin(), hero.eventLog.end(),
                    [&rest](const std::string& s) { return s.find(rest) != std::string::npos; });
                if (eit != hero.eventLog.end()) {
                    std::cout << "  " << *eit << "\n"
                              << "  (found in event log)\n";
                    hero.eventLog.push_front("search " + rest + " — found in event log");
                    continue;
                }
                std::cout << "No such creature, item, or past event under that name.\n";
                hero.eventLog.push_front("search " + rest + " — not found");
            }
            else if (cmd == "inventory") {
                printInventory(hero);
                hero.eventLog.push_front(
                    "inventory — listed " + std::to_string(hero.inventory.size()) + " items");
            }
            else if (cmd == "inspect") {
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
                const Item& it = hero.inventory.at(n - 1);
                std::cout << "  " << it.name
                          << "  (wt " << it.weight
                          << ", val " << it.value << ")\n";
                hero.eventLog.push_front("inspect " + rest + " — ok");
            }
            else if (cmd == "sort") {
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
                hero.eventLog.push_front("sort inventory by " + criterion);
            }
            else if (cmd == "log") {
                bool oldestFirst = false;
                std::size_t n = 10;
                std::istringstream in(rest);
                std::string tok;
                bool bad = false;
                while (in >> tok) {
                    if (tok == "--oldest") {
                        oldestFirst = true;
                    } else {
                        try { n = std::stoull(tok); }
                        catch (...) { bad = true; break; }
                    }
                }
                if (bad) {
                    std::cout << "Usage: log [--oldest] [n]\n";
                    continue;
                }
                // ONE templated printLog — two call sites, two iterator
                // flavors (forward vs reverse). The function does not
                // know or care which container it's walking.
                if (oldestFirst) {
                    // Oldest-first uses rbegin()/rend(), which depend on
                    // Chain<T>::iterator::operator-- (Floor 5, Friday). On a
                    // non-empty chain, probe it first: --end() must step off
                    // the end. The stubbed operator-- is a no-op, so the probe
                    // never dereferences null; an empty chain needs no probe.
                    bool ready = true;
                    if (!hero.eventLog.empty()) {
                        auto probe = hero.eventLog.end();
                        --probe;
                        ready = (probe != hero.eventLog.end());
                    }
                    if (ready) {
                        printLog(hero.eventLog.rbegin(), hero.eventLog.rend(),
                                 n, "oldest first", hero.eventLog.size());
                    } else {
                        std::cout << "  (oldest-first needs Chain::iterator operator-- — that's Friday's work)\n";
                    }
                } else {
                    printLog(hero.eventLog.begin(),  hero.eventLog.end(),
                             n, "newest first", hero.eventLog.size());
                }
            }
            else if (cmd == "clone") {
                if (rest != "hero") {
                    std::cout << "Usage: clone hero\n";
                    continue;
                }
                std::cout << "  -- original log (newest first) --\n";
                printLog(hero.eventLog.begin(), hero.eventLog.end(),
                         5, "newest first", hero.eventLog.size());
                {
                    Hero copy = hero;
                    std::cout << "  -- cloned log (newest first) --\n";
                    printLog(copy.eventLog.begin(), copy.eventLog.end(),
                             5, "newest first", copy.eventLog.size());
                    std::cout << "  (clone is being destroyed now)\n";
                }
                std::cout << "  (clone destroyed; original event log still has "
                          << hero.eventLog.size() << " entries — try `log 3`)\n";
                hero.eventLog.push_front("clone hero — copy lived and died");
            }
            else if (cmd == "selftest") {
                if (rest == "chain") {
                    runChainSelfTest();
                } else if (rest == "iterator") {
                    runIteratorSelfTest();
                } else {
                    std::cout << "Usage: selftest chain   |   selftest iterator\n";
                }
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
                else if (first == "log") {
                    std::size_t n = 0;
                    std::string tok;
                    if (in >> tok) {
                        try { n = std::stoull(tok); }
                        catch (...) {
                            std::cout << "Usage: benchmark log [N]\n";
                            continue;
                        }
                    }
                    if (n == 0) runChainBenchmarkSweep();
                    else        runChainBenchmark(n);
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
            else if (cmd == "battle") {
                if (rest != "warden") {
                    std::cout << "Usage: battle warden\n";
                    continue;
                }
                BattleOutcome outcome = runWardenBattle(hero);
                switch (outcome) {
                    case BattleOutcome::Victory:
                        std::cout << "\nThe Warden falls. The gate opens.\n"; break;
                    case BattleOutcome::Defeat:
                        std::cout << "\nYou fall. The gate stays shut.\n"; break;
                    case BattleOutcome::Fled:
                        std::cout << "\nYou step back from the gate. It does not chase.\n"; break;
                }
            }
            else {
                std::cout << "The lens does not recognize '" << cmd << "'.\n";
            }
        }
        catch (const BagException& e) {
            std::cout << "No such item. (" << e.what() << ")\n";
            hero.eventLog.push_front(std::string("error: ") + e.what());
        }
        catch (const std::exception& e) {
            std::cout << "Something went wrong: " << e.what() << "\n";
            hero.eventLog.push_front(std::string("error: ") + e.what());
        }
    }

    return 0;
}
