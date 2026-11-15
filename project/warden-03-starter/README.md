# Warden of the Trees — Midterm 3 starter

A **self-contained** starter for *Midterm 3 — The Warden of the Trees*. Drop the contents
into a fresh folder and the entire dungeon app builds, with everything the eleven floors
above were supposed to deliver already in place and **working**: `Bag<T>` + `BagException`,
the sorts, doubly-linked `Chain<T>` with iterators, `Stack<T>` and `Queue<T>`, the general
`Tree<T>` (`taxonomy`/`lineage`), the full Floor 9 `BST<T>` + Floor 9½ balancing
(`catalogue`/`recall`/`balance`), the Floor 10 `Registry` (`std::set`/`std::map`), the
Floor 11 `HashTable` (`whisper`/`banish`/`load`), every `selftest`, and Warden 1's
`runWardenBattle`. This is the **post-Floor-11** reference state.

**How you complete this Warden:** watch the Warden III videos and code along, make sure it
builds and runs, then push to GitHub and submit your repo link on Canvas. That's all.
[The public midterm page](https://hogred.github.io/comp2450/floors/warden-03/) describes
what the videos build; the due date is on Canvas.

## Two paths to start

**This starter (what the videos use).** Sync your fork so you have the latest version, then
open this folder, `project/warden-03-starter/`, build it, and code along. Everything
compiles and runs already, and `battle trees` is already wired up. The only file you edit is
`battle/Battle.cpp`.

**Your own Floor 11 project (optional).** If you'd rather build the Warden into your own
`floor-11/` project (it has to build, and `selftest whispers` has to pass), make the three
small wiring changes listed below first, then follow the videos from where they start
writing `battle/Battle.cpp`.

## What's in the box

```
warden-03-starter/
  main.cpp                        — full post-Floor-11 main, with both the
                                    `battle warden` and `battle trees` commands
                                    already wired. runTreeWarden is the stub.
  CMakeLists.txt                  — already includes battle/Battle.cpp.
  battle/
    Battle.h                      — public API for BOTH wardens + BattleException.
    Battle.cpp                    — runWardenBattle is FILLED (Warden 1
                                    reference). runTreeWarden is the stub the
                                    videos fill in.
  bestiary/                       — Monster, Bestiary, Search, Bench, and the
                                    FINISHED Registry (std::set/std::map).
  hero/                           — Bag, Chain (with eraseFirst), Stack, Queue,
                                    Tree, BST + balance, the FINISHED HashTable,
                                    Hero (printLog), plus every *Tests / *Bench.
  data/
    monsters.txt                  — the 15-monster bestiary.
    taxonomy.txt                  — McCown's classification tree (Goblin absent).
    starter-inventory.txt         — starting inventory.
```

(The `warden-3-trials/` folder is left over from an older version of this midterm. You
don't need it.)

## Build & run

```
cmake -B build            # configure once (and any time CMakeLists.txt changes)
cmake --build build       # compile
build\the_descent.exe     # run (Windows cmd.exe)
./build/the_descent       # run (macOS / Linux)
```

You should land at the `> ` prompt with every previous-floor command working. Try
`battle warden` — Midterm 1's reference encounter plays through. Try `battle trees` — you'll
get the placeholder message from the unwritten `runTreeWarden` stub.

## What the videos build

`runTreeWarden` in [`battle/Battle.cpp`](battle/Battle.cpp): a boss fight that uses one
thing you built from each of Floors 8–11:

- **[F8]** `taxonomy.levelOrder()` lines up the masks; `taxonomy.lineage()` unmasks the
  Warden.
- **[F9 + F9½]** `catalogue.find(probe)` is the slow Recall; `isBalanced()`/`height()`/
  `balanceFactor()` decide the extra cost and print the readout.
- **[F10]** `registry.hasWeakness(w)` + `countWeakness(w)` + `weaknesses()` drive Ward.
- **[F11]** `whispers.find(name)` is the free identify; a **local copy** has one name
  erased so one beast is forced onto the Recall path. The shared table stays untouched.

The only contract `main.cpp` depends on is the public API in `Battle.h`:

```cpp
BattleOutcome runTreeWarden(Hero& hero,
                            const Tree<std::string>& taxonomy,
                            const BST<Monster>&      catalogue,
                            const Registry&          registry,
                            const HashTable&         whispers);
```

### Only if you're working in your own Floor 11 project

Skip this section if you're working in this starter: it's already done here. In your own
project, make these changes before you start the videos. **No `CMakeLists.txt` change** is
needed — `battle/Battle.cpp` is already in your build.

1. **`battle/Battle.h`** — add `BattleException` and the `runTreeWarden` declaration, plus
   the includes the new signature needs (`../hero/Tree.h`, `../hero/BST.h`,
   `../hero/HashTable.h`, `../bestiary/Registry.h`, `../bestiary/Monster.h`, `<exception>`,
   `<string>`):

   ```cpp
   class BattleException : public std::exception {
   public:
       explicit BattleException(const std::string& message) : msg_(message) {}
       const char* what() const noexcept override { return msg_.c_str(); }
   private:
       std::string msg_;
   };

   BattleOutcome runTreeWarden(Hero& hero,
                               const Tree<std::string>& taxonomy,
                               const BST<Monster>&      catalogue,
                               const Registry&          registry,
                               const HashTable&         whispers);
   ```

2. **`main.cpp`** — extend the existing `battle` command to also handle `trees`. The four
   indexes (`taxonomy`, `bestiaryTree`, `registry`, `whispers`) are already locals in
   `main()`; pass them straight through:

   ```cpp
   else if (cmd == "battle") {
       std::istringstream bs(rest);
       std::string which;
       bs >> which;
       if (which == "warden") {
           BattleOutcome outcome = runWardenBattle(hero);
           switch (outcome) { /* ...existing Warden 1 banners... */ }
       }
       else if (which == "trees") {
           BattleOutcome outcome =
               runTreeWarden(hero, taxonomy, bestiaryTree, registry, whispers);
           switch (outcome) {
               case BattleOutcome::Victory:
                   std::cout << "\nThe menagerie is unmasked. The Warden falls. The Archive stair opens.\n"; break;
               case BattleOutcome::Defeat:
                   std::cout << "\nThe Warden files you among its faces. The stair stays shut.\n"; break;
               case BattleOutcome::Fled:
                   std::cout << "\nYou step back from the Warden. Its menagerie still stands.\n"; break;
           }
       }
       else {
           std::cout << "Usage: battle warden   |   battle trees\n";
       }
   }
   ```

   And one line in `printHelp()`:

   ```cpp
                 << "   battle trees                  — face the Warden of the Trees (Midterm 3)\n"
   ```

3. **`battle/Battle.cpp`** — write `runTreeWarden` along with the videos.

## Submission

1. **Push to GitHub.** Commit the code you built along with the videos
   (`battle/Battle.cpp`; plus `main.cpp` and `battle/Battle.h` if you worked in your own
   Floor 11 project) and push it to your fork.
2. **Submit on Canvas.** Turn in the Warden III assignment with the link to your repo.

The due date is on Canvas. Your code must build.
