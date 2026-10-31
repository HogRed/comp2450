---
title: The Balanced Hall
floor_number: Floor 9½
order: 140
flavor_quote: "A tree leans the way it was planted. It takes a hand the tree cannot feel to set the branches level again — and the line, once short, stays short."
week: 12
zybook: "Chapter 10 (Balanced Trees) — class focus §10.1–10.4 (AVL trees & rotations)"
---

## The briefing

The chart in the next room is the same height as the doorway, and it should not be. You built McCown's catalogue last floor as a binary search tree, fast and self-sorting — and then you watched it betray that promise: feed it names in order and it grows downward in a single leaning line, a *stick*, until finding the last beast costs exactly as many questions as reading the old flat list. The rule held. The speed did not.

McCown is here, and he is not at peace with it. He stands before a chart that lists to one side like a ship taking water, and he keeps reaching up to straighten a branch that will not stay straight. "Order is not enough," he says, more to the chart than to you. "A thing can be perfectly sorted and perfectly *slow*." Behind him, in the dark above the chart, something moves — a counterweight, a pulley, a hand — and a whole limb of the tree swings up and over and settles, and the chart stands a little straighter than before. The names did not change places in the *line*. Only in *height*.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/mccown.svg' | relative_url }}" alt="Scrivener McCown, Keeper of the Bestiary — an aging scholar-monk in grey robes with ink-stained fingers and spectacles, reaching up to straighten one branch of a tall classification chart that leans badly to one side."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Scrivener McCown, Keeper of the Bestiary</span>

I gave the tree a rule and it gave me back a *list*. Do you see it lean? Every name I added was already in order, so every name went right, and right, and right — and now to reach the bottom I walk the whole length of it, exactly as I did before I ever learned to halve. *Order without balance is a lie I tell myself.* — But watch the room. There is a move — one move — that lifts a node up and folds its parent down, and when it is done the *same names* are in the *same order*, left still smaller and right still larger, only the tree is *shorter*. A rotation. I cannot do it on every insert by hand; my hands are full of ink. The truly balanced shelves — the ones the next hall is built from — do it for themselves, on every single insert, and never lean at all. This week you learn to *see* the lean, and to *make* the move. Then you go and use the shelf that never needed you.
</div>
</div>

A binary search tree is only fast when it is *short* — when its height stays near log₂ n, so a descent throws away half the remaining tree at each step. Nothing in last week's code forced that. **Balance** is the missing guarantee, and this half-floor is about three things and one honest admission:

1. **You can measure the lean.** A node's *balance factor* is the height of its left subtree minus the height of its right. Zero is level; far from zero is a problem. A tree is *height-balanced* — the AVL property — when **every** node's balance factor is within ±1. Your sorted stick fails that at the root by a mile; you will write the check that proves it.
2. **One move re-levels without breaking the rule.** A *rotation* lifts a child into its parent's place and lowers the parent onto the other side. The astonishing part: an in-order walk of the subtree is **exactly the same** before and after — left is still smaller, right still larger. Rotation changes *height*, never *order*. That is why it is the legal repair, and you will write it.
3. **You do not build the self-balancing tree.** A real AVL tree applies that rotation at *every node* on the way back up *every* insert and remove (and a second rotation for the zig-zag cases) so it never leans in the first place. That is a lot of careful code — and it is already written, and tested, and waiting for you in the standard library. `std::set` and `std::map` are balanced trees. You will *measure* one against your stick and never look back.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/bst-balance-vs-stick.svg' | relative_url }}" alt="Two binary search trees built from the same seven values 1 through 7. On the left, a balanced tree rooted at 4 with 2 and 6 below it and the leaves 1,3,5,7 — height 2, every node's balance factor zero. On the right, the same values inserted in sorted order form a single right-leaning chain 1-2-3-4-5-6-7 — height 6, the root's balance factor minus 6. Both obey left-smaller-right-larger; only the heights differ."/>
  <figcaption>Same seven values, same rule, two shapes. <strong>Balance is about height, not order.</strong> The balanced tree (height 2) answers any query in ~3 comparisons; the stick (height 6) takes up to 7 — a BST that has decayed back into the linked list it was supposed to beat. The balance factor at the stick's root is −6: as lopsided as a node can be.</figcaption>
</figure>

