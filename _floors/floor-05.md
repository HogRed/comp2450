---
title: The Eye of Scrying
floor_number: Floor 5
order: 80
flavor_quote: "The lens does not know the shelf. The lens only knows how to move and how to stop."
week: 7
zybook: "Chapter 6 (continued) — class focus §6.14 (iterators)"
---

## The briefing

Beyond Mavren's vault the corridor narrows into a circular chamber lit by a single hanging brass lantern. The walls are smooth stone, unmarked except for a wide unlidded eye carved at the apex of the ceiling. A woman stands at the centre with a strip of pale cloth tied across her eyes. A small glass lens hangs on a brass chain at her sternum; she does not lift it for you.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/vael.svg' | relative_url }}" alt="Sister Vael, the Lensbearer — pale woman in deep-violet robes with silver-thread trim at the collar, a strip of unbleached linen tied across her eyes with a small knot at her right temple, dark hair pulled back tight, a polished glass lens the size of a coin in a brass rim hanging on a brass chain at her sternum."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Sister Vael, the Lensbearer</span>

I cannot see your inventory. I cannot see Mavren's chain. The world above ground I have not seen in thirty years. And yet I can walk any list in this dungeon from end to end and find what I am looking for in it. *How.* — Because I do not look at the list. I look through a lens, and the lens knows two things only: how to advance, and where to stop. The shelf becomes the bag. The bag becomes the chain. The chain becomes the tree, on a floor you have not yet seen. The lens does not change. Build me one lens, and you build it for every shape the dungeon will ever hand you.
</div>
</div>

For two floors now you have walked the chain by hand. `for (auto* p = chain.head(); p != nullptr; p = p->next)` for newest-first. `for (auto* p = chain.tail(); p != nullptr; p = p->prev)` for oldest-first. And for the bag, on every floor before that, you wrote `for (std::size_t i = 0; i < bag.size(); ++i)`. Three different loop shapes for the *same operation*: visit each element in turn.

This week you collapse all three into one. You build a small type — `Chain<T>::iterator` — that wraps the messy details of "follow the next pointer until you hit nullptr" inside the four operators every C++ programmer already knows: `*`, `->`, `++`, `==`. Once that type exists, `for (const auto& event : hero.eventLog)` walks the chain. `std::find(eventLog.begin(), eventLog.end(), needle)` searches it. And `printLog` collapses from two near-identical functions into one templated function that takes a begin and an end and does not care which container they came from.

You will learn three things this week, and they sit on top of each other:

1. **What an iterator IS.** A small object that points at a position. Five operations define a *forward iterator*: dereference (`*it` returns the element), member access (`it->name`), advance (`++it` moves to the next position), and equality / inequality (`it == end` tells you when to stop). That is the entire contract. Implement those five, and the standard library — `std::find`, `std::copy`, range-based `for`, everything — works on your container without knowing what your container is.
2. **`begin()` and `end()`.** The container hands you two iterators: one at the first position, one *one past the last*. That "one past the last" is a sentinel — you never dereference it. The loop runs while `it != end`, advancing each step. Range-based `for` is sugar for exactly this.
3. **Iterator categories.** Not all iterators are equal. A `Chain<T>::iterator` starts the week moving only forward (`++`). Once you write `operator--` on Friday it can step backward too, which makes it a **bidirectional iterator**. It still cannot jump. A `Bag<T>::iterator` (which is really a `std::vector::iterator` underneath) can move forward, backward, and jump by an integer — it is a **random-access iterator**. `std::find` works on both. `std::sort` requires random access. When you try `std::sort(eventLog.begin(), eventLog.end())` the compiler will tell you, in a wall of templates, that you've asked the wrong question.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/iterator-as-lens.svg' | relative_url }}" alt="A hand holds a small circular glass lens on a brass chain. Behind the lens, dimmed and out of focus, sits a row of three boxes connected by arrows — the container — and below that, a row of cells with indices — another container. Arrows from the lens show it moving rightward step by step. Two captions: 'advance' on the lens's motion, 'stop' on a small marker beyond the last cell of either row."/>
  <figcaption>The iterator is a lens. It knows how to advance (<code>++it</code>) and where to stop (<code>it == end</code>). It does <em>not</em> know whether the shape behind it is a chain or a bag — and the code that uses it does not need to know either.</figcaption>
