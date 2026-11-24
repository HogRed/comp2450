---
title: The Chain Vault, Deeper
floor_number: Floor 4½
order: 70
flavor_quote: "A chain that runs one way is a leash. A chain that runs two ways is a record."
week: 6
zybook: "Chapter 6 (continued) — class focus §6.6–6.9 (doubly-linked lists)"
---

## The briefing

The stair from the upper vault doesn't lead down so much as *further in*. The lockboxes here hang differently — each one cuffed to the next *and* the previous, two iron rings instead of one. Mavren waits at the entrance with a second coil of chain across her other shoulder.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/mavren.svg' | relative_url }}" alt="Quartermistress Mavren, Keeper of the Chain Vault — silver-streaked dark hair braided through iron rings, slate-grey tunic under a chainmail half-apron, a coiled iron chain crossing from her right shoulder down to her belt, a brass tally-tag on a thin chain at her collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Quartermistress Mavren, the Deeper Vault</span>

Last week I taught you a chain that runs *one* way. You walked it from the head. You could not walk it back. And when I asked you to *copy* a chain — you refused, because the only honest answer you had was *delete the request*. This week you give the chain a second ring on every link, and you give yourself the right answer to *copy*. When the vault is asked to hand over its records — and it will be — you will not hand over the *same chain*. You will forge a fresh one, link for link, and let the original stay where it is.
</div>
</div>

Last week your `Chain<T>` was **singly-linked**: each node had `data` and a `next` pointer, plus a `head_` on the chain itself. Walking forward was cheap. Walking *backward* was impossible without rebuilding the whole list, and `push_back` was O(n) because you had to walk to the tail every time. And `Chain(const Chain&)` was `= delete` — you could not copy a chain at all.

This week you fix both. You promote `Chain<T>` to a **doubly-linked list**, and you write the **deep copy** that Floor 4 promised and refused.

You will learn two things, and they tie last week's loose ends together:

1. **The `prev` pointer and the `tail_` handle.** Every node now points both ways. The chain now has a `head_` *and* a `tail_`. You can walk in either direction. `push_back` becomes O(1) — the same constant-time prepend you had on the front, now available on the back. Pop from either end is O(1) too. The price: every operation that touches a link has to keep *both* pointers consistent. Get one wrong and the chain silently breaks in the middle.
2. **Deep copy — closing the Rule of Three.** When `Chain<T> b = a;` runs, `b` must end up with its *own* nodes that hold the same data. Not shared nodes. Not the same `head_`. *Its own chain.* You walk the source from head to tail and `push_back` each value into the destination. When `a` and `b` both go out of scope, each destructor walks and deletes *its own* nodes. Zero shared memory. Zero double-frees.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/doubly-linked-anatomy.svg' | relative_url }}" alt="A row of three nodes. Each node has prev on the left, data in the middle, next on the right. A 'head' arrow points into the leftmost node's prev (which is nullptr). A 'tail' arrow points into the rightmost node's next (which is nullptr). Adjacent nodes are connected by paired arrows: one going right (next) and one going left (prev)."/>
  <figcaption>Doubly-linked: every node has a <code>prev</code> and a <code>next</code>. The chain has a <code>head_</code> <em>and</em> a <code>tail_</code>. The first node's <code>prev</code> is <code>nullptr</code>; the last node's <code>next</code> is <code>nullptr</code>. Two ground symbols, one at each end.</figcaption>
</figure>

<figure class="diagram">
  <img src="{{ '/assets/diagrams/shallow-vs-deep-copy.svg' | relative_url }}" alt="Two chain diagrams side by side. Left: shallow copy — two chain handles labelled a and b both have arrows pointing into the same row of three nodes. A red 'double-free' warning glyph hangs over the nodes. Right: deep copy — chain handle a points into one row of three nodes; chain handle b points into a parallel row of three nodes that hold the same values but are physically separate."/>
  <figcaption>Shallow copy (left) is what the compiler-generated copy constructor would give you: two <code>Chain</code> handles, one set of nodes, two destructors fighting over the same memory. Deep copy (right) is what you write: two handles, two physically separate rows of nodes carrying equal values. Each destructor cleans up only what it owns.</figcaption>
