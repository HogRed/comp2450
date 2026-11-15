// COMP 2450 — The Descent
// Floor 7 — The Cisterns — starter
//
// This is the post-Floor-6 reference codebase plus this week's Floor-7
// stubs. Everything Floors 0–6 delivered is in place: Bag<T>,
// BagException + at(), findByName<T>, mergeSort, quicksort, std::sort,
// the boss battle, doubly-linked Chain<T> with the full Rule of Three,
// working iterator + const_iterator, std::find_if-based search across
// bestiary, inventory, AND event log, Stack<T> over Chain<T> (push,
// pop, top, size, empty), isBalanced + the `lint` command, the `take`
// and `undo` and `selftest stack` commands.
//
// What is NEW for Floor 7:
//   * hero/Queue.h         — Queue<T>, a FIFO adapter over Chain<T>.
//                            Four one-line bodies you write (enqueue,
//                            dequeue, size, empty); front() ships written.
//                            Mirrors Stack<T>; opposite end-pairing.
//   * hero/Potato.h/.cpp   — std::string hotPotato(names, k) — classical
//                            FIFO elimination algorithm over Queue<string>.
//                            Body is Wednesday's TODO.
//   * hero/QueueTests.*    — `selftest queue` exercises Queue<int>'s
//                            enqueue/dequeue/front/size/empty in five phases.
//   * `provoke <name>`     — enqueues an enemy action onto
//                            hero.enemyActionQueue. Pre-wired here.
//                            Becomes a no-op-then-useful once your
//                            Queue::enqueue actually grows the queue.
//   * `simulate`           — drains hero.enemyActionQueue in FIFO order,
//                            printing each action. Dispatcher body is
//                            Friday's TODO; placeholder works as a no-op
//                            until Friday.
//   * `potato <names...> <k>` — runs hotPotato; prints the survivor.
//                            Starts working once you implement hotPotato.
//   * `selftest queue`     — runs QueueTests.
//
// What you do this week (see the Floor 7 page):
//   Mon — Queue<T>'s four stubbed bodies (front() is provided);
//         `selftest queue` passes.
//   Wed — hotPotato in Potato.cpp; `potato <names...> <k>` works.
//   Fri — the `simulate` dispatcher body in this file; `simulate`
//         drains hero.enemyActionQueue in FIFO order. Also: live-rewrite
//         a Queue<T> using two Stacks. (Demo, no checked-in code.)

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "battle/Battle.h"
#include "bestiary/Bench.h"
#include "bestiary/Bestiary.h"
#include "bestiary/Search.h"
#include "hero/Bench.h"
#include "hero/ChainBench.h"
#include "hero/ChainTests.h"
#include "hero/Hero.h"
#include "hero/IteratorTests.h"
#include "hero/Lint.h"
#include "hero/Potato.h"
#include "hero/QueueTests.h"
#include "hero/Sort.h"
#include "hero/StackTests.h"

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
              << "   take <name>                   — add a small item by that name to your inventory\n"
              << "   sort inventory by <key> [asc|desc]\n"
              << "                                 — key is name, weight, or value\n"
              << "   undo                          — reverse the most recent mutating command\n"
              << "   lint <text>                   — check that brackets/parens/braces in <text> balance\n"
              << "   provoke <name>                — enqueue an enemy action onto the line\n"
              << "   simulate                      — drain the enemy line in FIFO order\n"
              << "   potato <name>... <k>          — classical hot-potato survivor (FIFO elimination)\n"
              << "   log [n]                       — show the last n events, newest first\n"
              << "   log --oldest [n]              — show the first n events, oldest first\n"
              << "   clone hero                    — deep-copy the hero, print both logs, let the copy die\n"
              << "   selftest chain                — Chain<T> leak + deep-copy harness\n"
              << "   selftest iterator             — Chain<T>::iterator harness\n"
              << "   selftest stack                — Stack<T> push/pop/top/LIFO harness\n"
              << "   selftest queue                — Queue<T> enqueue/dequeue/front/FIFO harness\n"
              << "   benchmark [N]                 — race the search algorithms\n"
              << "   benchmark sort [N] [--sorted] [--bad-pivot]\n"
              << "                                 — race the sorting algorithms\n"
              << "   benchmark log [N]             — race Chain::push_front vs vector insert(begin)\n"
              << "   battle warden                 — face the Warden of the Foundations\n"
              << "   help                          — this screen\n"
              << "   quit                          — leave the dungeon)\n";
}

// Snapshot hero.inventory and push it onto hero.undoStack with the given
// description. Call BEFORE mutating the inventory. The push is what makes
// the next `undo` know what to restore. (Floor 6 wiring — unchanged.)
void recordUndo(Hero& hero, const std::string& description) {
    UndoAction a;
    a.description       = description;
    a.inventorySnapshot = hero.inventory;
    hero.undoStack.push(a);
}

}  // anonymous namespace