<figure class="diagram">
  <img src="{{ '/assets/diagrams/bst-rotation.svg' | relative_url }}" alt="A left rotation around node 1, whose right child is 2 and right-right grandchild is 3 — a right-leaning stick. After the rotation, 2 is the new root with 1 as its left child and 3 as its right child. Below both trees, the in-order traversal is written out as 1, 2, 3 for each, identical, with a caption noting the order is unchanged."/>
  <figcaption>A <strong>left rotation</strong> on the leaning triple <code>1→2→3</code>: the right child <code>2</code> rises to the root, the old root <code>1</code> folds down to its left, and any subtree caught in the middle re-attaches on the way. Height drops from 2 to 1. Read both trees in order: <code>1, 2, 3</code> either way. <em>The order is untouched — that is the whole reason a rotation is safe.</em></figcaption>
</figure>

## Objectives

By the end of Floor 9½ you will be able to:

- Define what it means for a BST to be **height-balanced**, and compute a node's **balance factor** as `signedHeight(left) - signedHeight(right)` — counting an empty subtree as height −1, one less than a leaf.
- Explain why a correct BST built from **sorted input** degenerates into a stick of height *N−1*, and why that drags lookup back to *O(n)*.
- Implement `isBalanced` — the AVL invariant, checked at every node — and use it to diagnose the stick your Floor 9 tree becomes.
- Implement a **left and a right rotation**, and explain why a rotation re-levels a subtree *without changing its in-order order*, so it can never break the search-tree rule.
- Given an out-of-balance node, choose the **correct single rotation** (rotate right when left-heavy, left when right-heavy) and apply it.
- Compare your plain BST to `std::set` on **identical sorted input** and explain why a self-balancing tree holds its height near log₂ *N* — and why you reach for `std::set`/`std::map` instead of hand-rolling the balancing.
- Name what a full AVL insert does that your `rebalanceAt` does **not**: rebalance at every node on the way up, and handle the zig-zag (left-right / right-left) cases with a second rotation.

## Pre-class

### Reading (ZyBook Ch. 10, *Balanced Trees* — §10.1–10.4 AVL + a §10.5 red-black skim this week)

This is new reading, and it is short. Chapter 10 is the chapter that fixes Chapter 8's catch. Read the AVL sections; the red-black sections are a skim for context (they are the family `std::set`/`std::map` actually belong to).

- Before **Monday** — §10.1 *AVL: A balanced tree*. The one definition that matters: a node's **balance factor** is the height of its left subtree minus the height of its right, and an AVL tree keeps every balance factor in {−1, 0, +1}. The convention that matters: an empty subtree counts as height −1 (one less than a leaf) — that is what the provided `signedHeight` uses, and the reason a three-node stick comes out *unbalanced* rather than balanced.
- Before **Wednesday** — §10.2 *AVL rotations*. Watch a single left and right rotation and convince yourself, by reading the values left-to-right, that the **in-order order does not change**. Then §10.3 *AVL insertions* — how a real tree decides *which* rotation to apply and does it automatically on every insert. (You will write the decision for *one* node; §10.3 is the whole machine you are deliberately **not** building.)
- Before **Friday** — §10.5 *Red-black trees* (skim). You do not implement one — but `std::set` and `std::map` are red-black trees, so this is the structure you are about to benchmark against and then rely on for the rest of the course. If you want the matching AVL case, §10.4 *AVL removals* mirrors Floor 9's remove with a rebalance bolted on.

> **The honest admission, restated.** This is the one floor where the lab tells you *not* to finish the data structure. Implementing a correct self-balancing tree — rotations at every level, the four rebalance cases, removal — is a real piece of work, and the standard library has done it. The skill this week is to *understand the move well enough to trust the library*: measure the imbalance, perform one rotation by hand, and read the benchmark that proves why `std::set` exists.

There are no pre-class videos beyond the chapter's own animations. Class is live coding all three days.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | See the lean | Open `hero/BST.h` — your finished Floor 9 tree, with five new stubs near the bottom. Read the **provided** `signedHeight` (empty = −1) and say out loud why it differs from `height()`. Write `balanceFactorOf` (one subtraction) and `isBalancedFrom` (the familiar "check this node, recurse both sides"). Run `selftest balance` (phases 1–3 green) and `balance` — McCown's real catalogue reports **height 6, balance factor −3, NOT balanced**. Phase 3 drives it home: a tree built from already-sorted values is a stick, and now your check catches it. |
| **W** | Make the move | Do a left and a right rotation **on the board** first, writing the in-order walk under each — same before and after. Then write `rotateRight` and `rotateLeft` (four lines of pointer surgery each). Run `selftest balance` phase 4: `rotateRootLeft` on the stick `1→2→3` lifts `2` to the root, height drops 2→1, and the in-order walk is unchanged. The "order kept" assertion is the lesson. |
| **F** | Don't reinvent it | Write `rebalanceAt` — *left-heavy? rotate right; right-heavy? rotate left* — the decision a real AVL makes at every node. `balanceRoot` applies it once; phase 5 goes green. Then the payoff: `benchmark balance`. Same sorted input, your plain BST is a stick of height *N−1* while `std::set` stays flat. Close on the line that carries into next week: **you don't write the balancing — you reach for the balanced container.** That container is the Hall of Names. |