</figure>

You are not throwing away last week's `Chain<T>` — you are growing it. The event log still works the same way from `main.cpp`'s perspective (`push_front` for newest-first), but now `log --oldest 5` walks the chain backward from the tail, and `Hero` can be *copied* without the compiler refusing.

## Objectives

By the end of Floor 4½ you will be able to:

- Add a `prev` pointer to `Node<T>` and a `tail_` pointer to `Chain<T>` and explain what invariant ties `head_->prev`, `tail_->next`, and an empty chain together.
- Implement `push_back` in O(1) and explain why it could not have been O(1) without `tail_`.
- Implement `pop_front` and `pop_back` in O(1) — and for `pop_back`, explain why a *singly*-linked list with only a `tail_` pointer still couldn't pop from the back in O(1).
- Walk the chain backward with `for (const Node<T>* p = chain.tail(); p != nullptr; p = p->prev) ...` and state which steps differ from the forward walk.
- Implement `Chain(const Chain& other)` as a deep copy that walks the source and `push_back`s each value into a fresh chain.
- Implement `operator=(const Chain&)` using the **copy-and-swap idiom** — or hand-rolled with `clear()` + copy — and explain why naive assignment (just copying `head_` and `tail_`) is wrong in the same way the compiler-generated version was.
- State the **Rule of Three** in full, and tick all three boxes on your own `Chain<T>`.

## Pre-class

### Reading (ZyBook Ch. 6, continued)

**The whole of Chapter 6 was assigned back on Floor 4.** If any of it is still unread — especially the doubly-linked and iterator sections we hadn't reached in class yet — catch up now. The pointers below are what *this week's* class leans on.

Before **Monday**: §6.6 *Doubly-linked lists* — each node now carries a `prev` as well as a `next`, and the list keeps both a head and a tail.
Before **Wednesday**: §6.7 *Doubly-linked lists: Insert* and §6.8 *Doubly-linked lists: Remove* — the four-pointer surgery (two writes on each side) that two-way links demand.
Before **Friday**: §6.9 *Linked list traversal* (forward *and* reverse — the reverse walk is exactly what `prev` buys you) and §6.12 *Linked lists: Recursion*. The full Rule of Three (destructor, copy, move) your `Chain<T>` needs is class material; the textbook doesn't section it.

The §6.6 animation is the picture you want in your head when you write `push_back`. Watch what changes about `tail_` and what changes about the previous tail's `next`. There are two writes, not one.

There are no pre-class videos. Class time is for live coding and discussion together — your reading is the prep.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | `prev`, `tail_`, two-way walk | Live-code the upgrade from `Node { T; Node* next; }` to `Node { T; Node* prev; Node* next; }`. Add `tail_` to `Chain`. Rewrite `push_front` to also wire `prev`, and write `push_back`. Walk the chain backward on the projector. Add a `log --oldest <n>` flavor of the existing command. |
| **W** | Deep copy ctor          | Live-code `Chain(const Chain& other)`. Plant the bug — start with the shallow copy the compiler would have written — and watch the sanitizer / Node counter catch the double-free. Then write the real one. Discussion: why is the source `const`-correct walk easier with `head_` than with `tail_`? |
| **F** | `operator=` + Rule of Three recap | Live-code copy assignment using `clear()` + copy loop. Compare with copy-and-swap as a one-liner. Show the program copying a `Chain<int>` of 1,000 elements with allocations and deallocations both at 1,000. Read the Rule of Three off the whiteboard one last time before we leave this material for iterators. |

## The project — Floor 4½

This week's project increment is **a chain you can walk in both directions and a `Hero` you can copy**.

You will receive (in your starter drop):