int main() {
    std::cout << "\n=== THE CISTERNS ===\n\n";

    std::cout << "What is your name, adventurer? ";
    Hero hero;
    std::getline(std::cin, hero.heroName);
    if (hero.heroName.empty()) hero.heroName = "Nameless One";

    std::cout << "\nWelcome, " << hero.heroName << ".\n"
              << "Steward Quill sits behind a low ironwood desk in a soaked grey robe.\n"
              << "He does not look up. \"Take your number. I will see you in order.\"\n"
              << "The line behind you stretches further than you can see.\n\n";

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
                std::cout << "Quill stamps your ticket. The line shuffles forward by one.\n";
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
                if (const Monster* m = findByName<Monster>(bestiary, rest)) {
                    printMonster(*m);
                    std::cout << "  (found in bestiary)\n";
                    hero.eventLog.push_front("search " + rest + " — found in bestiary");
                    continue;
                }
                if (const Item* it = findByName<Item>(hero.inventory, rest)) {
                    std::cout << "  " << it->name
                              << "  (wt " << it->weight
                              << ", val " << it->value << ")\n"
                              << "  (found in inventory)\n";
                    hero.eventLog.push_front("search " + rest + " — found in inventory");
                    continue;
                }
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
            else if (cmd == "take") {
                if (rest.empty()) {
                    std::cout << "Usage: take <name>\n";
                    continue;
                }
                recordUndo(hero, "take " + rest);
                Item it{rest, 0.1, 1};
                hero.inventory.push_back(it);
                std::cout << "  Taken: " << rest << "  (wt 0.1, val 1)\n";
                hero.eventLog.push_front("take " + rest);
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
                recordUndo(hero, "sort inventory by " + criterion);
                if (!sortInventory(hero, criterion)) {
                    hero.undoStack.pop();
                    std::cout << "Unknown sort key. Try: name, weight, or value.\n";
                    continue;
                }
                printInventory(hero);
                hero.eventLog.push_front("sort inventory by " + criterion);
            }
            else if (cmd == "undo") {
                // Floor 6 reference. The order matters: bind the reference
                // BEFORE pop(). Once you pop, the Action you were looking
                // at is gone.
                if (hero.undoStack.empty()) {
                    std::cout << "Nothing to undo.\n";
                } else {
                    const UndoAction& a = hero.undoStack.top();
                    hero.inventory = a.inventorySnapshot;
                    std::cout << "Undid: " << a.description << "\n";
                    hero.eventLog.push_front("undo (" + a.description + ")");
                    hero.undoStack.pop();
                }
            }
            else if (cmd == "lint") {
                if (rest.empty()) {
                    std::cout << "Usage: lint <text>\n";
                    continue;
                }
                const bool ok = isBalanced(rest);
                std::cout << "  " << (ok ? "balanced" : "not balanced") << "\n";
                hero.eventLog.push_front(std::string("lint — ") + (ok ? "balanced" : "not balanced"));
            }
            else if (cmd == "provoke") {
                if (rest.empty()) {
                    std::cout << "Usage: provoke <name>\n";
                    continue;
                }
                // Floor 7 wiring — enqueue a synthesized enemy action onto
                // hero.enemyActionQueue. Until your Queue<T>::enqueue actually
                // grows the queue (Monday), this is a no-op and `simulate`
                // will find the line empty.
                hero.enemyActionQueue.enqueue(rest + " stirs in the dark");
                std::cout << "  " << rest << " takes a number and joins the line.\n";
                hero.eventLog.push_front("provoke " + rest);
            }
            else if (cmd == "simulate") {
				if (hero.enemyActionQueue.empty()) {
					std::cout << "  No enemies are in line. Use provoke <name> first.\n";
				}
				else {
                    std::size_t turn = 1;
					while (!hero.enemyActionQueue.empty()) {
						const std::string& action = hero.enemyActionQueue.front();
						std::cout << "Turn" << turn << ": " << action << "\n";
						hero.eventLog.push_front("simulate — turn " + std::to_string(turn) + ": " + action);
						hero.enemyActionQueue.dequeue();
						turn++;
					}
					std::cout << "  All enemies have acted. The line is empty.\n";
				}
            }
            else if (cmd == "potato") {
                // Parse "<name> <name> ... <k>" — the last token is k, the
                // rest are names. The hotPotato body lives in Potato.cpp
                // and is Wednesday's TODO; until then it returns the
                // placeholder string.
                std::istringstream in(rest);
                std::vector<std::string> tokens;
                std::string tok;
                while (in >> tok) tokens.push_back(tok);
                if (tokens.size() < 2) {
                    std::cout << "Usage: potato <name> <name>... <k>\n";
                    continue;
                }
                std::size_t k = 0;
                try { k = std::stoull(tokens.back()); }
                catch (...) {
                    std::cout << "Last argument must be a positive integer k.\n";
                    continue;
                }
                if (k < 1) {
                    std::cout << "k must be at least 1.\n";
                    continue;
                }
                tokens.pop_back();
                const std::string survivor = hotPotato(tokens, k);
                std::cout << "  Survivor: " << survivor << "\n";
                hero.eventLog.push_front("potato — survivor " + survivor);
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
                if (oldestFirst) {
                    printLog(hero.eventLog.rbegin(), hero.eventLog.rend(),
                             n, "oldest first", hero.eventLog.size());
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
                } else if (rest == "stack") {
                    runStackSelfTest();
                } else if (rest == "queue") {
                    runQueueSelfTest();
                } else {
                    std::cout << "Usage: selftest chain | iterator | stack | queue\n";
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
                std::cout << "Quill does not recognize '" << cmd << "'. Try `help`.\n";
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
