---
title: The Scrying Well
floor_number: Floor 1
flavor_quote: "Every thing that lives in this Keep has a name. The Well will show you any of them — if you can find the name first."
week: 2
zybook: "Chapter 2"
---

## The briefing

You descend the stair from the Antechamber into a circular stone chamber. In the center, set into the floor, is a black-water **well** — the **Scrying Well**. Whatever you name aloud, it shows you. A goblin's stats. A spell's effect. The shape of a beast you have not yet met.

Beside the well is a **stone tablet** — the **Bestiary** — listing every creature in the Keep. Without the Bestiary, you would have nothing to ask for. Without a way to *look up* a name in the Bestiary, the Bestiary is just a wall of dead words.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/mccown.svg' | relative_url }}" alt="Scrivener McCown, Keeper of the Bestiary — scholar with salt-and-pepper hair and beard, dark indigo robe with brass clasp, ledger and quill at chest."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Scrivener McCown, Keeper of the Bestiary</span>

Every creature in this Hold has a name. Every name is in this book — I wrote it. The Plague cannot touch what is already cataloged; that is why I returned, and why these pages are the most valuable thing in the monastery. If you meet a beast that is not listed, bring me its name.
</div>
</div>

This week you build the looking-up. By Friday your hero will be able to type `search Goblin` and have the goblin's stats appear, instantly — and you will be able to *measure* how fast it is.

You will learn three ways to search:

1. **Linear search** — start at the top, check every entry, stop when you find it. Always works. Slow when the Keep is full.
2. **Binary search (iterative)** — keep cutting the remaining list in half. Blazingly fast. *Only works on a sorted list*, and the silent failure mode when the list isn't sorted is one of the most dangerous bugs in this course.
3. **Binary search (recursive)** — the same algorithm, expressed as a function that calls itself. Your first taste of the **recursive thinking** that will dominate the second half of the semester.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/linear-vs-binary.svg' | relative_url }}" alt="Linear search takes seven probes to find 7 in a sorted array of eight cells, walking left to right. Binary search finds it in three probes by halving the remaining range each step."/>
  <figcaption>The same sorted array, the same target. Linear walks every cell until the target appears. Binary halves the remaining range each step — three probes instead of seven on a list of eight; twenty probes instead of a million on a list of a million.</figcaption>
</figure>

Then you learn to talk about how *fast* each one is — your first real encounter with **Big-O notation** — and you measure them yourself with a built-in benchmark.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/big-o-growth.svg' | relative_url }}" alt="Growth curves for five complexity classes. O(1) and O(log n) stay almost flat. O(n) rises as a diagonal line. O(n log n) curves gently above it. O(n squared) launches off the top of the chart before n reaches 12."/>
  <figcaption>Big-O, visualized. Cheap algorithms hug the floor; expensive ones punch through the ceiling. The y-axis is schematic — what matters is the <em>shape</em>. Linear search is the orange line; binary search is the gold one just above flat.</figcaption>
</figure>

## Objectives

By the end of Floor 1 you will be able to:

- Implement **linear search** over a `std::vector<Monster>` from scratch.
- Implement **iterative binary search** over a *sorted* `std::vector<Monster>`.
- Implement **recursive binary search** using a private helper that carries the search range.
- State, without looking, the Big-O of each: `O(n)` and `O(log n)`.
- Explain — in your own words and with an example — why binary search requires a sorted input, and what goes wrong if you skip that step.
- Read a benchmark table and identify the input size at which `O(log n)` clearly beats `O(n)`.

## Pre-class

### Reading (ZyBook Ch. 2)

