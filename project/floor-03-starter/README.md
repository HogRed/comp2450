# Floor 3 Starter — *The Forgemaster's Vault*

This is the Floor 3 starter for **COMP 2450 — The Descent**. Drop the contents into a new folder; the starter is **self-contained** and ships reference versions of your Floor 1 search and Floor 2 sort code, so every command you unlocked in previous weeks still works while you focus on **templates and exceptions**.

You still have **one program** — `the_descent` — that does everything you have unlocked so far.

## What this program does — once you finish your work

```
=== THE FORGEMASTER'S VAULT ===

What is your name, adventurer? Aldric

Welcome back, Aldric.
Empty molds line the walls. The Forgemaster watches, silent.
This week you press one mold to hold every list the keep remembers.

(commands:
   search <name>                 — look up by name in bestiary or inventory
   list                          — list the bestiary
   inventory                     — list your inventory
   inspect <n>                   — show the nth item in your inventory
   sort inventory by <key> [asc|desc]
                                 — key is name, weight, or value
   benchmark [N]                 — race the search algorithms
   benchmark sort [N] [--sorted] [--bad-pivot]
                                 — race the sorting algorithms
   help                          — this screen
   quit                          — leave the dungeon)

> search Goblin
Goblin   HP 8   ATK 2   weakness: fire
> search Iron key
  Iron key          (wt 0.1, val 0)
> inventory
   1.  Rusty sword       (wt 4.0, val 5)
   2.  Healing potion    (wt 0.5, val 12)
   3.  Iron key          (wt 0.1, val 0)
   4.  Loaf of bread     (wt 0.1, val 1)
   5.  Cloak of shadows  (wt 1.5, val 80)
> inspect 3
  Iron key  (wt 0.1, val 0)
> inspect 99
No such item. (index 98 out of bounds for size 5)
> sort inventory by weight
   1.  Iron key          (wt 0.1, val 0)
   2.  Loaf of bread     (wt 0.1, val 1)
   3.  Healing potion    (wt 0.5, val 12)
   4.  Cloak of shadows  (wt 1.5, val 80)
   5.  Rusty sword       (wt 4.0, val 5)
> quit
The forge cools. The molds stand empty again.
```

Until you finish Floor 3:

- `search <item-name>` will NOT yet find items — it only searches the bestiary until you wire Monday's work in.
- `inspect 99` is undefined behavior — may crash, may print garbage — until Friday's `at()` + try/catch.
- After Wednesday, if any of Bag<T>'s method bodies are still TODO stubs, the first command that touches the inventory or bestiary will throw `std::logic_error` with a diagnostic naming the method.

## What's already here (do not edit)

| File | Purpose |
|------|---------|
| `main.cpp`                          | CLI loop — parses every command, calls into your template + exception code. ONE line you'll add here on Friday (a try/catch). |
| `bestiary/Monster.h`                | The `Monster` struct |
| `bestiary/Bestiary.h` / `.cpp`      | Loader, sort, print for the bestiary |
| `bestiary/Search.cpp`               | **Reference:** Floor 1's three monster-specific searches (`linearSearch`, `binarySearch`, `binarySearchRecursive`) and `findMonster`. Kept as-is so the `search` command keeps working. |
| `bestiary/Bench.h` / `.cpp`         | The Floor 1 search benchmark |
| `hero/Item.h`                       | The `Item` struct |
| `hero/Hero.h` / `.cpp`              | Hero struct, inventory loader, `printInventory` |
| `hero/Sort.h` / `.cpp`              | **Reference:** Floor 2's `mergeSort`, `quicksort`, `sortInventory`. |
| `hero/Bench.h` / `.cpp`             | The Floor 2 sort benchmark |
| `data/monsters.txt`                 | Floor 1 bestiary (15 monsters) |
| `data/starter-inventory.txt`        | Floor 2 starting inventory (5 items, weight tie on purpose) |
| `CMakeLists.txt`                    | Build target `the_descent` |

## What you write

Three files. One thing per day.

