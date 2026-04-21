---
title: The Antechamber
floor_number: Floor 0
flavor_quote: "The gate is open. Before you descend, you must learn the names of the things that hunt in the dark."
week: 1
zybook: "Chapter 1"
---

## The briefing

You stand in the **Antechamber** — the first room of the dungeon. Nothing threatens you here yet. Before you descend, you must learn the **language of the dungeon**: the names of the things that adventurers carry, the things they fight, and the way their efforts are measured.

This week, you learn three vocabularies:

1. **Abstract Data Types (ADTs)** — the *interfaces* your adventurers use to organize loot, spells, and enemies. Every tool you'll carry this semester is an instance of one of these.
2. **Abstraction & encapsulation** — the *discipline* of keeping implementation details hidden behind those interfaces.
3. **Algorithm efficiency** — an intuition for how *costly* an operation is as the dungeon grows. (Full Big-O follows on Floor 1.)

You will not implement anything new this week. You are being given a map of what's coming.

## Objectives

By the end of Floor 0 you will be able to:

- Define *data structure*, *algorithm*, and *abstract data type* in your own words.
- Name the common ADTs (list, stack, queue, deque, bag, set, priority queue, dictionary) and describe what operations each supports.
- Explain why two implementations of the same ADT can have very different performance.
- Estimate, informally, whether an algorithm's cost grows *slowly* or *quickly* as input grows.
- Compile and run a C++ program on your machine (confirming your dev environment works).

## Pre-class

### Reading (ZyBook Ch. 1)

Before **Monday**: §1.1 Data structures, §1.2 Introduction to algorithms
Before **Wednesday**: §1.3 Relation between data structures and algorithms, §1.4 Abstract data types, §1.5 Applications of ADTs
Before **Friday**: §1.6 Algorithm efficiency

Work the Question Sets and Animations inside the ZyBook — they count toward your participation grade.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | Course entry, what is a DS?     | Syllabus & dungeon tour; *Unplugged ADT* card activity; **start dev environment setup** (g++, editor, git) — finish before Wednesday |
| **W** | Common ADTs; first C++ syntax   | *ADT Triage* — match scenarios to ADTs in pairs; live-code two tiny C++ scripts together (`hello.cpp`, `vector_demo.cpp`) so everyone has touched the compiler at least once |
| **F** | Algorithm efficiency, intuition | *Race the dungeon* — hand-trace two algorithms on inputs of size 10, 100, 1000; predict; then run a small `growth.cpp` sample together to see the numbers |

There are no pre-class videos this week. Class time is for setup, discussion, and live coding together.

## Lab — folded into the project

**Lab 0 — "First light."** No separate handout. The project task below *is* the lab: build, run, change one string, commit. By Friday you should have a working compiler, a clean clone, and one commit pushed.

Deliverable: a commit pushed to your project repo with your hero's name set in `main.cpp`.

## The project — Floor 0

This week's project increment is **environment-only**. You will not implement any data structures yet. What you will do:

1. Clone the starter.
2. Build it with CMake (`cmake -B build && cmake --build build`).
3. Run it. It prompts for your hero's name and echoes a single line of dungeon flavor text.
4. Change one string constant so the flavor text is yours.
5. Commit and push.

Why so little? Because environment setup — compilers, paths, build tools — is the single most common reason students fall behind in week 1. We flush out those gremlins before we need to fight real ones.

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 0</p>

**The Compile Demon** — *HP: your patience.*
Strikes when your code is fine but your *environment* isn't. Symptoms: `g++: command not found`, `'cout' was not declared`, linker errors with no line number. Counter by:

- Confirming your compiler is on your `PATH`.
- `#include <iostream>` and `using namespace std;` (or `std::cout`).
- Rebuilding from a clean state when mystery errors persist.
</div>

<div class="callout check" markdown="1">
<p class="callout-title">Check for understanding</p>

Before you descend to Floor 1, you should be able to answer these without looking:

1. In one sentence, what is the difference between a **data structure** and an **abstract data type**?
2. Name three ADTs and, for each, name one real-world situation where it's the right tool.
3. If Algorithm A takes 1 ms on 100 inputs and 4 ms on 200 inputs, and Algorithm B takes 1 ms on 100 inputs and 100 ms on 1000 inputs — which one do you trust on a million inputs, and why?
4. What does it mean to say a container is "encapsulated"?
5. What command compiles `main.cpp` on your machine? Write it from memory.

Answers are discussed in the Monday Floor 1 warmup.
</div>

---

*The Antechamber is quiet. The stair to Floor 1 is marked in the map. Rest well.*
