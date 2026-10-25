// COMP 2450 — Floor 4 starter (post-Warden reference state)
// battle/Battle.cpp — REFERENCE implementation of runWardenBattle.
//
// This is the post-midterm baseline. If you wrote your own Warden boss
// battle for Trial III, drop YOUR battle/Battle.cpp over this file and
// keep working on Floor 4. The reference below is intentionally compact;
// your own version is almost certainly more polished. The point of
// shipping it here is that Floor 4 builds and runs even for students
// whose midterm submission was incomplete.
//
// Floor 4 thread:
//   The battle loop now also writes one event into hero.eventLog every
//   turn — "warden HP X / Y", "you struck for D damage", "the warden
//   struck for D", and the final outcome. By Friday, when push_front
//   actually does something, `log 10` after a battle will replay the
//   fight backward.
//
// Floor 0–3 ties (still required for the Warden's grade):
//   F0 — actions live in a Bag<MenuAction>.
//   F1 — Use-item branch calls findByName<Item> on hero.inventory.
//   F2 — items menu std::sort's the inventory by value-descending first.
//   F3 — bad input throws BagException, caught inside the loop.

#include "Battle.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "../bestiary/Search.h"
#include "../hero/Bag.h"
#include "../hero/BagException.h"
#include "../hero/Item.h"
#include "../hero/Sort.h"

namespace dungeon {

    namespace {

        constexpr int kPlayerStartHP = 30;
        constexpr int kWardenStartHP = 50;
        constexpr int kPlayerAttackDmg = 6;
        constexpr int kWardenAttackDmg = 4;

        // F0 — abstract data type for the available turn actions. Bag<MenuAction>
        // is the Floor-3 container template; one stamping for an enum-payload
        // list is all the action menu needs. Defended in one sentence: the menu
        // is a small, ordered, indexed list with no insertions mid-list, which
        // is exactly what Bag<T> (vector-backed) is good for.
        enum class MenuAction { Attack, UseItem, Inspect, Flee };

        struct MenuOption {
            int         number;
            std::string label;
            MenuAction  action;
        };

        void printMenu(const Bag<MenuOption>& menu, int playerHP, int wardenHP) {
            std::cout << "\n  -- Your turn --   you HP " << playerHP
                << "    Warden HP " << wardenHP << "\n";
            for (std::size_t i = 0; i < menu.size(); ++i) {
                std::cout << "    " << menu[i].number << ". " << menu[i].label << "\n";
            }
            std::cout << "  > ";
        }

        MenuAction readMenuChoice(const Bag<MenuOption>& menu) {
            std::string line;
            if (!std::getline(std::cin, line)) {
                // EOF: treat as flee so we don't loop on closed stdin.
                return MenuAction::Flee;
            }
            int n = -1;
            try { n = std::stoi(line); }
            catch (...) {
                // F3 — non-numeric input throws into the battle loop's catch.
                throw BagException(
                    static_cast<std::size_t>(menu.size() + 1),
                    menu.size());
            }
            for (std::size_t i = 0; i < menu.size(); ++i) {
                if (menu[i].number == n) return menu[i].action;
            }
            // F3 — out-of-range throws too.
            throw BagException(static_cast<std::size_t>(n), menu.size());
        }