</figure>

<figure class="diagram">
  <img src="{{ '/assets/diagrams/iterator-categories.svg' | relative_url }}" alt="A four-rung ladder. From bottom to top: Input (single-pass, read-only); Forward (multi-pass, advance only); Bidirectional (advance and retreat); Random-access (jump by integer in O(1)). Beside the ladder, a column lists which standard algorithms unlock at each rung: std::find at Input/Forward; std::reverse at Bidirectional; std::sort at Random-access."/>
  <figcaption>Iterator categories — each rung adds an operation. <code>Chain&lt;T&gt;::iterator</code> reaches the Bidirectional rung once Friday's <code>operator--</code> works. <code>Bag&lt;T&gt;::iterator</code> reaches Random-access. <code>std::find</code> needs almost nothing; <code>std::sort</code> needs the top rung.</figcaption>
</figure>

<div class="callout lore" markdown="1">
<p class="callout-title">The fifth category — the lens that writes</p>

The ladder above is about *reading*: each rung up adds a way to **move**. The fifth category, the **output iterator**, stands beside the ladder rather than on it, because it adds a way to **write**. It promises two things only: `*out = value` stores a value, and `++out` moves to the next slot. You may not read through it, and you may not go back.

The standard library hands you two ready-made ones in `<iterator>`:

```cpp
// a lens that writes to the screen
std::copy(hero.eventLog.begin(), hero.eventLog.end(),
          std::ostream_iterator<std::string>(std::cout, "\n"));

// a lens that appends to a vector — and the honest way to sort a chain
std::vector<std::string> sorted;
std::copy(hero.eventLog.begin(), hero.eventLog.end(), std::back_inserter(sorted));
std::sort(sorted.begin(), sorted.end());   // fine: a vector's iterator is random-access
```

`std::copy` takes two lenses to read with and one to write with, and knows nothing about what is behind any of them. Your own `Chain<T>::iterator` can stand on the writing side too: `*it` returns `T&`, so `*it = value` works. `const_iterator` cannot, because `*it` returns `const T&` — and that refusal is the reason it exists.
</div>

`Chain<T>` already has `begin()`-shaped behavior under another name — `head()` is an iterator-like handle. This week you wrap it in a real iterator type and finally retire the hand-walking.

## Objectives

By the end of Floor 5 you will be able to:

- Define a nested `iterator` type inside a container template and implement the operators that make it a forward iterator, `++` (pre/post) and `==` / `!=`, and then `--`, which makes it bidirectional. (`*` and `->` ship written — a reference-returning dereference can't be honestly stubbed — but you'll explain what they return and why.)
- Define a separate `const_iterator` and explain *why* — what `const Chain<T>&` would otherwise let you do that it shouldn't.
- Wire `begin()`, `end()`, `cbegin()`, `cend()` on `Chain<T>` and walk a chain with `for (const auto& s : chain) ...`.
- Read the *single* templated `printLog(first, last, ...)` the starter ships — it replaced Floor 4½'s `printLog`/`printLogOldest` pair — and explain how one function walks a chain forward (`begin`/`end`) and backward (`rbegin`/`rend`) without knowing the container.
- Use `std::find` and `std::find_if` against `Chain<std::string>`, `Bag<Item>`, and `Bag<Monster>` from one source line each.
- Read a `std::sort` template error against a `Chain` iterator and name the missing operation it asked for.
- Name the five iterator categories — the four rungs of the ladder in order, plus the output iterator beside it — and identify which ones your container's iterator belongs to.

