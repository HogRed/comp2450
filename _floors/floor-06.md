---
title: The Hall of Echoes
floor_number: Floor 6
order: 90
flavor_quote: "Push, and the hall remembers. Pop, and the hall forgets in the reverse order it learned. There is no other order."
week: 8
zybook: "Chapter 7 — full chapter; class focus §7.1–7.4 (stacks)"
---

## The briefing

Past Sister Vael's chamber, the corridor opens into a vast, low-ceilinged hall whose walls are tiered like the inside of a bell. Every step you take echoes back, then again, then again — but not in the order you made them. The hall serves them up *last shout first*.

A figure in faded choirmaster's robes stands at a lectern in the centre, eyes half-closed, hands folded.

"I am an adventurer," you call. Then, after a nervous pause: "Hello?"

The choirmaster does not move. His lips do not part.

From the walls comes a soft reply: "Hello."

Then, a moment later: "Adventurer."

The hall has handed your words back in reverse.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/resh.svg' | relative_url }}" alt="Cantor Resh, Keeper of the Hall of Echoes — gaunt elderly man in faded grey choirmaster's robes with frayed silver embroidery at the cuffs, deep-set eyes nearly closed, thin silver hair pulled back, hands folded on a stone lectern, a small brass tuning fork on a leather cord at his belt."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Cantor Resh, Keeper of the Hall of Echoes</span>

I do not remember my own name. I remember the *last* thing I was told. Tell me yours, and I will know it until the next person tells me theirs — and then I will know that one. The hall is the same. *Push* a thing into it: the hall remembers. *Pop* a thing out: the hall returns what you most recently gave it. Never what was oldest. Never what was middle. Only what was last. — This is not a flaw. This is the *only* arrangement that makes the next problem solvable. Undo. Backtracking. The call your program is in the middle of. Each is a stack. Each waits for the most recent thing to finish before the one beneath it can speak.
</div>
</div>

For five floors now you have built containers that hold things and let you ask about *all* of them — sort the bag, walk the chain, find by name. The hall is not interested in *all*. The hall is interested in *the most recent*. Once you accept that constraint you discover, to your surprise, that an astonishing number of programs only need the most recent.

This week you build a `Stack<T>` — the **L**ast-**I**n, **F**irst-**O**ut container. You will build it in five lines of code. That is not a typo. A stack is not really a data structure; it is a *promise*: whatever you `push` last is what `top` returns and `pop` removes. Any container that lets you add and remove from one end in O(1) can keep that promise — your `Chain<T>` from Floor 4½, a `std::vector`, even a `Bag<T>` if you don't mind the trade-offs. So we will not invent storage this week. We will *adapt* the storage you already have.

You will learn three things this week, and they layer on top of each other:

1. **An ADT is not a data structure.** A *list* is an ADT (the contract: an ordered sequence). A *stack* is an ADT (the contract: LIFO). `std::vector` is a data structure that *implements* the list ADT. `Chain<T>` is another. Your `Stack<T>` will implement the stack ADT — by *delegating every operation* to a `Chain<T>` underneath. Three nouns, two layers, one tiny class.

2. **The adapter pattern.** `Stack<T>` will be five one-line methods — four you'll write (`push`, `pop`, `size`, `empty`) plus `top()`, which ships done — each one forwarding to a `Chain<T>` method. `push` becomes `chain_.push_front`. `pop` becomes `chain_.pop_front`. `top` becomes `chain_.head()->data`. The brevity is the lesson — and it is the exact pattern `std::stack` uses in the standard library (adapter over `std::deque` by default). You are recreating a real library move.

3. **What a stack is *for*.** Three classical use-cases land this week. *Bracket balancing:* a syntax check that needs to remember "the most recent opener I haven't closed yet." *Undo:* a record of "the most recent change I might want to reverse." *Recursion as an explicit stack:* every function call your CPU executes is pushed onto a stack of return frames; if you write the same algorithm iteratively with your *own* `Stack`, you make the implicit explicit.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/stack-lifo.svg' | relative_url }}" alt="A vertical column of three boxes labeled bottom to top: 'A', 'B', 'C'. An arrow labeled 'push' enters from the right above the column; an arrow labeled 'pop' exits from the right above the column. A pointer labeled 'top' points at 'C', the topmost box. To the right, a sequence diagram: push A, push B, push C, pop → C, pop → B, pop → A. Caption emphasises that push and pop both happen at the same end."/>
  <figcaption>A stack is one rule: <em>push and pop both touch the same end.</em> The <code>top</code> is whichever element you pushed most recently. Pop it and the new top is the one before it.</figcaption>
