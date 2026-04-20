// COMP 2450 — The Descent
// Floor 1: The Scrying Well
//
// You descend from the Antechamber into a circular stone chamber. A black
// well sits in the center. Beside it: the Bestiary tablet.
//
// The CLI is wired for you. The thing the game CANNOT do until you finish
// your work this week is look anything up. That is `bestiary/Search.cpp`.

#include <iostream>
#include <string>

#include "bestiary/Bestiary.h"
#include "bestiary/Search.h"

using namespace dungeon;

int main() {
    std::cout << "\n=== THE SCRYING WELL ===\n\n";

    std::cout << "What is your name, adventurer? ";
    std::string hero_name;
    std::getline(std::cin, hero_name);
    if (hero_name.empty()) hero_name = "Nameless One";

    std::cout << "\nWelcome back, " << hero_name << ".\n";
    std::cout << "The water is black. It shows what you name aloud.\n";
    std::cout << "(commands: search <name>, list, quit)\n\n";

    auto bestiary = loadBestiary("data/monsters.txt");
    if (bestiary.empty()) {
        std::cerr << "The Bestiary tablet is blank. Cannot proceed.\n";
        return 1;
    }
    sortBestiary(bestiary);  // upholds binarySearch's precondition

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        auto sp = line.find(' ');
        std::string cmd = (sp == std::string::npos) ? line : line.substr(0, sp);
        std::string arg = (sp == std::string::npos) ? ""   : line.substr(sp + 1);

        if (cmd == "quit" || cmd == "exit") {
            std::cout << "The Well goes still.\n";
            break;
        }
        else if (cmd == "list") {
            for (const auto& m : bestiary) printMonster(m);
        }
        else if (cmd == "search") {
            if (arg.empty()) {
                std::cout << "Speak a name:  search <monster>\n";
                continue;
            }
            const Monster* m = findMonster(bestiary, arg);
            if (m) printMonster(*m);
            else   std::cout << "No such creature stalks this Keep.\n";
        }
        else {
            std::cout << "The Well does not understand '" << cmd << "'.\n";
        }
    }

    return 0;
}