## Pre-class

### Reading (ZyBook Ch. 6, continued — the iterator sections)

**The whole of Chapter 6 was assigned back on Floor 4** — this week we reach its iterator material (§6.14). If you read the chapter through on Floor 4, this is review; if not, read the rest now. The pointers below are what *this week's* class leans on.

Before **Monday**: §6.14 *List* — read the first half, where an *iterator* is introduced as "an object that points to a location in a list," and `begin()` / `end()` give you the two ends. That arrow-that-points-into-a-container is the abstraction you'll build for `Chain<T>`.
Before **Wednesday**: the rest of §6.14 — traversing with `begin()`/`end()`, the range-based `for` loop over a list, and `insert()`/`erase()` at an iterator position. Notice that one iterator interface drives every walk.
Before **Friday**: no new section — iterator *categories* (input → random-access), the separate `const_iterator` type, and L-values vs R-values are class material this floor. The textbook's §6.14 hands you the iterator; we build the rest on top of it.

Watch §6.14's animation of the list accessing elements through an iterator — the arrow advancing across the cells. Then picture the same arrow walking a vector versus a list — same interface, different storage. That is the abstraction you're building.

There are no pre-class videos. Class time is for live coding and discussion together — your reading is the prep.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | The iterator contract + `begin`/`end` | Read the iterator's five typedefs and its *provided* `operator*`/`operator->` (note the reference return). Live-code the stubbed operators: `operator++` (pre- and post-) and `operator==` (with `!=` derived). Wire `begin()` and `end()` on `Chain`. Write `for (const auto& s : hero.eventLog) std::cout << s << "\n";` on the projector — students see it Just Work. |
| **W** | `const_iterator` and `std::find_if`  | Live-code `Chain<T>::const_iterator` (its `operator++`/`==`; `*`/`->` provided) and `begin()/end() const`. Discussion: why a separate type, not `const iterator`. Then rewrite `findByName<T>` to use `std::find_if` (one line) — and watch the *same* `std::find_if` search the bestiary, the inventory, and the event log, because all satisfy the iterator contract. |
| **F** | Iterator categories + `operator--`   | Try `std::sort(chain.begin(), chain.end())` — it won't compile; read the error together and find the line about random-access / the missing `operator-`. (`std::sort(inventory.begin(), inventory.end())` *does* compile — the Bag's iterator is random-access.) Then implement `operator--` on both iterators so `std::reverse`, `rbegin`/`rend`, and `log --oldest` work. |

## The project — Floor 5

This week's project increment is **one `search` command that walks three containers without caring which**.

You will receive (in your starter drop):

- Everything through Floor 4½, fully working — `Bag<T>`, `Chain<T>` (doubly-linked, full Rule of Three), `BagException`, `findByName<T>`, every benchmark, the boss battle, `clone hero`.
- An upgraded `hero/Chain.h` with two nested types — `iterator`, `const_iterator` — whose movement/comparison operators and `begin()`/`end()` are stubbed for you to fill in. (`operator*`/`->`, `!=`, `cbegin`/`cend`, and `rbegin`/`rend` ship written.) The TODOs walk you through each operation in order.
- A `hero/Hero.h` whose `printLog` is now a function template taking a `(first, last)` iterator pair. The Floor 4½ `printLog`/`printLogOldest` pair is gone — `main.cpp` calls `printLog(eventLog.begin(), eventLog.end(), ...)` for newest-first and `printLog(eventLog.rbegin(), eventLog.rend(), ...)` for oldest-first. (`rbegin()`/`rend()` are *pre-wired* in the starter on top of `std::reverse_iterator`; they start working the moment your `operator--` does, on Friday.)
- A `main.cpp` with a new `search` command behavior: one call to `findByName` on the bestiary, one on the inventory, one on the event log — all using the *same* templated function, which itself uses `std::find_if` internally.
- A new `selftest iterator` harness that builds a `Chain<int>`, calls `std::find`, `std::distance`, and range-based-`for` on it, and reports pass/fail.

