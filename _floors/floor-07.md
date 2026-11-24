---
title: The Cisterns
floor_number: Floor 7
order: 100
flavor_quote: "Take your number. I will see you in order. There is no other order."
week: 9
zybook: "Chapter 7 (continued) — class focus §7.5–7.9 (queues)"
---

## The briefing

Past Cantor Resh's lectern the corridor turns and slopes down. The walls grow slick. Somewhere ahead, water — a low, regular *drip* with the cadence of a clock. The corridor empties into a low-ceilinged hall floored with painted tiles, red then blue then red then blue, that mark out a single, very long line. At the far end of the line, behind a low ironwood desk, a clerk-monk in a soaked grey robe waits with a stamp in one hand. Petitioners stand in front of him, one after another, and he sees them strictly in the order they arrived. No one cuts. No one is skipped. The cantor behind you remembered the *last* word spoken. The steward in front of you remembers only the *first* petitioner still waiting.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/quill.svg' | relative_url }}" alt="Steward Quill, Keeper of the Cisterns — a clerk-monk in a soaked grey robe with a brass ticket-dispenser at his belt, seated behind a low ironwood desk, ink-stained fingers, an open ledger in front of him, eyes downcast on the page, a wax stamp in his right hand. The line of petitioners trails away behind him into the dark."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Steward Quill, Keeper of the Cisterns</span>

I will see you in the order in which you arrived. No sooner. No later. The petitioner at the *front* of the line is the *oldest* unanswered petition. When I am done with them they leave; the next-oldest steps forward; the line shortens by one. There is no other way to keep order. Cantor Resh upstairs remembers the last thing said. I remember the *first* thing still owed. Both of us are right, for different problems. *Bring me your problem.*
</div>
</div>

For two weeks now you have built containers that hand back the *most recent* — push and pop touched the same end of the chain, and the lesson was *last-in, first-out*. This week the contract flips. Push at one end, remove at the other; the oldest waiting element is the next one served. *First-in, first-out.* The line at Quill's desk is the most ordinary data structure in the world: a queue, the shape of every fair waiting room ever built.

The work this week is the second adapter you have written and the contrast is the point. `Stack<T>` from Floor 6 wrapped a `Chain<T>` with `push_front` + `pop_front`. `Queue<T>` will wrap the same `Chain<T>` with `push_back` + `pop_front`. One word changed; the contract flipped from LIFO to FIFO. That contrast is the whole lesson on what an ADT actually *is*: not the code — the *promise*.

You will learn three things this week, and the third one is a quiet payoff to the first two:

1. **FIFO is the other half of the LIFO coin.** Push and pop on the *same* end is a stack; push and pop on *opposite* ends is a queue. Same `Chain<T>` underneath; the adapter chooses which ends to use. Reading your Floor 6 `Stack.h` and your new `Queue.h` side by side is the entire ADT-vs-DS lesson said twice in different voices.
2. **Queues are how fair things stay fair.** Print jobs are queued. Network packets are queued. The threads waiting on a mutex are queued. The browser tabs waiting for the JavaScript event loop are queued. Anywhere the system has to honor the *order in which requests arrived*, a queue is doing the bookkeeping. The classic exercise — *Hot Potato*, pass a token around a circle and eliminate every k-th person — drills that pattern in the smallest interesting example.
3. **A queue is half of a tree traversal you have not written yet.** Three floors from now you will walk a tree in *breadth-first* order — closest nodes first, then the next-closest, and so on outward. The data structure that makes that walk possible is a queue: each visited node enqueues its children, and the children come back out in the order they were enqueued. Today you write the queue. Floor 10 will already have it sitting on the shelf, ready to use.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/queue-fifo.svg' | relative_url }}" alt="A horizontal row of three boxes labeled left to right: 'A', 'B', 'C'. An arrow labeled 'enqueue' enters from the right of 'C'; an arrow labeled 'dequeue' exits from the left of 'A'. A pointer labeled 'front' points at 'A'; a pointer labeled 'back' points at 'C'. To the right, a sequence diagram: enqueue A, enqueue B, enqueue C, dequeue → A, dequeue → B, dequeue → C. Caption emphasises that enqueue and dequeue touch OPPOSITE ends."/>
  <figcaption>A queue is one rule: <em>enqueue and dequeue touch opposite ends.</em> The <code>front</code> is whichever element was enqueued <em>first</em> among those still present. Dequeue it and the new front is the next-oldest.</figcaption>
</figure>