</figure>

<figure class="diagram">
  <img src="{{ '/assets/diagrams/stack-as-adapter.svg' | relative_url }}" alt="Two stacked boxes. The outer box is labeled 'Stack<T>' and contains five method names: push, pop, top, size, empty. The inner box is labeled 'Chain<T>' and contains the corresponding methods: push_front, pop_front, head(), size, empty. Arrows connect each Stack method to its Chain delegate. A label at the top reads 'ADT (the promise)'; a label at the bottom reads 'data structure (the storage)'."/>
  <figcaption>Adapter pattern: <code>Stack&lt;T&gt;</code> is a thin shell that forwards every operation to a <code>Chain&lt;T&gt;</code> underneath. The ADT lives on the outside; the storage lives on the inside; the five method bodies are the bridge (you write four; <code>top()</code> ships done). <code>std::stack</code> does the exact same thing — by default it adapts <code>std::deque</code>.</figcaption>
</figure>

## Objectives

By the end of Floor 6 you will be able to:

- State the LIFO contract in one sentence and identify which end of the underlying storage each stack operation touches.
- Distinguish an **ADT** from a **data structure** by example: name two data structures that could implement the stack ADT, and explain why both can keep the same promise despite different internals.
- Implement `Stack<T>` as an adapter over `Chain<T>` — four one-line method bodies (`push`, `pop`, `size`, `empty`; `top()` ships written — a reference-returning `T&` can't be honestly stubbed) — and explain *why* the same Stack would also work over `std::vector` or `Bag<T>` (with which trade-off).
- Use a `Stack<char>` to check that brackets, parens, and braces are balanced in a string — the classical exercise that introduces stacks in every textbook for a reason.
- Implement an `undo` command for the dungeon game by snapshotting state onto a `Stack<UndoAction>` before every mutating command and restoring on pop.
- Recognize the *implicit* stack inside every recursive function and (optionally, Friday lab) rewrite one recursive function from an earlier floor as an iterative version with an explicit `Stack`.
- Read a compile error caused by calling a non-existent method on `Stack<T>` (say, `Stack::operator[]`) and explain what the ADT *deliberately omits* and why.

## Pre-class

### Reading (ZyBook Ch. 7 — the whole chapter)

**Read all of Chapter 7.** The reading assignment is the entire chapter — including the queue sections we won't reach in class until next floor. Reading them now means next week is review, not a cold start. The day-by-day pointers below are what *this week's* class (stacks) leans on.

Before **Monday**: §7.1 *Stack abstract data type* — push, pop, top, and the LIFO contract; and §7.3 *Stacks using linked lists* — a stack built on a linked list, which is exactly this week's adapter over your `Chain<T>`.
Before **Wednesday**: §7.2 *C++ stack class* — the standard library's own `std::stack`, the adapter pattern you're recreating; and §7.4 *Array-based stacks* — the same stack ADT on a different container, so you see why the adapter doesn't care what's underneath.
Before **Friday**: no new stack section to read — undo, and "a recursive call is an implicit stack," are class material. For the textbook's classic stack application, skim §7.10 *LAB: Calculator*, where a stack evaluates an expression (the same shape as Wednesday's bracket check).

Skim the ZyBook §7.1 animation that pushes three letters onto a stack and pops them off — the LIFO contract takes ninety seconds to absorb visually and saves you twenty minutes of confusion in code.