You will write:

1. **Monday:** `Chain<T>::iterator` — `operator++` (pre- and post-) and `operator==`. Then `begin()` and `end()` on `Chain`. (`operator*`/`->` and `!=` are provided.)
2. **Wednesday:** `Chain<T>::const_iterator`. Then `cbegin()` / `cend()`. Then rewrite `findByName<T>` to call `std::find_if` internally (one line). Then call it from `search` against all three containers.
3. **Friday:** Iterator categories. Try `std::sort` on a `Chain` iterator — it won't compile; paste the error into lab notes. Then note that `std::reverse(chain.begin(), chain.end())` *does* compile (your iterator claims bidirectional and `operator--` exists as a stub) but is broken until you finish it — `selftest iterator` phase 5 catches that. Implement `operator--` on `iterator` and `const_iterator` and watch `std::reverse`, `rbegin`/`rend`, and `log --oldest` come alive. Finally, run `selftest iterator` and confirm all phases pass.

Demo target (Friday):

```
> search Goblin
  Goblin   HP 8   ATK 2   weakness: fire
  (found in bestiary)
> search Iron key
  Iron key  (wt 0.1, val 0)
  (found in inventory)
> search began
  began session as "Aric"
  (found in event log)
> log
   1.  search began — found in event log
   2.  search Iron key — found in inventory
   3.  search Goblin — found in bestiary
   4.  began session as "Aric"
  (newest first; chain length 4)
> log --oldest 3
   1.  began session as "Aric"
   2.  search Goblin — found in bestiary
   3.  search Iron key — found in inventory
  (oldest first; chain length 4)
> selftest iterator
  range-for over Chain<int>: OK
  std::find(Chain<int>, 42): OK
  std::distance(begin, end): OK
  range-for over const Chain<int>&: OK
  std::reverse(Chain<int>) — first now == 9: OK
  all phases OK
> quit
  The lens dims. The lens does not remember what it saw — only how it moved.
```

### Lab 5 — *One Lens, Three Shelves* (folded into the project)

There is no separate lab handout. The work you do this week *is* the lab.

Commit `floor-05/lab-notes.md` to your project repo with the six items below.
Three of them ask you to break something on purpose and watch what happens —
restore your code after each one.

1. **The transcript.** A transcript of the demo above, pasted from your terminal.

2. **Plant the bug.** `hero/Chain.h` has two pre-increment `operator++`s. You want the one in `class iterator`, *not* the one in `const_iterator` — `log` walks a non-const chain, so it never calls the const one. Change its `p_ = p_->next` to `p_ = p_->prev`. Build, run two or three `search` commands so the chain holds several events, then run `log` and paste what happens. Run only `log`: once Friday's `operator--` is written, `selftest iterator` crashes while this bug is in. Restore.

3. **Skip `end()`.** Change the non-const `end()` — the one that returns `iterator` — to return an iterator wrapping `head_` instead of `nullptr`. Build, run a couple of `search` commands, then walk the non-empty chain two ways: `log` walks it from `begin()` to `end()`, and phase 1 of `selftest iterator` is a range-based `for` over a 100-element chain. Paste what both print. (The selftest's FAIL messages were written for Monday's stubs, so they will blame operators you did not touch. The cause is your `end()`.) Restore. In one sentence — what invariant of the iterator contract did the broken `end()` violate?

4. **The `std::sort` error.** In `main.cpp`, find the line near the top of `main()` that pushes `"began session as …"` onto `hero.eventLog`. Directly below it, add this line:

   ```cpp
   std::sort(hero.eventLog.begin(), hero.eventLog.end());
   ```

   Build — it fails. Paste the full compiler error. Find and quote the line that names an iterator category requirement. Visual Studio never writes "random access" in words: the category shows up only as the name of `std::sort`'s template parameter, `_RanIt`, in the *see reference to function template instantiation* notes (g++ spells it `_RandomAccessIterator`). Then, in one sentence: which operation is the standard library asking for that your iterator doesn't provide? Delete the line.

