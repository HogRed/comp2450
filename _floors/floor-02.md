---
title: The Sorting Crucible
floor_number: Floor 2
flavor_quote: "Order is not given to the world. Order is hammered into it."
week: 3
zybook: "Chapter 3"
---

## The briefing

You step from the Scrying Well's chamber onto a stair of black iron and descend until the air smells of hot metal. The room beyond is round, low-ceilinged, and lit by a single forge in the center — the **Crucible**. Heaped on the floor around it are your trophies from Floor 1: the loot you collected and never bothered to organize.

Last week, the Well showed you that **binary search demands sorted data**. The Bestiary was sorted *for you* before you ever touched it. The Crucible asks the harder question: **what if no one sorts it for you?**

This week you build the sorting. By Friday your hero will be able to type `sort inventory by weight` — or `by name desc`, or `by value` — and watch the heap re-arrange itself. You will write **two** sorting algorithms by hand, then learn the C++ way to ask for one (`std::sort`) and the comparator pattern that makes it bend to whatever order *you* want.

You will learn three sorts:

1. **Merge sort** — the canonical **divide-and-conquer** sort. Split the vector in half, sort each half (recursively!), then merge the two sorted halves. Predictable `O(n log n)`. Stable. Allocates a scratch buffer.
2. **Quicksort** — also divide-and-conquer, but the work is in the *split*, not the merge. Pick a **pivot**, partition into "less than pivot" and "greater than pivot," recurse on each side. Average `O(n log n)`, in-place, ferociously fast in practice — but a *bad pivot* can degrade it to `O(n²)`.
3. **`std::sort`** — what production C++ actually reaches for. Not a single algorithm: it's a hybrid (introsort) tuned by people who think about cache lines for a living. You don't write it; you learn to *aim* it with a comparator.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/mergesort-tree.svg' | relative_url }}" alt="Merge sort split-and-merge tree on eight items"/>
  <figcaption>Merge sort. Split until each piece holds one item; merge pairs back in sorted order. Three levels of split, three of merge — each doing O(n) work — gives O(n log n).</figcaption>
</figure>

<figure class="diagram">
  <img src="{{ '/assets/diagrams/quicksort-partition.svg' | relative_url }}" alt="Quicksort partition: pivot separates values less than it from values greater than it"/>
  <figcaption>Quicksort's core move. Pick a pivot, shuffle so everything smaller sits left and everything larger sits right — then recurse on each side. All the work is in the partition.</figcaption>
</figure>

Then you race them. The benchmark from Floor 1 grows a sorting harness, and you watch the three curves on real data.

## Objectives

By the end of Floor 2 you will be able to:

- Implement **merge sort** on a `std::vector<Item>` from scratch, including the merge step.
- Implement **quicksort** on a `std::vector<Item>` from scratch, including a partition step around a pivot.
- Pass a **comparator** (a function or lambda) to `std::sort`, your `mergeSort`, and your `quicksort` so that *one* sorting routine produces multiple orderings.
- State the average and worst-case Big-O of merge sort and quicksort, and explain *why* quicksort can degrade.
- Define **stable** and **in-place** sorting, and identify which of the three you wrote (or used) is which.
- Read a benchmark table and identify the input size at which `O(n log n)` clearly beats `O(n²)` — or at which `std::sort` clearly beats your hand-rolled implementations.

## Pre-class

### Reading (ZyBook Ch. 3)

Before **Monday**: §3.1 Sorting: introduction; §3.5 Merge sort
Before **Wednesday**: §3.4 Quicksort
Before **Friday**: §3.8 Overview of fast sorting algorithms; skim §3.2 Selection sort and §3.3 Insertion sort (we will not implement them, but you should be able to recognize them and say *why* they're slow)

The book also covers Shell sort (§3.6) and Radix sort (§3.7). Both are interesting; neither is on this week's exam. Read for curiosity, not for grade.

Work the Question Sets and Animations inside the ZyBook — they count toward your participation grade. The Animation in §3.5 is the best 90 seconds you will spend this week.

There are no pre-class videos. Class time is for live coding and discussion together — your reading is the prep.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | Divide-and-conquer + merge sort | "Sort yourselves alphabetically by first name" warmup; live-code `mergeSort` and `merge` together; watch it sort the inventory; commit |
| **W** | Quicksort + the pivot question  | Live-code `partition` and `quicksort`; demo a bad pivot on already-sorted input and watch it degrade; discuss why `std::sort` doesn't pick first-element pivots |
| **F** | Comparators + `std::sort` + race the sorts | Add the `<criterion>` parameter; pass lambdas to all three sorts; run `benchmark sort` and read the table together; class discussion of when you'd reach for which |

## The project — Floor 2

This week's project increment is the **`sort inventory` command** and the underlying machinery.

You will receive (in your starter drop):

- An `Item` struct with `name`, `weight`, `value`.
- A `Hero` whose inventory is a `std::vector<Item>` (already populated with starting loot).
- The CLI wired so that `sort inventory by <criterion>` calls into a function **you write**: `sortInventory(...)`, which dispatches to one of the three sorts.
- A header `hero/Sort.h` declaring the functions you need to implement.
- A `benchmark sort` command, fully implemented, that times `mergeSort`, `quicksort`, and `std::sort` against synthetic inventories from N=10 up to N=100,000.

You will write (in `hero/Sort.cpp`):

1. **Monday:** `mergeSort` and the helper `merge`.
2. **Wednesday:** `quicksort` and the helper `partition`.
3. **Friday:** `sortInventory` — read the criterion string, build the right comparator, dispatch to whichever sort you want as the default, and defend your pick in your commit message.

Demo target (Friday):

```
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
> sort inventory by name desc
  1.  Rusty sword       (wt 4.0, val 5)
  2.  Loaf of bread     (wt 0.1, val 1)
  3.  Iron key          (wt 0.1, val 0)
  4.  Healing potion    (wt 0.5, val 12)
  5.  Cloak of shadows  (wt 1.5, val 80)
> benchmark sort 10000
  N=  10000  mergeSort=  1.20 ms  quicksort=  0.95 ms  std::sort=  0.62 ms
```

Try `sort inventory by weight` *twice in a row* and notice nothing visibly changes — that's correct. A sort is **idempotent** on already-sorted data. (Not free, though — that's a Friday discussion.)