There are no pre-class videos. Class time is for live coding and discussion together — your reading is the prep.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | `Stack<T>` as adapter            | Live-code the four stubbed method bodies on `Stack<T>` together: push → `chain_.push_front`, pop → `chain_.pop_front`, size and empty → delegate. (`top()` → `chain_.head()->data` *ships written* — a reference-returning `T&` can't be honestly stubbed; read it together and note the reference return.) Run `selftest stack` — all phases pass. Then on the projector: `std::stack<int> s; s.push(1); s.push(2); std::cout << s.top();` — point out that you have just recreated the standard library. |
| **W** | Bracket balancing                | Live-code `isBalanced` in `hero/Lint.cpp` using `Stack<char>`. Walk through `({a} (b) [c])`, pushing and popping aloud. Then try `({)}` and watch the algorithm catch the mismatch on the very first closer. Discuss: this is the *first* real use of a stack most programmers ever write. The compiler used one to parse the file you're typing in. |
| **F** | Undo + recursion as a stack       | Live-code the `undo` dispatcher in `main.cpp`. Run `take Lantern; sort inventory by weight; undo; undo` — the inventory walks backward through your history. Then a board exercise: take Floor 1's `binarySearchRecursive`, draw the call stack on the board frame by frame, and rewrite the function iteratively with an explicit `Stack<Range>`. (No checked-in code required — the demo *is* the lesson.) |

## The project — Floor 6

This week's project increment is **a `Stack<T>` adapter and three things you do with it**: a `lint` command that checks bracket balance, an `undo` command that reverses the most recent inventory change, and a `selftest stack` harness that proves the LIFO contract holds.

You will receive (in your starter drop):

- Everything through Floor 5, fully working — `Bag<T>`, `Chain<T>` with `iterator` / `const_iterator`, `BagException`, `findByName<T>` via `std::find_if`, every benchmark, the boss battle, `clone hero`, the `log` and `log --oldest` commands.
- A new `hero/Stack.h` with **four method bodies stubbed** — `push`, `pop`, `size`, `empty`. (`top()` ships written — a reference-returning `T&` can't be honestly stubbed.) The TODOs name each stubbed method's one-line implementation in comment form.
- A new `hero/UndoAction.h` defining the small struct that gets pushed onto `hero.undoStack`: a `description` string and a `Bag<Item> inventorySnapshot`.
- A new `hero/Lint.h`/`Lint.cpp` declaring `bool isBalanced(const std::string&)`. The body is **stubbed** — your Wednesday work.
- A new `hero/StackTests.cpp` harness — `selftest stack` exercises `push`, `pop`, `top`, `size`, `empty` across five phases with diagnostic output on FAIL.
- A modified `hero/Hero.h` that now owns a `Stack<UndoAction> undoStack` field.
- A modified `main.cpp` with three new commands wired in: `take <name>` (which records an undo snapshot), `undo` (whose dispatcher body is **stubbed** — your Friday work), and `lint <text>`. The existing `sort inventory by <key>` now also records an undo snapshot.

You will write:

1. **Monday:** `Stack<T>`'s four stubbed method bodies in `hero/Stack.h` — `push`, `pop`, `size`, `empty` (`top()` is already provided). Each is one line. After this, `selftest stack` passes every phase, and *every* mutating command in `main.cpp` starts recording undo snapshots correctly (you just won't be able to *use* them yet).

2. **Wednesday:** `isBalanced` in `hero/Lint.cpp`, using your `Stack<char>`. After this, `lint <text>` returns real answers, not the stub `not balanced` for everything.

3. **Friday:** The `undo` dispatcher in `main.cpp`. Five lines: bind a reference to `hero.undoStack.top()`, copy its `inventorySnapshot` over `hero.inventory`, print the description, push an event-log entry, then pop. After this, `take Lantern; undo` round-trips your inventory.

Demo target (Friday):

```
> selftest stack
  empty Stack<int>: size==0 && empty(): OK
  push(7): size==1 && top()==7: OK
  push 1,2,3: size==3 && top()==3 (LIFO): OK
  after pop(): size==2 && top()==2: OK
  drain to empty: size==0 && empty(): OK
  all phases OK

> lint ({a} (b) [c])
  balanced
> lint ({)}
  not balanced
> lint hello (world
  not balanced

> inventory
   1.  Rusty sword       (wt 4.0, val 5)
   2.  Healing potion    (wt 0.5, val 12)
   3.  Iron key          (wt 0.1, val 0)
   4.  Loaf of bread     (wt 0.1, val 1)
   5.  Cloak of shadows  (wt 1.5, val 80)
> take Lantern
  Taken: Lantern  (wt 0.1, val 1)
> sort inventory by weight
   1.  Iron key          (wt 0.1, val 0)
   2.  Loaf of bread     (wt 0.1, val 1)
   3.  Lantern           (wt 0.1, val 1)
   4.  Healing potion    (wt 0.5, val 12)
   5.  Cloak of shadows  (wt 1.5, val 80)
   6.  Rusty sword       (wt 4.0, val 5)
> undo
Undid: sort inventory by weight
> inventory
   1.  Rusty sword       (wt 4.0, val 5)
   2.  Healing potion    (wt 0.5, val 12)
   3.  Iron key          (wt 0.1, val 0)
   4.  Loaf of bread     (wt 0.1, val 1)
   5.  Cloak of shadows  (wt 1.5, val 80)
   6.  Lantern           (wt 0.1, val 1)
> undo
Undid: take Lantern
> inventory
   1.  Rusty sword       (wt 4.0, val 5)
   2.  Healing potion    (wt 0.5, val 12)
   3.  Iron key          (wt 0.1, val 0)
   4.  Loaf of bread     (wt 0.1, val 1)
   5.  Cloak of shadows  (wt 1.5, val 80)
> undo
Nothing to undo.
> quit
The cantor falls silent. The last echo decays. The hall is quiet.
```

### Lab 6 — *The Last Word First* (folded into the project)

There is no separate lab handout. The work you do this week *is* the lab.

Commit `floor-06/lab-notes.md` to your project repo with:

1. A transcript of the demo above (with your own hero name).

2. **The ADT-vs-DS one-pager.** In two short paragraphs: define *abstract data type* and *data structure* in your own words, then state which one `Stack` is and which one `Chain` is. End with one sentence on why `Stack<T>` as an adapter over `Chain<T>` reinforces the distinction.

3. **The cheap swap.** In `hero/Stack.h`, swap the storage underneath your stack from `Chain<T>` to `std::vector<T>`. Six small edits, all in that one file: `#include "Chain.h"` → `#include <vector>`; `Chain<T> chain_;` → `std::vector<T> chain_;`; `push_front` → `push_back`; `pop_front` → `pop_back`; and `head()->data` → `back()` in *both* `top()` overloads. Leave `size()` and `empty()` alone. Build, run `selftest stack`, and paste the output. Then run `benchmark log 1000` (the Floor 4 prepend bench — give it the `1000`; without a number it runs a sweep that takes many minutes in a Debug build). Paste that too. Does the benchmark change? Why or why not? *(Hint: open `hero/ChainBench.cpp` and look for the word `Stack`.)* Restore `Stack.h` when you're done.

4. **An empty stack call.** Paste this block at the very top of `main()` in `main.cpp`, right after `int main() {`:

   ```cpp
   {   // LAB 6 ITEM 4 -- temporary
       Stack<int> s;
       std::cout << "about to call top() on an empty stack...\n";
       std::cout << "top() = " << s.top() << "\n";
       std::cout << "survived\n";
   }
   ```

   Build and run. Paste what you see — the console text, the exit code Visual Studio prints, or the debugger's message. Then guard it: replace the `top()` line with `if (!s.empty()) std::cout << "top() = " << s.top() << "\n"; else std::cout << "stack is empty\n";` and run again. In one sentence: what does the stack ADT *not* promise (a queue, a list, and a map don't promise it either), and what should callers always do? Delete the block when you're done.

