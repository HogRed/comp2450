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

This week you build the looking-up. By Friday your hero will be able to type `search goblin` and have the goblin's stats appear, instantly.

You will learn two ways to search:

1. **Linear search** — start at the top, check every entry, stop when you find it. Always works. Slow when the Keep is full.
2. **Binary search** — keep cutting the remaining list in half. Blazingly fast. *Only works on a sorted list*, and the silent failure mode when the list isn't sorted is one of the most dangerous bugs in this course.

Then you learn to talk about how *fast* each one is — your first real encounter with **Big-O notation**.

## Objectives

By the end of Floor 1 you will be able to:

- Implement **linear search** over a `std::vector<Monster>` from scratch.
- Implement **binary search** (iterative) over a *sorted* `std::vector<Monster>`.
- State, without looking, the Big-O of each: `O(n)` and `O(log n)`.
- Explain — in your own words and with an example — why binary search requires a sorted input, and what goes wrong if you skip that step.
- Predict, given two algorithms with different growth rates, which one wins as input size grows.

## Pre-class

### Reading (ZyBook Ch. 2)

Before **Monday**: §2.1 Searching, §2.2 Linear search
Before **Wednesday**: §2.3 Binary search
Before **Friday**: §2.4 Algorithm analysis (Big-O), §2.5 Recursive search (skim — full recursion later)

Work the Question Sets and Animations inside the ZyBook — they count toward your participation grade.

### Videos (watch before the matching class)

| # | For | Length | Title |
|---|-----|--------|-------|
| 1.1 | Monday    | ~10 min | *Linear search — checking every door* |
| 1.2 | Wednesday | ~12 min | *Binary search — halving the dungeon* |
| 1.3 | Friday    | ~10 min | *Big-O for searchers — when does fast actually win?* |

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | Linear search                  | Live-code `linearSearch` together against a 12-entry bestiary; pair-debug an off-by-one I plant in the loop; add it to the project |
| **W** | Binary search                  | *Guess my number* warmup; live-code iterative `binarySearch` on the *sorted* bestiary; finish with the question "what if the list isn't sorted?" |
| **F** | Big-O & the Unsorted Lich      | *Race the Bestiary* lab (below); class discussion of growth curves; demo: 10 / 1,000 / 100,000-entry bestiary timed |

## Lab (Friday, in-class or open-lab)

**Lab 1 — "Race the Bestiary."**

You will be given three bestiaries: 10 monsters, 1,000 monsters, and 100,000 monsters (auto-generated). Run both `linearSearch` and `binarySearch` against each, with both **best-case** (target near the front), **worst-case** (target at the end / not present), and **average-case** target locations.

Record the times. Plot or tabulate them. Answer in writing:

1. At what bestiary size does binary search start to clearly beat linear?
2. For the 10-monster bestiary, which is faster? Why might that surprise a beginner?
3. What happens when you call `binarySearch` on the *unsorted* 1,000-entry bestiary? (Try it. Do not trust your output.)

Deliverable: your timing table + your three written answers, committed to your project repo as `floor-01/lab-notes.md`.

## The project — Floor 1

This week's project increment is the **`search` command**.

You will receive (in your starter drop):

- A `Monster` struct with `name`, `hp`, `attack`, `weakness`.
- A `Bestiary` loader that reads `data/monsters.txt` into a `std::vector<Monster>`.
- The CLI wired so that `search <name>` calls into a function **you write**: `Monster* findMonster(const std::vector<Monster>&, const std::string& name)`.
- A header `bestiary/Search.h` declaring the two functions you need to implement.

You will write:

1. `linearSearch` — returns a pointer to the matching `Monster`, or `nullptr` if not found.
2. `binarySearch` — same signature; assumes the bestiary has been sorted by name.
3. Wire `findMonster` to call whichever search makes sense (your choice; defend it in your commit message).

Demo target (Friday):

```
> search Goblin
Goblin   HP 8   ATK 2   weakness: fire
> search Ghost
No such creature stalks this Keep.
```

(Search is case-sensitive this week. Try `search goblin` lowercase and notice it fails — we will handle case in a later floor.)

Commit message must include: *which* search you used in `findMonster` and *why*.

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 1</p>

**The Unsorted Lich** — *HP: invisible. Damage: silent.*
Strikes when you call `binarySearch` on a list that is not sorted. Returns `nullptr` for things that *are* in the bestiary. Returns a wrong-but-plausible monster for things that aren't. Never crashes. Never errors. Just lies.

Counter by:

- Sorting the bestiary *before* the first `binarySearch` call (`std::sort` works fine here).
- Writing an `assert(isSorted(bestiary))` at the top of `binarySearch` while you are learning. Remove it later if you must, but never if you are unsure.
- Knowing in your bones: **`linearSearch` always works. `binarySearch` works only if you uphold its precondition.**
</div>

<div class="callout check" markdown="1">
<p class="callout-title">Check for understanding</p>

Before you descend to Floor 2, you should be able to answer these without looking:

1. What is the worst-case Big-O of linear search? Of binary search? Why?
2. Binary search is faster, so why would anyone ever use linear search?
3. You call `binarySearch` on an unsorted bestiary and it returns `nullptr` for "goblin" even though "goblin" is in the list. Explain *exactly* why, in terms of the algorithm's steps.
4. If your bestiary doubles in size from 1,000 to 2,000 monsters, how many *more* comparisons does linear search do in the worst case? How many more does binary search do?
5. In our codebase, `findMonster` is the one place the rest of the game asks "is this monster in the bestiary?" Why is that a good design — i.e., what would change in Week 11 if every place in the game called the search function directly?

Answers are discussed in the Monday Floor 2 warmup.
</div>

---

*The Well is dark and waiting. Speak a name. The stair to Floor 2 — the **Sorting Crucible** — opens only once you can find what you are looking for.*
