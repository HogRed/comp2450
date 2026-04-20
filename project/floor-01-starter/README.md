# Floor 1 Starter — *The Scrying Well*

This is the Floor 1 starter for **COMP 2450 — The Descent**. Drop the contents into your project repo (alongside Floor 0 work, in a sibling folder or as the new state of `main.cpp` — your call; the rule is *one repo grown over the semester*).

## What this program does — once you finish your work

```
=== THE SCRYING WELL ===

What is your name, adventurer? Aldric

Welcome back, Aldric.
The water is black. It shows what you name aloud.
(commands: search <name>, list, benchmark [N], quit)

> search Goblin
Goblin   HP 8   ATK 2   weakness: fire
> search Ghost
No such creature stalks this Keep.
> benchmark 1000
  N=   1000  query=last    linear=    4.231 us  binary=   0.082 us  recursive=   0.094 us
  N=   1000  query=absent  linear=    4.512 us  binary=   0.085 us  recursive=   0.097 us
> quit
The Well goes still.
```

Search is case-sensitive this week — `search goblin` lowercase will report "no such creature." We tackle case-handling in a later floor.

Until you finish your work it does the same thing — except every `search` returns "No such creature." and every `benchmark` row reports `linear=0.000 us  binary=0.000 us  recursive=0.000 us` (because all three searches are stubs).

## What's already here (do not edit)

| File | Purpose |
|------|---------|
| `main.cpp`                 | CLI loop — parses commands, calls into your search |
| `bestiary/Monster.h`       | The `Monster` struct |
| `bestiary/Bestiary.h/.cpp` | Loads `data/monsters.txt`, sorts, prints |
| `bestiary/Search.h`        | Interface — four function declarations |
| `bestiary/Bench.h/.cpp`    | Benchmark harness (synthetic bestiary, timing) |
| `data/monsters.txt`        | The sample bestiary (15 monsters, intentionally unsorted) |
| `Makefile`                 | Build target `scrying_well` |

## What you write

Open **`bestiary/Search.cpp`** — it has four TODOs.

1. **`linearSearch`** — walk the vector, return a pointer to the matching `Monster`, or `nullptr`. (Monday)
2. **`binarySearch`** — iterative, by name, on the *sorted* bestiary. (Wednesday — `main.cpp` calls `sortBestiary()` for you.)
3. **`binarySearchRecursive`** — same contract, but recursive. Use a static helper inside `Search.cpp` that takes `(low, high)`. (Friday)
4. **`findMonster`** — pick which of the three above the rest of the game should use. Defend your pick in your commit message.

That is all the code you write this week. The discipline you practice is *interfaces*: every other file calls only through `Search.h`, so the moment your four functions work, the whole `search` command works AND the `benchmark` command produces meaningful numbers.

## Build & run

```
make           # produces ./scrying_well
make run       # builds and runs
make clean
```

If `make` is unavailable (Windows without MSYS2):

```
g++ -std=c++17 -Wall -Wextra -O2 -o scrying_well main.cpp bestiary/Bestiary.cpp bestiary/Search.cpp bestiary/Bench.cpp
./scrying_well        # Windows: scrying_well.exe
```

The program expects `data/monsters.txt` to be in the working directory you launch from.

## Demo target (Friday)

You should be able to type:

- `search Goblin` → goblin's stats
- `search goblin` → "No such creature." (string compare is case-sensitive — note this; case-insensitive search is **not** required this week, but think about why a player might be annoyed)
- `search Ghost` → "No such creature stalks this Keep."
- `list` → all 15 monsters (now in alphabetical order, since `sortBestiary` ran)
- `benchmark` → the full sweep (10 / 100 / 1k / 10k / 100k); takes a few seconds
- `benchmark 1000` → just N=1000

## Lab 1 — *Race the Bestiary* (folded into the project)

There is no separate lab handout this week. The `benchmark` command is the lab.

**Deliverable:** a `lab-notes.md` file under `floor-01/` in your project repo, containing:

1. The full `benchmark` output (paste from your terminal).
2. **At what bestiary size does binary search start to clearly beat linear?** Read your table; pick a row.
3. **For N=10, which is faster, and why might that surprise a beginner?** (Hint: count what each algorithm actually does at small N.)
4. **What happens when you call `binarySearch` on the *unsorted* bestiary?** Try it: comment out the `sortBestiary(bestiary);` call in `main.cpp`, rebuild, run `search Goblin` and `search Wraith`. Restore `sortBestiary` when done.
5. **Iterative vs. recursive binary search** — do their per-call times differ in your benchmark? By how much? Why might that be? (Think about what extra work happens when you call a function.)

Commit `lab-notes.md` along with your code. Your commit history should show at least three commits this week — Mon (linear), Wed (iterative binary), Fri (recursive + lab).

## If you get stuck

- **Both searches return nullptr forever** — likeliest cause: you wrote `if (bestiary[i].name = name)` (assignment) instead of `==` (comparison). The compiler may not warn.
- **`binarySearch` returns nullptr for monsters that exist** — check that `main.cpp`'s call to `sortBestiary` is happening (it is, but verify by `list`-ing first).
- **Off-by-one in binary search** — the safe pattern is `low <= high` with `high = size - 1`, OR `low < high` with `high = size`. Pick one and commit to it. If you use `size_t` for indices, the half-open `low < high` form avoids a nasty underflow trap when the target is smaller than every element.
- **Stack overflow on `binarySearchRecursive` with large N** — your recursion isn't terminating. Check the base case and that `mid+1` / `mid` actually shrink the range every call.
- **Linker errors mentioning `linearSearch` etc.** — you probably edited `Search.h` instead of `Search.cpp`. Revert the header.

Bring any error you can't defeat to Wednesday's class.
