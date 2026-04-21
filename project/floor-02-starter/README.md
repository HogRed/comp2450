# Floor 2 Starter — *The Sorting Crucible*

This is the Floor 2 starter for **COMP 2450 — The Descent**. Drop the contents into a new folder; this starter is **self-contained** and includes a reference version of your Floor 1 search code so the Bestiary still works while you focus on sorting this week.

You now have **one program** — `the_descent` — that does everything you've unlocked so far.

## What this program does — once you finish your work

```
=== THE SORTING CRUCIBLE ===

What is your name, adventurer? Aldric

Welcome back, Aldric.
Your trophies from Floor 1 are heaped on the floor, unsorted.
The forge at the center of the Crucible burns hot.

(commands:
   search <name>                 — look up a monster in the bestiary
   list                          — list the bestiary
   inventory                     — list your inventory
   sort inventory by <key> [asc|desc]
                                 — key is name, weight, or value
   benchmark [N]                 — race the search algorithms
   benchmark sort [N] [--sorted] [--bad-pivot]
                                 — race the sorting algorithms
   help                          — this screen
   quit                          — leave the dungeon)

> search Goblin
Goblin   HP 8   ATK 2   weakness: fire
> inventory
   1.  Rusty sword       (wt 4.0, val 5)
   2.  Healing potion    (wt 0.5, val 12)
   3.  Iron key          (wt 0.1, val 0)
   4.  Loaf of bread     (wt 0.1, val 1)
   5.  Cloak of shadows  (wt 1.5, val 80)
> sort inventory by weight
   1.  Iron key          (wt 0.1, val 0)
   2.  Loaf of bread     (wt 0.1, val 1)
   3.  Healing potion    (wt 0.5, val 12)
   4.  Cloak of shadows  (wt 1.5, val 80)
   5.  Rusty sword       (wt 4.0, val 5)
> benchmark sort 10000
  N=  10000  mergeSort=   1.205 ms  quicksort=    0.954 ms  std::sort=   0.617 ms
> quit
The forge dies down to embers.
```

Until you finish Floor 2, the sort commands will appear to do nothing — `sort inventory by weight` prints "Unknown sort key" (because `sortInventory` returns `false` from its stub) and every `benchmark sort` column reports 0.000 ms for your two sorts (because they are empty).

## What's already here (do not edit)

| File | Purpose |
|------|---------|
| `main.cpp`                  | CLI loop — parses every command, calls into your sort |
| `bestiary/*`                | The Floor 1 bestiary — loader, search, benchmark. `Search.cpp` is the reference version. |
| `hero/Item.h`               | The `Item` struct |
| `hero/Hero.h / Hero.cpp`    | Hero struct, inventory loader, `printInventory` |
| `hero/Sort.h`               | Interface — three function declarations + `Comparator` typedef |
| `hero/Bench.h / Bench.cpp`  | `benchmark sort` harness (synthetic inventory, timing, `--sorted` / `--bad-pivot` options) |
| `data/monsters.txt`         | Floor 1 bestiary (15 monsters) |
| `data/starter-inventory.txt` | Floor 2 starting inventory (5 items, intentionally unsorted, with a deliberate weight tie) |
| `Makefile`                  | Build target `the_descent` |

## What you write

Open **`hero/Sort.cpp`** — it has three TODOs.

1. **`mergeSort(inventory, cmp)`** — stable, O(n log n) always. Split [lo, hi) in two, recurse, merge. (Monday)
2. **`quicksort(inventory, cmp)`** — in-place, O(n log n) average, O(n²) worst. Use the **middle element** as your pivot to avoid the Pivot Wraith on sorted input. (Wednesday)
3. **`sortInventory(hero, criterion)`** — parse the string (`"weight"`, `"name desc"`, `"value asc"`), build the right comparator, dispatch to a sort of your choice. Defend your pick in the Friday commit message. (Friday)

That is all the code you write this week. The rest of the game calls only through `Sort.h`, so the moment your three functions work, `sort inventory by …` works, `benchmark sort` produces real numbers, and your Lab 2 deliverable is one command away.

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
- `inventory` → 5 items, the order above
- `sort inventory by weight` → ascending weight (Iron key stays before Loaf of bread — stability!)
- `sort inventory by name desc` → reverse alphabetical
- `sort inventory by value` → ascending value
- `benchmark sort 10000` → three timing columns, non-zero
- `benchmark sort --sorted` → the full sweep on pre-sorted input
- `benchmark sort --sorted --bad-pivot` → the Pivot Wraith (run a small N first; this one gets SLOW)

## Lab 2 — *Race the Sorts* (folded into the project)

There is no separate lab handout this week. The `benchmark sort` command is the lab.

**Deliverable:** a `lab-notes.md` file under `floor-02/` in your project repo, containing:

1. The full `benchmark sort` sweep output (paste from your terminal).
2. **At what inventory size does `std::sort` clearly pull ahead** of your hand-rolled sorts? Read your table; pick a row.
3. **Run with `--sorted`.** Which sort gets faster? Which gets slower? *Why?*
4. **Run `benchmark sort --sorted --bad-pivot` at N=10,000.** What does the time for `quicksort*` look like? Compare to your middle-pivot run. This is the Pivot Wraith.
5. **One-paragraph reflection.** If you could only ship one of the three sorts in production, which would you ship? Defend your choice in terms of *worst case*, *average case*, and *what the data is likely to look like*.

Commit `lab-notes.md` along with your code. Your commit history should show at least three commits this week — Mon (merge), Wed (quick), Fri (`std::sort` wiring + lab notes).

## If you get stuck

- **`sort` says "Unknown sort key"** — your `sortInventory` returned `false`. Either it didn't recognize the key, or the stub still returns false.
- **Merge sort hangs / infinite-loops** — your recursion's base case or split is wrong. Base case for the half-open `[lo, hi)` form is `hi - lo < 2`.
- **Merge sort produces the wrong order** — the classic bug is writing `if (cmp(v[i], v[j]))` instead of `if (!cmp(v[j], v[i]))` at the "take from left half" branch. The second form is the one that preserves stability on ties.
- **Quicksort segfaults or hangs on an empty inventory** — early return when `v.size() < 2`. Lomuto partition's closed range `[lo, hi]` breaks on empty.
- **Quicksort crashes near `p - 1` when p == 0** — `std::size_t` is unsigned; `0 - 1` wraps. Guard with `if (p > lo) quicksortImpl(v, lo, p - 1, cmp);`.
- **`benchmark sort` prints 0.000 ms for all three columns** — your mergeSort and quicksort are still stubs. That's expected until you write them.

Bring any error you can't defeat to Wednesday's class.