- Everything through Floor 4, fully working — `Bag<T>`, `Chain<T>` (singly-linked, with deleted copy ops), the event log, the boss battle, `selftest chain`.
- An upgraded `hero/Chain.h` whose `Node` now has a `prev` pointer and whose `Chain` has a `tail_` member. The single-pointer Floor 4 methods are still there but several have **stubs that don't yet wire `prev`** — that's intentional; you'll fix them.
- Signatures for `push_back`, `pop_front`, `pop_back`, `tail()`, and the new copy ctor and copy assignment. Each is a TODO.
- A `main.cpp` with two new commands: `log --oldest <n>` (walks from `tail()` backward, calling your new accessor) and `clone hero` (constructs a `Hero` copy in a local scope and prints both event logs side-by-side before the copy is destroyed).
- An expanded `selftest chain` harness that copies a 1,000-element chain, lets the copy die, and asserts that the original is still walkable in both directions and the allocation/deallocation counts balance.

You will write:

1. **Monday:** Add `prev` to `Node`; add `tail_` to `Chain`. Rewire `push_front` to set `prev`. Implement `push_back`, `tail()`, and the backward walk used by `log --oldest`. Update the destructor — it still walks via `next`, but now you delete a node whose `prev` you no longer need (a useful one-line consistency check: assert `tail_->next == nullptr` in debug builds).
2. **Wednesday:** Implement `Chain(const Chain& other)` as a deep copy. Start by deliberately writing the shallow version (`head_ = other.head_; tail_ = other.tail_; size_ = other.size_;`) and running `selftest chain` — read the sanitizer/counter output, *then* delete it and write the deep copy.
3. **Friday:** Implement `operator=(const Chain& other)`. Either use copy-and-swap (`Chain tmp(other); swap(*this, tmp);`) or the explicit form (`if (this == &other) return *this;` first, then `clear();` and, for each node in `other`, `push_back(node->data);`). Whichever you choose, defend it in your lab notes. Implement `pop_front` and `pop_back`. Run `selftest chain` one last time and confirm a clean balance.

Demo target (Friday):

```
> search Goblin
  Goblin   HP 8   ATK 2   weakness: fire
> inventory
  1.  Rusty sword       (wt 4.0, val 5)
  2.  Healing potion    (wt 0.5, val 12)
  3.  Iron key          (wt 0.1, val 0)
  4.  Loaf of bread     (wt 0.1, val 1)
  5.  Cloak of shadows  (wt 1.5, val 80)
> inspect 99
  No such item. (index 98 out of bounds for size 5)
> log --oldest 5
  1.  began session as "Aric"
  2.  search Goblin — found in bestiary
  3.  inventory — listed 5 items
  4.  error: index 98 out of bounds for size 5
  (oldest first; chain length 4)
> clone hero
  -- original log (newest first) --
  1.  error: index 98 out of bounds for size 5
  2.  inventory — listed 5 items
  3.  search Goblin — found in bestiary
  4.  began session as "Aric"
  (newest first; chain length 4)
  -- cloned log (newest first) --
  1.  error: index 98 out of bounds for size 5
  2.  inventory — listed 5 items
  3.  search Goblin — found in bestiary
  4.  began session as "Aric"
  (newest first; chain length 4)
  (clone is being destroyed now)
  (clone destroyed; original event log still has 4 entries — try `log 3`)
> log 3
  1.  clone hero — copy lived and died
  2.  error: index 98 out of bounds for size 5
  3.  inventory — listed 5 items
  (newest first; chain length 5)
> selftest chain
  Phase 1 (single chain)
    allocations:  1000   deallocations:  1000   leaked:     0   OK
  Phase 2 (deep copy)
    original after copy died — forward walk:  1000   backward walk:  1000
    copy before death        — forward walk:  1000   backward walk:  1000
    allocations:  2000   deallocations:  2000   leaked:     0   OK
> quit
  The forge cools. Two chains dissolve, each by its own hand.
```

### Lab 4½ — *Both Directions, Twice the Iron* (folded into the project)

There is no separate lab handout. The work you do this week *is* the lab.