        // Use-item handler. Sorts the inventory by value descending (F2),
        // shows it, asks for a name, looks it up via findByName<Item> (F1),
        // applies the effect. A "Healing potion" heals 12 HP and is consumed
        // in spirit (we don't actually remove the item this floor — that is
        // Floor 4½'s remove-mid-list lesson).
        void useItem(Hero& hero, int& playerHP) {
            if (hero.inventory.empty()) {
                std::cout << "  Your satchel is empty.\n";
                return;
            }

            // F2 — sort the menu by value descending so the most valuable item
            // shows first. Sort routine reused from Floor 2; comparator is a
            // lambda.
            sortInventory(hero, "value desc");
            std::cout << "  Choose an item by name:\n";
            printInventory(hero);
            std::cout << "  > ";

            std::string name;
            if (!std::getline(std::cin, name) || name.empty()) {
                std::cout << "  You hesitate.\n";
                return;
            }

            // F1 — findByName<Item> lookup.
            const Item* it = findByName<Item>(hero.inventory, name);
            if (!it) {
                // F3 — no such item: throw, caught by the battle-loop catch.
                throw BagException(0, hero.inventory.size());
            }

            if (it->name.find("otion") != std::string::npos) {
                playerHP = std::min(playerHP + 12, kPlayerStartHP);
                std::cout << "  You drink " << it->name << ". HP -> " << playerHP << ".\n";
            }
            else {
                std::cout << "  You ready " << it->name << " — but it is not a consumable.\n";
            }
        }

    }  // anonymous namespace

    BattleOutcome runWardenBattle(Hero& hero) {
        int playerHP = kPlayerStartHP;
        int wardenHP = kWardenStartHP;

        // F0 — turn actions in a Bag<MenuOption>.
        Bag<MenuOption> menu;
        menu.push_back({ 1, "Attack",         MenuAction::Attack });
        menu.push_back({ 2, "Use item",       MenuAction::UseItem });
        menu.push_back({ 3, "Inspect Warden", MenuAction::Inspect });
        menu.push_back({ 4, "Flee",           MenuAction::Flee });

        hero.eventLog.push_front("battle warden — engaged");

        while (playerHP > 0 && wardenHP > 0) {
            // F3 — anything thrown by readMenuChoice or useItem lands here,
            // we re-prompt without ending the turn.
            try {
                printMenu(menu, playerHP, wardenHP);
                switch (readMenuChoice(menu)) {
                case MenuAction::Attack: {
                    wardenHP -= kPlayerAttackDmg;
                    std::cout << "  You strike for " << kPlayerAttackDmg
                        << ".  Warden HP -> " << std::max(wardenHP, 0) << ".\n";
                    hero.eventLog.push_front(
                        "you struck for " + std::to_string(kPlayerAttackDmg));
                    if (wardenHP > 0) {
                        playerHP -= kWardenAttackDmg;
                        std::cout << "  The Warden retaliates for "
                            << kWardenAttackDmg
                            << ".  Your HP -> " << std::max(playerHP, 0) << ".\n";
                        hero.eventLog.push_front(
                            "the Warden struck for " + std::to_string(kWardenAttackDmg));
                    }
                    break;
                }
                case MenuAction::UseItem: {
                    useItem(hero, playerHP);
                    // Using an item ends the turn — the warden retaliates.
                    if (wardenHP > 0 && playerHP > 0) {
                        playerHP -= kWardenAttackDmg;
                        std::cout << "  The Warden strikes while you fumble.  Your HP -> "
                            << std::max(playerHP, 0) << ".\n";
                        hero.eventLog.push_front(
                            "the Warden struck for " + std::to_string(kWardenAttackDmg));
                    }
                    break;
                }
                case MenuAction::Inspect: {
                    std::cout << "  Warden of the Foundations.  HP " << wardenHP
                        << " / " << kWardenStartHP
                        << ".  No visible weakness.  (free action)\n";
                    // FREE action — turn does not end.
                    break;
                }
                case MenuAction::Flee: {
                    hero.eventLog.push_front("battle warden — fled");
                    return BattleOutcome::Fled;
                }
                }
            }
            catch (const BagException& e) {
                std::cout << "  " << e.what() << " — try again.\n";
                // Turn does NOT advance.
            }
        }

        if (wardenHP <= 0) {
            hero.eventLog.push_front("battle warden — outcome: Victory");
            return BattleOutcome::Victory;
        }
        hero.eventLog.push_front("battle warden — outcome: Defeat");
        return BattleOutcome::Defeat;
    }

}  // namespace dungeon