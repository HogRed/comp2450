# Floor 1 Starter — *The Scrying Well*

This is the Floor 1 starter for **COMP 2450 — The Descent**. Drop the contents into your project repo (alongside Floor 0 work, in a sibling folder or as the new state of `main.cpp` — your call; the rule is *one repo grown over the semester*).

## What this program does — once you finish your work

```
=== THE SCRYING WELL ===

What is your name, adventurer? Aldric

Welcome back, Aldric.
The water is black. It shows what you name aloud.
(commands: search <name>, list, quit)

> search Goblin
Goblin   HP 8   ATK 2   weakness: fire
> search Ghost
No such creature stalks this Keep.
> quit
The Well goes still.
```

Search is case-sensitive this week — `search goblin` lowercase will report "no such creature." We tackle case-handling in a later floor.

Until you finish your work it does the same thing — except every `search` returns "No such creature." Both `linearSearch` and `binarySearch` are stubs that return `nullptr`.

## What's already here (do not edit)

| File | Purpose |
|------|---------|
| `main.cpp`                 | CLI loop — parses commands, calls into your search |
| `bestiary/Monster.h`       | The `Monster` struct |
| `bestiary/Bestiary.h/.cpp` | Loads `data/monsters.txt`, sorts, prints |
| `bestiary/Search.h`        | Interface — three function declarations |
| `data/monsters.txt`        | The sample bestiary (15 monsters, intentionally unsorted) |
| `Makefile`                 | Build target `scrying_well` |

## What you write

Open **`bestiary/Search.cpp`** — it has three TODOs.

1. **`linearSearch`** — walk the vector, return a pointer to the matching `Monster`, or `nullptr`.
2. **`binarySearch`** — iterative, by name, on the *sorted* bestiary. (`main.cpp` calls `sortBestiary()` for you.)
3. **`findMonster`** — pick which of the two above the rest of the game should use. Defend your pick in your commit message.

That is all the code you write this week. The discipline you practice is *interfaces*: every other file calls only through `Search.h`, so the moment your three functions work, the whole `search` command works.

## Build & run

```
make           # produces ./scrying_well
make run       # builds and runs
make clean
```

If `make` is unavailable (Windows without MSYS2):

```
g++ -std=c++17 -Wall -Wextra -O2 -o scrying_well main.cpp bestiary/Bestiary.cpp bestiary/Search.cpp
./scrying_well
```

The program expects `data/monsters.txt` to be in the working directory you launch from.

## Demo target (Friday)

You should be able to type:

- `search Goblin` → goblin's stats
- `search goblin` → "No such creature." (string compare is case-sensitive — note this; case-insensitive search is **not** required this week, but think about why a player might be annoyed)
- `search Ghost` → "No such creature stalks this Keep."
- `list` → all 15 monsters (now in alphabetical order, since `sortBestiary` ran)

## Lab 1 deliverable — *Race the Bestiary*

Generate larger bestiaries (script provided in class), time both searches, and write up your findings. See the floor page on the course site for the full lab brief.

Commit your `lab-notes.md` alongside your code under `floor-01/`.

## If you get stuck

- **Both searches return nullptr forever** — likeliest cause: you wrote `if (bestiary[i].name = name)` (assignment) instead of `==` (comparison). The compiler may not warn.
- **`binarySearch` returns nullptr for monsters that exist** — check that `main.cpp`'s call to `sortBestiary` is happening (it is, but verify by `list`-ing first).
- **Off-by-one in binary search** — the safe pattern is `low <= high` with `high = size - 1`, OR `low < high` with `high = size`. Pick one and commit to it.
- **Linker errors mentioning `linearSearch` etc.** — you probably edited `Search.h` instead of `Search.cpp`. Revert the header.

Bring any error you can't defeat to Wednesday's class.