Commit `floor-04h/lab-notes.md` to your project repo with:

1. A transcript of the demo above.
2. **Break a `prev` pointer on purpose.** In `push_back`, omit the line that sets `new_node->prev = tail_;`. Run `log --oldest 5`. Describe exactly what gets printed — and explain in one sentence which step of the backward walk reads the bad pointer. Restore the line.
3. **Try the shallow copy.** Replace your `Chain(const Chain&)` body with a literal field-by-field copy: `head_ = other.head_; tail_ = other.tail_; size_ = other.size_;`. Run `selftest chain`. Paste the failure. Which exact line of which destructor was the second `delete` on the same node? Restore the deep copy.
4. **Compare copy-and-swap with the explicit form.** Implement `operator=` both ways on a branch. Paste both functions. In two sentences: which one are you more confident you can write correctly under exam pressure, and why?
5. **Why doesn't a singly-linked list with a `tail_` pointer give you O(1) `pop_back`?** Answer in two sentences. (Hint: after you delete the tail, what has to point to `nullptr`, and what would it take to find it?)
6. **One-paragraph reflection.** You now have both halves of the Rule of Three implemented. State the rule in one sentence. Then: when would you reach for the **Rule of Zero** instead — and why does `Chain<T>` not qualify?

Your commit history this week should show at least three commits — Mon (doubly-linked + push_back), Wed (deep copy ctor), Fri (copy assignment + pop_front/pop_back + lab notes).

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix the Opportunist offers another clever shortcut</span>

*Friend, friend!* You want to copy a chain? **Just copy the head pointer!** One line! `b.head_ = a.head_;` — done. Now you have *two* chains. Free chain! Grix gets two chains for the price of one all the time. Works perfect. Until — well. *Both go out of scope.* But by then, *Grix has left the building.*
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 4½</p>

**The Twin-Owner** — *HP: appears at end-of-scope. Damage: catastrophic, in pairs.*
Strikes when a programmer writes a class that owns memory and then accepts the compiler's default copy constructor — or, worse, writes one that copies pointers without copying what they point to. The program runs fine for a long time. Then a function returns by value, or a `vector<Chain>` resizes, or someone writes `auto b = a;`. Two objects now believe they own the same nodes. The first destructor runs and deletes them. The second destructor runs and deletes them *again*. On a good day the program crashes immediately. On a bad day it crashes a week later in production, in a place that has nothing to do with chains.

Counter by:

- **Rule of Three.** If you wrote a destructor, write the copy constructor and copy assignment *too* — or `= delete` them. Pick one. Never leave the compiler to guess.
- **Deep copy means deep.** Walk the source. Allocate every node yourself. Never write `head_ = other.head_;` in a copy constructor that owns memory. That line is the bug.
- **Copy-and-swap.** When in doubt, `Chain tmp(other); swap(*this, tmp);`. The temporary's destructor cleans up the old contents; you inherit the freshly-built copy. Three lines, exception-safe, hard to get wrong.
- **Rule of Zero.** If your class owns nothing raw — only `std::vector`, `std::string`, `std::unique_ptr` — write **none** of the special members. The compiler's defaults are correct. `Chain<T>` doesn't qualify because it owns `Node*` directly. `Bag<T>` *does* qualify, which is why Floor 3 never made you write a destructor for it.
</div>

<div class="callout check" markdown="1">
<p class="callout-title">The Keeper's Check</p>

This floor ends the way every floor ends: with a short stack of paper questions in class — closed notes, closed laptop, answers in your own hand — drawn from everything this floor taught. If you did the reading, wrote the code, and can say *why* this week's work earns its place in the game, it takes ten minutes.
</div>

---

*Mavren cuffs the two new rings onto your chain — one on each end — and lets it hang. Both ends touch the floor now. The vault extends still further, but the next stair leads somewhere else entirely: to a tower where the same spell walks `Bag`, `Chain`, and things you have not yet built — without caring which is which. The stair to Floor 5 — the **Eye of Scrying** — opens.*
