// COMP 2450 — The Descent
// Floor 1: The Scrying Well
//
// You descend from the Antechamber into a circular stone chamber. A black
// well sits in the center. Beside it: the Bestiary tablet.
//
// Read this file. You will not edit it. Its job is to parse commands the
// player types and dispatch to the functions YOU wrote in Search.cpp.
// This is the "seam" pattern: every CLI command ultimately calls one of
// your four functions through Search.h. Nothing else knows about them.
// That decoupling is worth more than the search algorithm itself.

#include <iostream>
#include <string>

#include "bestiary/Bench.h"
#include "bestiary/Bestiary.h"
#include "bestiary/Search.h"

// `using namespace` is usually a sin in headers, but in a top-level .cpp
// that no one else #includes, it's fine — it saves writing `dungeon::`
// in front of every type name below.
using namespace dungeon;

int main() {
    std::cout << "\n=== THE SCRYING WELL ===\n\n";

    // Prompt for the hero name. getline, not `cin >> name`, so that
    // multi-word names ("Sir Reginald") aren't truncated at the first space.
    std::cout << "What is your name, adventurer? ";
    std::string hero_name;
    std::getline(std::cin, hero_name);
    if (hero_name.empty()) hero_name = "Nameless One";

    std::cout << "\nWelcome back, " << hero_name << ".\n";
    std::cout << "The water is black. It shows what you name aloud.\n";
    std::cout << "(commands: search <name>, list, benchmark [N], quit)\n\n";

    // Load the bestiary from disk. If CMake's post-build copy worked, this
    // file lives next to the binary. If loadBestiary fails (file missing,
    // permissions), we bail rather than run with an empty Bestiary.
    auto bestiary = loadBestiary("data/monsters.txt");
    if (bestiary.empty()) {
        std::cerr << "The Bestiary tablet is blank. Cannot proceed.\n";
        return 1;
    }

    // Uphold binarySearch's precondition BEFORE the player can run any
    // search. This is the Unsorted Lich's banishment. If you ever doubt
    // whether binary search requires sorted input, comment this line out
    // and try `search Wraith`. (Then restore it.)
    sortBestiary(bestiary);

    std::string line;
    while (true) {
        std::cout << "> ";

        // getline returns a stream in a false state on EOF (Ctrl-D / Ctrl-Z)
        // or on pipe closure — the natural way to end a REPL.
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        // Split the line on the FIRST space: everything before is the
        // command, everything after is the argument string. Simple but
        // adequate — Floor 1 commands only ever have one argument.
        auto sp = line.find(' ');
        std::string cmd = (sp == std::string::npos) ? line : line.substr(0, sp);
        std::string arg = (sp == std::string::npos) ? ""   : line.substr(sp + 1);

        if (cmd == "quit" || cmd == "exit") {
            std::cout << "The Well goes still.\n";
            break;
        }
        else if (cmd == "list") {
            // `const auto&` iterates without copying each Monster. Notice
            // this is the same pattern you'd use in linearSearch.
            for (const auto& m : bestiary) printMonster(m);
        }
        else if (cmd == "search") {
            if (arg.empty()) {
                std::cout << "Speak a name:  search <monster>\n";
                continue;
            }
            // THE seam. findMonster — written by you in Search.cpp — is
            // the only thing main.cpp knows about lookup. Swap it between
            // linear/binary/recursive and this file never notices.
            const Monster* m = findMonster(bestiary, arg);
            if (m) printMonster(*m);
            else   std::cout << "No such creature stalks this Keep.\n";
        }
        else if (cmd == "benchmark") {
            if (arg.empty()) {
                runBenchmarkSweep();
            } else {
                // std::stoull throws on non-numeric input. We catch, print
                // a usage message, and keep the REPL alive. (Crashing on a
                // typo in a REPL is a bad player experience.)
                try {
                    runBenchmark(std::stoull(arg));
                } catch (const std::exception&) {
                    std::cout << "Usage: benchmark [N]   where N is a positive integer\n";
                }
            }
        }
        else {
            std::cout << "The Well does not understand '" << cmd << "'.\n";
        }
    }

    return 0;
}
