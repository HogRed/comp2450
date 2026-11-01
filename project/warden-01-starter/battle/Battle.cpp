// COMP 2450 — Warden of the Foundations (Midterm 1)
// battle/Battle.cpp — YOU build the body.
//
// =====================================================================
// REQUIRED MECHANICS
// =====================================================================
// Re-read the public midterm page if anything below is unclear.
//
// MENU (at minimum, four options):
//   1. Attack          — damage the warden; warden retaliates that turn
//   2. Use item        — pick from inventory; effect; turn ends
//   3. Inspect Warden  — print warden's state; FREE action — turn does
//                        NOT end
//   4. Flee            — leave; gate stays closed
//
// END:
//   Victory  when wardenHP <= 0
//   Defeat   when playerHP <= 0
//   Fled     when the player chose Flee
//
// =====================================================================
// FLOOR-TIED REQUIREMENTS — ALL FOUR MUST APPEAR IN YOUR CODE
// =====================================================================
//
//   F0 (ADT)         — the available actions on a turn must live in a
//                      container of your choice. ABOVE the declaration,
//                      in a comment, name the ADT and defend it in one
//                      sentence (the same kind of defence Trial I Q1
//                      practiced — but this is a different collection
//                      than Q1's items menu; the right ADT may differ).
//
//   F1 (search)      — the Use-item branch MUST call findByName<Item>
//                      against hero.inventory to look up the item the
//                      player typed.
//
//   F2 (sort)        — when the items menu is displayed, sort the
//                      inventory at display time with a comparator
//                      (std::sort or your Floor 2 sortInventory). Pick
//                      a criterion (value — the healing-power stand-in
//                      — weight, or name) and document it in a comment.
//
//   F3 (templates +  — invalid menu input must `throw BattleException`
//      exceptions)     (ready skeleton in Battle.h) — or BagException
//                      where a genuinely bad index is the fault —
//                      caught INSIDE the battle loop so the player gets
//                      another prompt — not a crash, not an exit.
//
// =====================================================================
// WHAT THE GRADER WILL DO
// =====================================================================
//   1. cmake --build the project. If it does not compile, the Warden
//      has won by default.
//   2. Type `battle warden`, play through to BOTH a victory and a
//      defeat (or attempt to — items + RNG permitting).
//   3. Type a deliberately invalid menu choice (e.g., "9" for a
//      4-option menu). The game must NOT crash; it must re-prompt.
//   4. Open this file and find each of the four Floor ties. They must
//      be REAL — i.e., the menu actually runs through your container,
//      Use-item actually goes through findByName, the items menu is
//      actually sorted, the throw actually fires on bad input.

#include "Battle.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../hero/Bag.h"
#include "../hero/BagException.h"
#include "../hero/Item.h"
#include "../bestiary/Search.h"
#include "../hero/Sort.h"

namespace dungeon {

    namespace {

        const int kPlayerStartHP = 30;
        const int kWardenStartHP = 50;
        const int kPlayerAttackDmg = 6;
        const int kWardenAttackDmg = 4;

        struct BattleAction {
            int id;
            std::string description;
        };

        // =====================================================================
        // FLOOR 0 (ADT Requirement):
        // ADT Choice: Vector / List (Sequence ADT).
        // Defense: A sequential vector guarantees that the available turn actions maintain
        // a fixed, ordered numerical menu for direct indexing, unlike an unordered Set or Bag.
        // =====================================================================
        const std::vector<BattleAction> kBattleActions = {
            {1, "Attack"},
            {2, "Use item"},
            {3, "Inspect Warden"},
            {4, "Flee"}
        };

    }  // anonymous namespace