5. **One recursive function, one stack.** Here is Floor 1's recursive binary search (from `bestiary/Search.cpp`), rewritten to search a `std::vector<int>`. Paste it into `main.cpp` *above* `int main()`, along with a `Range` struct:

   ```cpp
   int binSearchRec(const std::vector<int>& v, int target, int lo, int hi) {
       if (lo >= hi) return -1;
       int mid = lo + (hi - lo) / 2;
       if (v[mid] == target) return mid;
       if (v[mid] <  target) return binSearchRec(v, target, mid + 1, hi);
       return                       binSearchRec(v, target, lo,      mid);
   }

   struct Range { int lo, hi; };
   ```

   Now write `int binSearchStack(const std::vector<int>& v, int target)` right below it — the same algorithm, with no recursion, using a `Stack<Range>`. The shape: push the starting range `{0, (int)v.size()}`; then, while the stack isn't empty, read the `top()`, `pop()` it, and do what one recursive call would have done — except where the recursive version *calls itself*, you `push` the new range instead. Test both at the top of `main()`:

   ```cpp
   {   // LAB 6 ITEM 5 -- temporary
       std::vector<int> v = {2, 5, 8, 13, 21, 34, 55};
       for (int t : {2, 13, 55, 4, 99}) {
           std::cout << t << ": recursive=" << binSearchRec(v, t, 0, (int)v.size())
                     << "  stack=" << binSearchStack(v, t) << "\n";
       }
   }
   ```

   Paste your `binSearchStack` and the output (both columns should match). In two sentences: what does the explicit-stack version *show* that the recursive one *hides*? (Bonus: what is the most `Range`s your stack ever holds at once, and why?) Delete the temporary code when you're done.