### Lab 2 — *Race the Sorts* (folded into the project)

There is no separate lab handout. The `benchmark sort` command IS the lab.

Commit `floor-02/lab-notes.md` to your project repo with:

1. The full `benchmark sort` output (paste from your terminal).
2. **At what inventory size does `std::sort` clearly pull ahead** of your hand-rolled sorts? Read your table; pick a row.
3. **Run the benchmark with a *pre-sorted* input** (the harness has a `--sorted` flag). Which sort gets faster? Which gets slower? *Why?*
4. **Quicksort with a first-element pivot on a sorted input** — try it. (The harness has a `--bad-pivot` flag.) Time it on N=10,000. What does the curve look like? Show your work.
5. **One-paragraph reflection.** If you could only ship one of the three sorts in production, which would you ship? Defend your choice in terms of *worst case*, *average case*, and *what the data is likely to look like*.

Your commit history this week should show at least three commits — Mon (merge), Wed (quick), Fri (`std::sort` + lab notes).

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 2</p>

**The Pivot Wraith** — *HP: situational. Damage: silent and slow.*
Strikes when you call `quicksort` with a fixed-position pivot (always-first, always-last) on data that is already nearly sorted. Your `O(n log n)` average case becomes `O(n²)` worst case — no crash, no error, just a sort that takes a thousand times longer than it should and a benchmark you can't explain.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/quicksort-pivot.svg' | relative_url }}" alt="Two recursion trees: a balanced tree of depth log n, and a skewed tree of depth n"/>
  <figcaption>Quicksort's cost lives in the tree's depth. Each level touches n items. A balanced split gives log n levels; a skewed split gives n — the same algorithm, but a hundred times slower.</figcaption>
</figure>

Counter by:

- Picking the **middle element** as the pivot for this week's hand-rolled `quicksort` — it sidesteps the trivial sorted-input pathology.
- Knowing in your bones: **average case is what your data probably looks like; worst case is what an attacker, or a bad day, will hand you.** A real `std::sort` switches algorithms when it detects bad recursion depth (introsort). Yours does not.
- When in doubt, reach for `std::sort`. It is faster and meaner than anything you will write this week.
</div>

<div class="callout check" markdown="1">
<p class="callout-title">Check for understanding</p>

Before you descend to Floor 3, you should be able to answer these without looking:

1. What does **divide-and-conquer** mean? Name two algorithms that use it (one you wrote this week, one you wrote last week).
2. Merge sort is `O(n log n)` *always*. Quicksort is `O(n log n)` *on average*. Why isn't quicksort always `O(n log n)`?
3. Define **stable sort** in one sentence. Suppose `Iron key` (wt 0.1) appears before `Loaf of bread` (wt 0.1) in your inventory. After a **stable** `sort by weight`, which appears first — and what does an **unstable** sort fail to guarantee about those two? (Hint: this is why to get *weight-primary, name-tiebreak* order you sort by name **first** and then stably by weight — not the other way around.)
4. Define **in-place sort**. Which of {merge sort, quicksort, std::sort} are in-place?
5. A **comparator** is a function (or lambda) that returns true if `a` should come before `b`. Why does taking a comparator let one sort routine produce *every possible ordering* of your items?
6. In our codebase, `sortInventory` is the one place the rest of the game asks "put these items in order." Why is that a good design — i.e., what would change in Week 11 (when the inventory becomes a balanced tree) if every command sorted directly?

Answers are discussed in the Monday Floor 3 warmup.
</div>

---

*The Crucible's forge dies down to embers. Your inventory is sorted. The stair to Floor 3 — the **Forgemaster's Vault** — opens only once you can put your house in order.*