    BattleOutcome runWardenBattle(Hero& hero) {
        int playerHP = kPlayerStartHP;
        int wardenHP = kWardenStartHP;

        std::cout << "\n=== A WARDEN APPEARS! ===\n";

        while (playerHP > 0 && wardenHP > 0) {
            std::cout << "\n[ Hero HP: " << playerHP << " | Warden HP: " << wardenHP << " ]\n";

            try {
                // Display main menu using the Floor 0 action container
                for (const auto& action : kBattleActions) {
                    std::cout << "  " << action.id << ". " << action.description << "\n";
                }
                std::cout << "Choose action: ";

                std::string input;
                if (!(std::cin >> input)) {
                    return BattleOutcome::Fled;
                }

                int choice = 0;
                try {
                    choice = std::stoi(input);
                }
                catch (...) {
                    // =====================================================================
                    // FLOOR 3 (Exceptions Requirement - Throw):
                    // Throw BattleException on non-numeric or invalid input.
                    // =====================================================================
                    throw BattleException("Invalid choice: must enter a valid option number.");
                }

                if (choice < 1 || choice > static_cast<int>(kBattleActions.size())) {
                    throw BattleException("Choice out of bounds: please select a listed option.");
                }

                if (choice == 1) { // Attack
                    wardenHP -= kPlayerAttackDmg;
                    std::cout << "  You strike the Warden for " << kPlayerAttackDmg << " damage!\n";

                    if (wardenHP > 0) {
                        playerHP -= kWardenAttackDmg;
                        std::cout << "  The Warden retaliates for " << kWardenAttackDmg << " damage!\n";
                    }
                }
                else if (choice == 2) { // Use item
                    if (hero.inventory.empty()) {
                        std::cout << "  Your inventory is empty!\n";
                        continue; // Free action if no items available
                    }

                    // =====================================================================
                    // FLOOR 2 (Sort Requirement):
                    // Sort items at display time descending by item potency (value), 
                    // which represents healing power in this codebase.
                    // =====================================================================
                    std::sort(hero.inventory.begin(), hero.inventory.end(),
                        [](const Item& a, const Item& b) {
                            return a.value > b.value;
                        });

                    std::cout << "  Available Items (Sorted by Potency/Value):\n";
                    for (const auto& item : hero.inventory) {
                        std::cout << "   - " << item.name << " (Potency: " << item.value
                            << ", Weight: " << item.weight << ")\n";
                    }

                    std::cout << "Enter the name of the item to use: ";
                    std::string itemName;
                    std::cin.ignore();
                    std::getline(std::cin, itemName);

                    // =====================================================================
                    // FLOOR 1 (Search Requirement):
                    // Lookup item using findByName<Item> template function against hero.inventory.
                    // =====================================================================
                    const Item* foundItem = findByName<Item>(hero.inventory, itemName);

                    if (!foundItem) {
                        throw BattleException("Item '" + itemName + "' not found in inventory.");
                    }

                    std::cout << "  You used " << foundItem->name << " and restored "
                        << foundItem->value << " HP!\n";
                    playerHP += foundItem->value;

                    // Enemy turn after consumable action
                    playerHP -= kWardenAttackDmg;
                    std::cout << "  The Warden retaliates for " << kWardenAttackDmg << " damage!\n";
                }
                else if (choice == 3) { // Inspect Warden
                    std::cout << "  [INSPECT] Warden HP: " << wardenHP << "/" << kWardenStartHP
                        << " | Base Attack: " << kWardenAttackDmg << "\n";
                    // FREE action — loop continues without advancing turn
                }
                else if (choice == 4) { // Flee
                    std::cout << "  You flee from the Warden!\n";
                    return BattleOutcome::Fled;
                }

            }
            // =====================================================================
            // FLOOR 3 (Exceptions Requirement - Catch):
            // Catch exceptions inside the battle loop to re-prompt without crashing.
            // =====================================================================
            catch (const BattleException& e) {
                std::cout << "  [Error] " << e.what() << " Try again.\n";
                continue;
            }
            catch (const BagException& e) {
                std::cout << "  [Bag Error] " << e.what() << " Try again.\n";
                continue;
            }
            catch (const std::exception& e) {
                std::cout << "  [Error] " << e.what() << " Try again.\n";
                continue;
            }
        }

        if (wardenHP <= 0) {
            std::cout << "\n=== VICTORY! The Warden has been defeated! ===\n";
            return BattleOutcome::Victory;
        }

        std::cout << "\n=== DEFEAT! You were vanquished by the Warden... ===\n";
        return BattleOutcome::Defeat;
    }

}  // namespace dungeon