<figure class="diagram">
  <img src="{{ '/assets/diagrams/stack-vs-queue.svg' | relative_url }}" alt="Two side-by-side boxes. Left box labeled 'Stack<T> — LIFO'; shows push and pop both pointing at the same end (the top) of a Chain<T>. Right box labeled 'Queue<T> — FIFO'; shows enqueue at one end (back) and dequeue at the other end (front) of an identical Chain<T>. Caption: same underlying data structure, different end-pairing, different ADT."/>
  <figcaption>The same <code>Chain&lt;T&gt;</code> underneath both adapters. <strong>One end-pairing decision</strong> — same end vs. opposite ends — is the difference between LIFO and FIFO. That decision is the ADT.</figcaption>
</figure>

## Objectives

By the end of Floor 7 you will be able to:

- State the FIFO contract in one sentence and identify which end of the underlying storage each queue operation touches.
- Read your Floor 6 `Stack.h` and your new `Queue.h` side by side and explain — *in terms of which `Chain<T>` method each operation calls* — why one is LIFO and the other is FIFO.
- Implement `Queue<T>` as an adapter over `Chain<T>` — four one-line method bodies (`enqueue`, `dequeue`, `size`, `empty`; `front()` ships written — a reference-returning `T&` can't be honestly stubbed) — and explain why `Bag<T>` would be a *worse* choice underneath (and which operation would slow down).
- Use a `Queue<std::string>` to play the classical *Hot Potato* / Josephus-lite elimination — the canonical "rotate-then-dequeue" pattern — and explain why this algorithm fundamentally needs FIFO order (would not work on a stack).
- Implement a turn-resolution loop that drains an enemy action queue in arrival order, demonstrating that the queue is what makes the resolution *fair*.
- Read a compile error caused by calling a non-existent method on `Queue<T>` (say, `Queue::back()`) and explain what the queue ADT *deliberately omits* and why (no random access, no peek-at-back, no iterator — and what that buys you).
- Sketch in plain English how a *breadth-first* tree traversal would use a queue (foreshadowing Floor 10): each node visited enqueues its children, children come out in arrival order, so the closer ones come out first.

## Pre-class

### Reading (ZyBook Ch. 7, continued — the queue sections)

**The whole of Chapter 7 was assigned back on Floor 6** — this week we reach its queue material (§7.5–7.9). If you read the chapter through then, this is review. The pointers below are what *this week's* class leans on.

Before **Monday**: §7.5 *Queue abstract data type* — enqueue, dequeue, front, and the FIFO contract; and §7.7 *Queues using linked lists* — a queue built on a linked list, which is exactly this week's adapter over your `Chain<T>`.
Before **Wednesday**: §7.6 *C++ queue class* — the standard library's own `std::queue`, the adapter pattern you're recreating; and §7.8 *Array-based queues* — the same queue ADT on a different container.
Before **Friday**: §7.9 *Deque abstract data type* — the double-ended queue, where items go in and out at *both* ends. (Friday's hot-potato simulation is class material built on the plain queue.)

Skim the ZyBook §7.5 animation that enqueues three letters and dequeues them. Watch for the *opposite-ends* movement — and contrast it mentally with last week's §7.1 stack animation, which moved the same end up and down. The contrast is the whole lesson.

There are no pre-class videos. Class time is for live coding and discussion together — your reading is the prep.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | `Queue<T>` as adapter            | Open `Queue.h` and `Stack.h` side by side on the projector. Type the four stubbed method bodies on `Queue<T>` together: enqueue → `chain_.push_back`; dequeue → `chain_.pop_front`; size and empty → delegate. (`front()` → `chain_.head()->data` *ships written* — a reference-returning `T&` can't be honestly stubbed; read it alongside `Stack::top()` and note the reference return.) Run `selftest queue` — all phases pass. Then on the projector: `std::queue<int> q; q.push(1); q.push(2); std::cout << q.front();` — point out that you have just recreated the standard library again, on top of the same `Chain<T>` we wrote in Floor 4½. |
| **W** | Hot Potato                        | Live-code `hotPotato` in `hero/Potato.cpp` using `Queue<std::string>`. Walk through `{A,B,C,D,E}` with `k=3` on the board first, slot by slot, narrating which name moves to the back and which gets dequeued. Then write the function: load the queue, then loop "rotate k-1 times, then dequeue" until size == 1. Run `potato A B C D E 3` — survivor `D`. Discuss: this algorithm cannot be done on a stack, and that is *why* the queue exists. |
| **F** | Drain + queue-from-two-stacks     | Live-code the `simulate` dispatcher in `main.cpp`. Same hazard as Floor 6's `undo`: `const std::string& action = hero.enemyActionQueue.front();` refers to the front node, and `dequeue()` deletes that node. Finish *using* `action` before you `dequeue()`. Run `provoke Goblin; provoke Skeleton; provoke Imp; simulate` — three turns, in arrival order. Then on the board: sketch a `Queue<T>` implemented as *two `Stack<T>`s* — an in-stack and an out-stack. Cute interview classic; reinforces that ADT and DS are different (Queue ADT, Stack DS underneath). No checked-in code — the sketch *is* the lesson. |

## The project — Floor 7

This week's project increment is **a `Queue<T>` adapter and three things you do with it**: a `potato` command that runs the classical hot-potato elimination, a `simulate` command that drains an enemy action queue in arrival order, and a `selftest queue` harness that proves the FIFO contract holds.

You will receive (in your starter drop):

- Everything through Floor 6, fully working — `Bag<T>`, `Chain<T>` with `iterator` / `const_iterator`, `BagException`, `findByName<T>`, `Stack<T>` (LIFO adapter over `Chain<T>`), `isBalanced`, the `lint` command, the `undo` command, `take`, the `selftest chain` / `iterator` / `stack` harnesses, every benchmark, the boss battle, `clone hero`, the `log` and `log --oldest` commands.
- A new `hero/Queue.h` with **four method bodies stubbed** — `enqueue`, `dequeue`, `size`, `empty`. (`front()` ships written — a reference-returning `T&` can't be honestly stubbed, exactly like `Stack::top()`.) The TODOs name each stubbed method's one-line implementation in comment form. Deliberately mirrors `Stack.h` so you can read them side by side.
- A new `hero/Potato.h`/`Potato.cpp` declaring `std::string hotPotato(names, k)`. The body is **stubbed** — your Wednesday work.
- A new `hero/QueueTests.cpp` harness — `selftest queue` exercises `enqueue`, `dequeue`, `front`, `size`, `empty` across five phases with diagnostic output on FAIL. Phase 3 is the FIFO-vs-LIFO assertion (front after three enqueues must be the *first*, not the *last*).
- A modified `hero/Hero.h` that now owns a `Queue<std::string> enemyActionQueue` field alongside its Floor 6 `Stack<UndoAction> undoStack`.
- A modified `main.cpp` with three new commands wired in: `provoke <name>` (which enqueues a synthesized action onto the queue), `simulate` (whose dispatcher body is **stubbed** — your Friday work), and `potato <name>... <k>`. The existing `lint`, `undo`, `take`, and `sort` commands are unchanged.

You will write:

1. **Monday:** `Queue<T>`'s four stubbed method bodies in `hero/Queue.h` — `enqueue`, `dequeue`, `size`, `empty` (`front()` is already provided). Each is one line. After this, `selftest queue` passes every phase, and `provoke <name>` actually grows the queue (you can verify with repeated provokes and then a `simulate` that will print *empty* until Friday but at least sees the queue is non-empty if you check `.empty()` from inside its TODO).
2. **Wednesday:** `hotPotato` in `hero/Potato.cpp`, using your `Queue<std::string>`. After this, `potato Grix Skrit Quill Vael Resh 3` returns a real survivor name.
3. **Friday:** The `simulate` dispatcher in `main.cpp`. While the queue is non-empty: read `front()`, print it, push an event-log entry, `dequeue()`, increment a turn counter. After this, `provoke; provoke; provoke; simulate` resolves three turns in arrival order.

Demo target (Friday):

```
> selftest queue
  empty Queue<int>: size==0 && empty(): OK
  enqueue(7): size==1 && front()==7: OK
  enqueue 1,2,3: size==3 && front()==1 (FIFO): OK
  after dequeue(): size==2 && front()==2: OK
  drain to empty: size==0 && empty(): OK
  all phases OK

> potato A B C D E 3
  Survivor: D
> potato Grix Skrit Quill Vael Resh 2
  Survivor: Quill
> potato OnlyOne 7
  Survivor: OnlyOne

> provoke Goblin
  Goblin takes a number and joins the line.
> provoke Skeleton
  Skeleton takes a number and joins the line.
> provoke Imp
  Imp takes a number and joins the line.
> simulate
  Turn 1: Goblin stirs in the dark.
  Turn 2: Skeleton stirs in the dark.
  Turn 3: Imp stirs in the dark.
All enemies have acted; the line is empty.
> simulate
No enemies are in line. Use `provoke <name>` first.
> quit
Quill stamps your ticket. The line shuffles forward by one.
```

### Lab 7 — *First In, First Served* (folded into the project)

There is no separate lab handout. The work you do this week *is* the lab.

Commit `floor-07/lab-notes.md` to your project repo with:

1. A transcript of the demo above (with your own hero name and your own choice of provoked enemies).

2. **Same chain, different ends.** Open your Floor 6 `hero/Stack.h` and your Floor 7 `hero/Queue.h` side by side and fill in this table with the `Chain<T>` method each one calls:

   | Job | `Stack<T>` calls… | `Queue<T>` calls… |
   |---|---|---|
   | add an item (`push` / `enqueue`) | | |
   | remove an item (`pop` / `dequeue`) | | |
   | read the next item out (`top` / `front`) | | |

   Then finish two sentences: *"A stack and a queue are different because…"* and *"Queue uses a `Chain<T>` instead of a `Bag<T>` because…"* (the comment at the top of `Queue.h` answers the second one).

3. **Grix's shortcut.** Grix says to skip the line and `enqueue` at the front. Try it: in `hero/Queue.h`, change the one word in `enqueue` from `chain_.push_back(value);` to `chain_.push_front(value);`. Build, then run `selftest queue` and `potato A B C D E 3`. Paste both outputs. In one sentence: what did you actually build? Change the word back, rebuild, and run `selftest queue` once more to confirm all phases are OK again.

   *Optional bonus:* Phase 4 still says OK even with Grix's version. Why did it pass by luck?

4. **Pass the potato by hand.** Before you run it, predict the survivor of `potato A B C D 2` on paper. Write the queue after each step (front on the left). The first two rows are done for you:

   ```
   start:               A B C D
   rotate (A to back):  B C D A
   eliminate B:         ...
   ```

   Keep going until one name is left. Then run `potato A B C D 2` and paste the output. Did your trace match?

5. **Short reflection (two or three sentences).** You wrote the same kind of thing two weeks in a row: a small adapter that puts a contract on top of a `Chain<T>`. What felt easier the second time?

Your commit history this week should show at least three commits — Mon (`Queue<T>` + `selftest queue` passes), Wed (`hotPotato` + `potato` returns real survivors), Fri (`simulate` dispatcher + lab notes).

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix the Opportunist offers yet another shortcut</span>

*A line?* Friend, why wait in line? Just `enqueue` at the FRONT — that way you are the next one served! Grix tried this. Grix has used many queues. Sometimes the algorithm gives the right answer. Sometimes the goblin gets eaten first instead of fourth. **Most times — *most times!* — it's fine.** It really is. *Mostly.*
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 7</p>

**The Eternal Backlog** — *HP: zero. Damage: clock drift, then silent staleness, then a sudden flood.*
Strikes when code enqueues without ever draining. The queue ADT *promises* FIFO order but says nothing about *latency*: if items go in faster than they come out, the queue grows without bound, the head of the line is reading stale state, and eventually the program runs out of memory in a way that looks nothing like the original bug. In a debug build you might see the queue's `size()` climbing in a log line. In a release build, on a hero whose `enemyActionQueue` you never `simulate`, you get a Hero copy in `clone hero` that takes longer and longer to deep-copy as the queue grows behind your back.

Counter by:

- **Drain the queue.** If a producer enqueues, there must be a consumer that dequeues. Asymmetric rates are a bug, not a feature. The classical signal: `provoke` ten times, `simulate` zero times — that is an Eternal Backlog growing.
- **Watch `size()`.** A queue that only grows is a queue that is wrong. In production systems, queue depth is a primary monitored metric for exactly this reason.
- **The Eternal Backlog and Floor 6's Hollow Echo are *opposites*.** The Hollow Echo struck on `top()`-of-empty — *underflow*. The Eternal Backlog strikes on never-dequeue — *overflow* / unboundedness. Both are queue/stack ADT silences: the contract does not say what happens on either end of *misuse*. The caller's job, not the container's.
- **Bound it if you can.** Some queue implementations carry a maximum capacity and refuse (or block) new enqueues past it. `std::queue` does not. Yours does not. *That is fine for this floor* — but recognize what you are not getting and why.
</div>

<div class="callout check" markdown="1">
<p class="callout-title">The Keeper's Check</p>

This floor ends the way every floor ends: with a short stack of paper questions in class — closed notes, closed laptop, answers in your own hand — drawn from everything this floor taught. If you did the reading, wrote the code, and can say *why* this week's work earns its place in the game, it takes ten minutes.

And this check is the small one: the [Warden of the Middle Gates]({{ site.baseurl }}/floors/warden-02/) waits next week. It tests Floors 4 – 7 together.
</div>

---

*Steward Quill stamps the ticket of the last petitioner in line and looks up for the first time. He sets the stamp down. "You are next," he says, "and the one behind you is next after that. Always." Beyond his desk a heavy door opens onto a marshalling yard where four guards stand at the corners and a fifth waits at the centre, hand on the pommel of a sword. The cisterns are quiet. The yard is not. The **Warden of the Middle Gates** stirs.*