## The project — Floor 9½

This week's increment is **balance, bolted onto the BST you already built** — the tools to measure a tree's lean and the one move that fixes it, plus the benchmark that shows why, in practice, you let the library do it.

You will receive (in your starter drop):

- Everything through Floor 9, fully working — including your **complete** `BST<T>` (insert, find, in-order, the Rule of Three, remove). Floor 9's six helpers are now reference code; you build on top of them.
- A `hero/BST.h` with **five new helper bodies stubbed**: `balanceFactorOf`, `isBalancedFrom` (Monday); `rotateRight`, `rotateLeft` (Wednesday); `rebalanceAt` (Friday). The public delegators (`balanceFactor`, `isBalanced`, `rotateRootRight`, `rotateRootLeft`, `balanceRoot`) and a `signedHeight` helper (empty = −1) are **provided**.
- A `hero/BalanceTests.cpp` harness — `selftest balance` runs `BST<int>` through **five phases**: the balance factor, a bushy tree (balanced) versus the sorted stick (not), a rotation that repairs structure while preserving in-order, and `balanceRoot` picking the correct rotation.
- New commands: `balance` (report the bestiary tree's height and root balance factor) and `benchmark balance [N]` (the same sorted input built into your plain BST and a `std::set`). **`benchmark balance` does not depend on this week's stubs** — it works from day one, because it is the *comparison*, not your code.

You will write:

1. **Monday:** `balanceFactorOf`, `isBalancedFrom`. After this, `selftest balance` passes phases 1–3 and `balance` tells the truth about how lopsided the catalogue is.
2. **Wednesday:** `rotateRight`, `rotateLeft`. After this, phase 4 passes — a rotation re-levels the stick and the in-order walk is unchanged.
3. **Friday:** `rebalanceAt`. After this, phase 5 passes; then read `benchmark balance` and explain it.

Demo target (Friday):

```
> selftest balance
  balanceFactor: empty==0, bushy==0, left-leaning==1: OK
  isBalanced: bushy {5,3,8,2,4,7,9} balanced (true): OK
  degenerate: sorted stick {1..6} is NOT balanced (height 5): OK
  rotate: rotateRootLeft on {1,2,3} -> root 2, height 1, order kept: OK
  balanceRoot: right- and left-heavy sticks both rebalance (order kept): OK
  all phases OK

> balance
  McCown's catalogue: 15 names, height 6, root balance factor -3 — NOT balanced (lopsided).

> benchmark balance
-- Same sorted input: your plain BST vs std::set (1000 iterations per cell) --
  N=     10  plain BST: height=     9  find=    0.530 us    std::set: find=   0.144 us
  N=    100  plain BST: height=    99  find=    2.988 us    std::set: find=   0.207 us
  N=   1000  plain BST: height=   999  find=   29.176 us    std::set: find=   0.255 us
  ... your microseconds vary by machine; the shape never does: the stick climbs, std::set stays flat ...
> quit
McCown notes your departure. "You leave in order."
```

### Lab 9½ — *The Hand on the Level* (folded into the project)

There is no separate lab handout. The work you do this week *is* the lab. Commit `floor-09h/lab-notes.md` with:

1. A transcript of the demo above (your own hero name).
2. **The lean, by hand.** On paper, insert `1, 2, 3, 4, 5, 6, 7` into an empty BST in that order and draw the stick. Write the balance factor at the root (`1`) and at the middle node (`4`), counting an empty side as −1. Then draw the *balanced* tree on the same seven values (root `4`). One sentence: same values, same rule — what is different, and why does it change how many nodes `find` has to look at?
3. **A rotation, by hand.** Insert `3, 5, 8` in that order: `3` at the top, `5` as its right child, `8` as `5`'s right child. Do a left rotation at `3`. Draw the result and write the in-order walk *before and after*. Confirm they match. One sentence: why does "in-order unchanged" mean the result is still a valid BST?
4. **Which rotation?** For each unbalanced root, name the rotation `rebalanceAt` would apply and why: (a) a left-heavy root (balance factor +2); (b) a right-heavy root (−2). Then the case one rotation *can't* fix: insert `1, 3, 2` (so `2` is `3`'s *left* child). Do one left rotation at `1` and draw what you get. Is it balanced now? One sentence: what does a real AVL tree do instead? (You do **not** have to code it.)
5. **The comparison.** Run `benchmark balance`. Paste the table. (Your microseconds will not match the page; the heights will.) In two sentences, in your own words: the plain BST and the `std::set` saw the identical sorted input — why did one become a stick and the other stay short? What is `std::set` doing on every insert that your BST is not?
6. **One-paragraph reflection.** You stopped one step short of a self-balancing tree on purpose. Was that the right call? When, if ever, *would* you write the balancing yourself instead of using `std::set`/`std::map`? And: you've built five pointer structures and are about to start *using* one you didn't write. What do you gain, and what do you give up?

