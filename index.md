---
title: The Dungeon Entrance
---

<div class="descent-banner">
  <img src="{{ '/assets/diagrams/descent-banner.svg' | relative_url }}" alt="A stone archway lit by a single torch opens onto a dark staircase descending toward distant glowing eyes. A skeleton hangs in chains from the right column. Title: COMP 2450 — Data Structures & Algorithms — The Descent."/>
</div>

> *Thirteen floors lie between you and mastery. Few who enter leave unchanged.*

## The Hold

What lies below you is what's left of the **Great Library of Algorithms** — sixteen floors that once held every craft of computation, now sealed beneath the monastery. When the **Unsorted Plague** came, it scrambled every shelf, every index, every cataloged beast — and the Library was closed. The **Order of the Sorted Tome** survives above-ground, and every year it sends one initiate down to recover what the Library lost. This year, that initiate is you.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/faith.svg' | relative_url }}" alt="Brother Faith, Abbot of the Sorted Tome — bearded monk in a dark brown robe with a brass clasp and a stopwatch on a brass cord."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Brother Faith, Abbot of the Sorted Tome</span>

I made the descent once. I will not say how deep. I returned with one page; it hangs in the antechamber. I will not give you answers — I will show you my stopwatch. **You have one semester. Go.**
</div>
</div>

Welcome, adventurer, to **COMP 2450 — Data Structures & Algorithms**. This semester you will descend through a dungeon of thirteen floors. Each floor teaches a new **artifact** (data structure) or **spell** (algorithm) that you will need to survive the ones below. By the end, you will have built a complete text-based dungeon crawler in C++ — not because you were told to, but because each floor's knowledge unlocked the next piece of it.

## The Floors

| Wk | Floor | Name | What you gain |
|----|-------|------|---------------|
| 1  | 0  | [The Antechamber]({{ site.baseurl }}/floors/floor-00/)         | ADTs, abstraction, the adventurer's vocabulary |
| 2  | 1  | [The Scrying Well]({{ site.baseurl }}/floors/floor-01/)         | Searching & Big-O intuition |
| 3  | 2  | [The Sorting Crucible]({{ site.baseurl }}/floors/floor-02/)     | Merge sort, quicksort, divide-and-conquer |
| 4  | 3  | [The Forgemaster's Vault]({{ site.baseurl }}/floors/floor-03/) | Templates & exceptions |
| 5  | 4  | The Chain Vault                               | Singly-linked lists |
| 6  | 4½ | The Chain Vault, Deeper                       | Doubly-linked lists, sorted insert |
| 7  | 5  | The Eye of Scrying                            | Iterators — one spell to walk any container |
| 8  | 6  | The Stacks                                    | LIFO, undo, call-stack intuition |
| 9  | 7  | The Cisterns                                  | FIFO, queues, AI action scheduling |
| 10 | 8  | The Branching Catacombs                       | Trees — dialog, skills, hierarchy |
| 11 | 9  | The Warden's Hall                             | Binary search trees |
| 12 | 9½ | The Balanced Hall                             | Self-balancing trees |
| 13 | 10 | The Hall of Names                             | Sets & maps |
| 14 | 11 | The Hall of Whispers                          | Hash tables — O(1) lookup |
| 15 | 12 | The Labyrinth / Final Descent                 | Graphs, project capstone, final prep |

## The Rules of the Dungeon

- **Pre-class.** Watch the floor's video and read the assigned ZyBook sections *before* arriving.
- **In class.** MWF 50-minute sessions. We will not re-read the textbook to you. We will wield it.
- **Lab.** Hands-on exercises. Paired work encouraged.
- **The Project.** A text-based dungeon crawler you grow every single floor. One unlock per week. By Floor 12 it is a real game.
- **Exams.** Three boss fights (end of weeks 4, 9, 14) and a cumulative **Final Descent**.

## Before Floor 0

1. Install a C++ compiler (`g++` or MSVC). See the syllabus for platform notes.
2. Install a text editor or IDE. VS Code with the C/C++ extension is a good default.
3. Read [the syllabus]({{ site.baseurl }}/syllabus/).
4. Arrive to the first class ready to discuss: *what is a data structure, and why should you care?*

*The gate creaks open. Floor 0 awaits.*