Before **Monday**: §2.1 Searching and algorithms (covers linear search)
Before **Wednesday**: §2.2 Binary search; skim §2.3 Growth of functions and complexity (focus on Big-O, not Big-Ω/Big-Θ)
Before **Friday**: §2.4 Constant time operations; §2.5 O notation; §2.6 Algorithm analysis; §2.7 Recursive definitions; §2.8 Recursive algorithms (the recursive binary search example is the punchline); §2.9 Time complexity of recursive algorithms (skim — recurrence relations are not on this week's exam)

Yes, that's a lot. The §2.4–2.6 material on Big-O is short and largely redundant; read for fluency, not depth. Recursion (§2.7–2.9) is a *first pass* — you'll see it again on Floor 4½ and use it heavily on Floors 8–9.

Work the Question Sets and Animations inside the ZyBook — they count toward your participation grade.

There are no pre-class videos. Class time is for live coding and discussion together — your reading is the prep.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | Linear search                          | Live-code `linearSearch` together against the 15-entry bestiary; pair-debug an off-by-one I plant in the loop; commit it to your project |
| **W** | Iterative binary search + Big-O        | *Guess my number* warmup; live-code iterative `binarySearch` on the *sorted* bestiary; derive `O(n)` vs `O(log n)`; finish with the question "what if the list isn't sorted?" |
| **F** | Recursion + the benchmark              | Walk through `binarySearchRecursive` together — same algorithm, recursive form; introduce the helper-function pattern; everyone runs `benchmark` and we read the table together; class discussion of growth curves |

## The project — Floor 1

This week's project increment is **the entire `search` and `benchmark` machinery**.

You will receive (in your starter drop):

- A `Monster` struct with `name`, `hp`, `attack`, `weakness`.
- A `Bestiary` loader that reads `data/monsters.txt` into a `std::vector<Monster>`.
- The CLI wired so that `search <name>` calls into a function **you write**: `findMonster(...)`, which delegates to one of the three searches.
- A header `bestiary/Search.h` declaring the four functions you need to implement.
- A `benchmark` command, fully implemented, that times all three of your searches against synthetic bestiaries from N=10 up to N=100,000.

You will write (in `bestiary/Search.cpp`):

1. **Monday:** `linearSearch`
2. **Wednesday:** `binarySearch` (iterative)
3. **Friday:** `binarySearchRecursive`
4. **Friday:** `findMonster` — pick whichever you want the rest of the game to use; defend your pick in your commit message.

Demo target (Friday):

```
> search Goblin
Goblin   HP 8   ATK 2   weakness: fire
> search Ghost
No such creature stalks this Keep.
> benchmark 10000
  N=  10000  query=last    linear=   42.1 us  binary=  0.10 us  recursive=  0.13 us
  N=  10000  query=absent  linear=   45.3 us  binary=  0.10 us  recursive=  0.13 us
```

(Search is case-sensitive this week. Try `search goblin` lowercase and notice it fails — we will handle case in a later floor.)

### Lab 1 — *Race the Bestiary* (folded into the project)

There is no separate lab handout. The `benchmark` command IS the lab.

Commit `floor-01/lab-notes.md` to your project repo with:

1. The full `benchmark` output (paste from your terminal).
2. **At what bestiary size does binary search start to clearly beat linear?** Read your table; pick a row.
3. **For N=10, which is faster, and why might that surprise a beginner?**
4. **What happens if you call `binarySearch` on the *unsorted* bestiary?** Try it: comment out the `sortBestiary(bestiary);` call in `main.cpp`, rebuild, search a few names. Restore `sortBestiary` when done. Write what you saw and why.
5. **Iterative vs. recursive** — do their per-call times differ in your benchmark? By how much? Why?

Your commit history this week should show at least three commits — Mon (linear), Wed (iterative binary), Fri (recursive + lab notes).

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 1</p>

**The Unsorted Lich** — *HP: invisible. Damage: silent.*
Strikes when you call `binarySearch` (or `binarySearchRecursive`) on a list that is not sorted. Returns `nullptr` for things that *are* in the bestiary. Returns a wrong-but-plausible monster for things that aren't. Never crashes. Never errors. Just lies.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/unsorted-lich.svg' | relative_url }}" alt="Binary search on an unsorted eight-monster array, looking for Goblin. It probes Lich at index 3, then Troll at index 1, then Ogre at index 0, then returns null. Meanwhile Goblin sits at index 4 — one cell to the right of the very first probe — but the algorithm's left-half decisions never look there."/>
  <figcaption>The silent failure. Probe 1 lands on Lich, decides Goblin must be to the left, and throws away the half of the array where Goblin actually lives. No crash. No error. Just <code>nullptr</code>.</figcaption>
</figure>

Counter by:

- Sorting the bestiary *before* the first binary call (`std::sort` works fine here).
- Writing an `assert(isSorted(bestiary))` at the top of `binarySearch` while you are learning. Remove it later if you must, but never if you are unsure.
- Knowing in your bones: **`linearSearch` always works. `binarySearch` works only if you uphold its precondition.**
</div>

<div class="callout check" markdown="1">
<p class="callout-title">Check for understanding</p>

Before you descend to Floor 2, you should be able to answer these without looking:

1. What is the worst-case Big-O of linear search? Of binary search? Why?
2. Binary search is faster, so why would anyone ever use linear search?
3. You call `binarySearch` on an unsorted bestiary and it returns `nullptr` for "Goblin" even though "Goblin" is in the list. Explain *exactly* why, in terms of the algorithm's steps.
4. If your bestiary doubles in size from 1,000 to 2,000 monsters, how many *more* comparisons does linear search do in the worst case? How many more does binary search do?
5. Iterative and recursive binary search have the same Big-O. Why might the iterative version still be a better default in production code?
6. In our codebase, `findMonster` is the one place the rest of the game asks "is this monster in the bestiary?" Why is that a good design — i.e., what would change in Week 11 if every place in the game called the search function directly?

Answers are discussed in the Monday Floor 2 warmup.
</div>

---

*The Well is dark and waiting. Speak a name. The stair to Floor 2 — the **Sorting Crucible** — opens only once you can find what you are looking for.*