1. **`bestiary/Search.h`** (Monday) — fill in `findByName<T>`. Three-line body at the bottom of the header. The same source template will be instantiated for `Monster` and `Item` from two different call sites in `main.cpp`.
2. **`hero/Bag.h`** (Wednesday) — fill in the method bodies of `Bag<T>`. Every body is a one-liner that delegates to the private `std::vector<T>`. Then flip `Hero::inventory` to `Bag<Item>`, flip the bestiary's type to `Bag<Monster>`, and follow the compiler errors — every signature that said `std::vector<Monster>` or `std::vector<Item>` needs to say `Bag<Monster>` or `Bag<Item>`.
3. **`hero/BagException.h`** + **`hero/Bag.h::at()`** + **`main.cpp` try/catch** (Friday) — build a diagnostic error message in `BagException`'s constructor, make `Bag<T>::at()` throw it on bad index, and wrap the main command loop in `try`/`catch`.

## Build & run

```
cmake -B build            # configure once (and any time CMakeLists.txt changes)
cmake --build build       # compile
./build/the_descent       # run (macOS / Linux)
build\the_descent.exe     # run (Windows cmd.exe)
```

The CMake build automatically copies `data/monsters.txt` and `data/starter-inventory.txt` next to the binary, so you can launch from either `build/` or the project root.

To wipe the build and start clean: `rm -rf build` (or delete the `build/` folder from your file manager) and re-run the two commands above.

## Demo target (Friday)

You should be able to type — in **one session of one binary**:

- `search Goblin` → goblin's stats (Floor 1 still works)
- `search Iron key` → the iron key's row from your inventory (Monday's template, instantiated for `Item`)
- `inventory` → 5 items, listed
- `inspect 3` → the Iron key's row
- `inspect 99` → `No such item. (index 98 out of bounds for size 5)` — and the prompt comes back, game still alive
- `sort inventory by weight` → ascending weight, Iron key stays before Loaf of bread (stability still works, now through `Bag<Item>`)
- `benchmark sort 10000` → three timing columns, non-zero (now running over `Bag<Item>`)

## Lab 3 — *One Mold, Many Shapes* (folded into the project)

There is no separate lab handout this week. The work above *is* the lab.

**Deliverable:** a `lab-notes.md` file under `floor-03/` in your project repo, containing:

1. A transcript of the demo above, pasted from your terminal.
2. **Instantiate `Bag<int>` in a throwaway test and call `findByName` on it.** Paste the full compiler error. Circle (or describe) the one line that actually tells you what's wrong. Why is this harder to read than a normal type error, and what does that tell you about templates?
3. **Swap `.at()` for `[]`** in the `inspect` command. Pass a bad index. Describe what the program does now — does it crash? Print garbage? Silently return? Restore `.at()` when done. One sentence: when would you ever want `operator[]` over `.at()` in production?
4. **Remove the try/catch around the main loop.** Trigger a bad `inspect`. Describe exactly what you see on stderr — the unhandled-exception message the runtime prints before `std::terminate`. Put the catch back. Then try the anti-pattern `catch (...) { }` (empty body) and describe what is now *worse* than having no catch at all.
5. **Change `BagException` so it does NOT inherit from `std::exception`.** What still compiles? What behavior (if any) do you lose? One sentence on why inheriting from `std::exception` is the right default.

Your commit history this week should show at least three commits — Mon (function template), Wed (class template + container swap), Fri (exceptions + lab notes).

## If you get stuck

- **"TODO: Bag::something() not yet implemented"** — that is the `throw std::logic_error` in the stubs. The message names the method you still need to fill in.
- **`search Iron key` says "no such creature" but `search Goblin` works** — you wired one instantiation but not the other. `main.cpp`'s `search` branch should call `findByName` against both the bestiary and the inventory.
- **Compiler errors that go on for pages when you instantiate `Bag<int>` and call findByName on it** — that's a template error. The message is ugly because the compiler shows you every substitution step. Read bottom-up. The top of the stack is where the problem started; the bottom is where it finally gave up. The Bestiary entry for this floor (The Swallowed Scream) has the survival guide.
- **Wednesday: "no matching function for call to `sortBestiary(Bag<Monster>&)`"** — you changed the bestiary's type but didn't update `sortBestiary`'s parameter. Every function whose signature said `std::vector<Monster>&` or `std::vector<Item>&` needs to switch to `Bag<Monster>&` or `Bag<Item>&`. The compiler will name them; follow its list.
- **Friday: `inspect 99` still crashes** — either `Bag<T>::at()` is still a stub, or BagException's constructor isn't actually writing into `msg_` (so `what()` hands back an empty message and you assume it's still broken), or you forgot the try/catch in main. Those three are the only places the Friday wiring lives.

Bring any error you can't defeat to Wednesday's class.