6. **One-paragraph reflection.** You wrote four one-line methods this week (`top()` came written). What did you spend the rest of class time on, and why was that the right ratio? Compare to Floor 4, when you wrote a destructor, a copy ctor, an operator=, and a Rule-of-Three discussion in roughly the same amount of class time.

Your commit history this week should show at least three commits — Mon (`Stack<T>` + `selftest stack` passes), Wed (`isBalanced` + `lint` returns real answers), Fri (`undo` dispatcher + lab notes).

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix the Opportunist offers yet another shortcut</span>

*Empty check?* Friend, that's just *more code.* Just call `top()` and see what comes out! If the stack is empty, the program will tell you. Grix never checks. **Grix has run plenty of programs.** Sometimes they print weird numbers. Sometimes they crash. Sometimes — *most times!* — they're fine. Most times.
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 6</p>

**The Hollow Echo** — *HP: zero. Damage: silent corruption, then a crash with no useful stack trace.*
Strikes when code calls `top()` or `pop()` on an empty stack. The stack ADT *does not promise* anything about those calls — `top()` of an empty stack is undefined behaviour, full stop. In a debug build you might get an assertion. In a release build, on a Stack-over-Chain implementation, you get a dereference of `head_`, which is `nullptr` — a segfault if you're lucky, garbage data if you aren't. The program crashes far from the actual mistake; the stack trace points at the dereference, not at the missing `empty()` check upstream.

Counter by:

- **Always guard with `empty()` first.** `if (!stack.empty()) { auto x = stack.top(); ... }`. This is not paranoia; it is the contract talking.
- **Or use a wrapper that throws.** Some stacks (not yours, not `std::stack`) make `top()`-on-empty throw an exception. Either contract is defensible — what is *not* defensible is "assume the stack is non-empty without checking."
- **Don't conflate `pop()` with `peek-and-remove`.** Our `pop()` (and `std::stack::pop()`) returns `void`. You read the top *with* `top()`, *then* call `pop()` to discard it. Trying to `pop()` an empty stack is just as undefined as reading its top.
- **The Hollow Echo and Floor 3's BagException are siblings.** Floor 3 taught you to bounds-check at the trust boundary; this floor extends that to *any* operation whose precondition the caller can violate. Empty-stack-`top()` is exactly that pattern at one level higher.
</div>

<div class="callout check" markdown="1">
<p class="callout-title">The Keeper's Check</p>

This floor ends the way every floor ends: with a short stack of paper questions in class — closed notes, closed laptop, answers in your own hand — drawn from everything this floor taught. If you did the reading, wrote the code, and can say *why* this week's work earns its place in the game, it takes ten minutes.
</div>

---

*Cantor Resh, at his lectern, repeats your last word once more and then is silent. The hall's echoes catch up, settle, and fade in reverse. Beyond the lectern a wide arch opens onto a corridor whose floor is striped with painted tiles — red, then blue, then red, then blue — leading down toward the sound of slow water. At the far end, beyond a long single-file line of waiting petitioners, a clerk-monk in a soaked grey robe sits behind an ironwood desk and does not look up. **Steward Quill** keeps **The Cisterns**. He sees petitioners in the order they arrived. He does not see them in any other order. The stair to Floor 7 — the **Queues** — opens.*