5. **`auto` vs spelled-out types.** The starter has no range-based loop over the event log, so write one first. The same spot in `main.cpp` you used for item 4 is a fine temporary home:

   ```cpp
   for (const auto& s : hero.eventLog) std::cout << s << "\n";
   ```

   Then write the same walk a second time as an explicit loop whose iterator type is spelled out — `Chain<std::string>::const_iterator` — starting at `cbegin()` and stopping at `cend()`. Both compile and print the same thing. Paste both loops into your notes; they don't need to stay in `main.cpp`. In two sentences: which version is easier to maintain when you later change the container type, and why?

6. **One-paragraph reflection.** You wrote two versions of `printLog` on Floor 4½ — one walking forward, one walking backward — and they were structurally identical except for three substitutions. (Look back at `printLog` and `printLogOldest` in your Floor 4½ `hero/Hero.cpp` and find the three before you write.) This week they collapsed into one function. State, in your own words, what abstraction the iterator type provides that lets that collapse happen.

Your commit history this week should show at least three commits — Mon (iterator + begin/end), Wed (const_iterator + std::find_if), Fri (categories + operator-- + lab notes).

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix the Opportunist offers yet another shortcut</span>

*Iterators!* Friend, who needs them? **Indexing works on EVERYTHING.** Just write `for (int i = 0; i < chain.size(); ++i) { chain[i]; }` — same thing, faster to type, looks just like vector code! Grix uses indexing on chains all the time. Programs run fine! Until — well, you'll see when the chain gets long. *Strange how it slows down. Must be the weather.*
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 5</p>

**The Quadratic Mire** — *HP: grows with the size of your container. Damage: invisible at small N, ruinous at large N.*
Strikes when a programmer reaches for indexed access on a container that doesn't support it cheaply. `chain[i]` on a linked chain is O(i) — to reach index `i` you walk from the head `i` times. Put that inside a `for (i = 0; i < n; ++i) chain[i]`, and the whole loop is O(n²) — the chain is walked once for index 0, twice for index 1, three times for index 2, and so on. On a 100-element chain you do 5,000 walks. On a 10,000-element chain you do 50,000,000. The program does not crash. It just gets slower in a way that scales with no obvious cause.

Counter by:

- **Use the iterator.** `for (auto it = chain.begin(); it != chain.end(); ++it)` is O(n) total — each `++it` is O(1), and you visit each node exactly once. The same loop on a `Bag<T>` is also O(n). Same code, same cost. That is the contract.
- **Range-based `for`** is iterator-based under the hood. `for (const auto& x : chain)` is the right default; reach for explicit iterators only when you need them (deletion, partial walks, comparing positions).
- **Read the iterator category before you reach for `[i]`.** Random-access iterators (`Bag`, `std::vector`, `std::array`, `std::deque` with caveats) make `[i]` cheap. Forward and bidirectional iterators (`Chain`, `std::list`, `std::forward_list`) do not. If a container's iterator can't even *be* random-access, indexing into it is a sign you're paying a hidden tax.
</div>

<div class="callout check" markdown="1">
<p class="callout-title">The Keeper's Check</p>

This floor ends the way every floor ends: with a short stack of paper questions in class — closed notes, closed laptop, answers in your own hand — drawn from everything this floor taught. If you did the reading, wrote the code, and can say *why* this week's work earns its place in the game, it takes ten minutes.
</div>

---

*Sister Vael turns the lens between her fingers without lifting it. The lens does not change its shape for any container; it never will. The corridor narrows again, descending. From below comes the sound of stacked plates settling — many of them, balanced one upon another. The stair to Floor 6 — the **Stacks** — opens.*