Your commit history should show at least three commits — Mon (`balanceFactor`/`isBalanced`), Wed (rotations), Fri (`rebalanceAt` + lab notes).

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix the Opportunist has a fix for his leaning tree</span>

Last floor Grix built a tree that was secretly a list — all the way to the right, you remember. Grix has FIXED it. Grix learned a new word: *rotation!* So now, whenever the tree leans, Grix grabs the top and rotates it. Rotate! Rotate again! ...It still leans. Grix rotates harder. Friend, why does one rotation at the very top not fix a tree that leans for *forty nodes*? ...Oh. *Oh.* You have to do it at every node? On every insert? While you go *back up*? That is — that is a lot of rotating. Grix does not have time for that much rotating. ...Grix will just use the `std::set`. Grix has always loved the `std::set`. (Grix is learning. Slowly. The right lesson, for once.)
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 9½</p>

**The Leaning Tower** — *HP: zero. Damage: a structure that is correct and slow at the same time, and a confidence that mistakes the first for fast.*
Strikes when code is *right* but *shaped wrong*. A BST built from sorted (or reverse-sorted, or nearly-sorted) input obeys every rule and answers every query correctly — and does it in *O(n)*, because it is a linked list wearing a tree's interface. The Tower is dangerous precisely because nothing is broken: tests pass, output is correct, and the only symptom is that it is slow in a way that grows with your data, discovered in production at 3 a.m. with a real dataset that happened to arrive in order.

Counter by:

- **Measure height, not just correctness.** A tree of *n* nodes should be about log₂ *n* tall. If `height()` is closer to *n*, you have a Tower, no matter how green your functional tests are. `balance` and `isBalanced` are the detectors.
- **Distrust ordered input.** The worst case is not random data — it is *sorted* data, which is exactly the data you most often have (it came out of another sorted structure, or a database `ORDER BY`). The plain BST's worst case is the common case.
- **Don't hand-roll the fix.** One rotation at the root is a demonstration, not a cure — as Grix discovered. A real balanced tree rotates at every node on every insert. That code exists and is correct: `std::set`, `std::map`, and every database index on earth. Reach for it.
- **The Leaning Tower and Floor 9's False Heir are cousins.** The False Heir broke the *order* invariant so `find` lied. The Leaning Tower keeps the order perfectly and breaks the *balance* the order was supposed to buy. One makes the tree *wrong*; the other makes it *pointless*. Both pass a naive test.
</div>

<div class="callout check" markdown="1">
<p class="callout-title">The Keeper's Check</p>

You are near the end of the Trees stretch (Floors 8–9½). This floor ends the way every floor ends: with a short stack of paper questions in class — closed notes, closed laptop, answers in your own hand — drawn from everything this floor taught. If you did the reading, wrote the code, and can say *why* this week's work earns its place in the game, it takes ten minutes.
</div>

---

*The hand in the dark sets the last branch level and withdraws, and the chart stands straight as a plumb line: fifteen names, and not one of them more than four questions deep. McCown exhales as if he had been holding the lean in his own spine. "It will hold now," he says, "as long as something keeps tending it." He does not say what. From the corridor beyond comes a different kind of order — not a tree at all, but shelves: every name with its meaning beside it, every key with its value, found not by descending but by simply* knowing where it lives. *The Nameless Bard, who has been tuning all this while, finally lays a chord across the strings and sings the next room's name — the Hall of Names, where you stop building the shelves and start, at last, only using them.*
