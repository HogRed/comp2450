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
| 4 end | Warden I | [Warden of the Foundations]({{ site.baseurl }}/floors/warden-01/) | Midterm 1 — Floors 0–3 code-along with the Warden videos |
| 5  | 4  | [The Chain Vault]({{ site.baseurl }}/floors/floor-04/) | Singly-linked lists |
| 6  | 4½ | [The Chain Vault, Deeper]({{ site.baseurl }}/floors/floor-04h/) | Doubly-linked lists & the full Rule of Three |
| 7  | 5  | [The Eye of Scrying]({{ site.baseurl }}/floors/floor-05/) | Iterators — one spell to walk any container |
| 8  | 6  | [The Hall of Echoes]({{ site.baseurl }}/floors/floor-06/)       | LIFO, undo, call-stack intuition |
| 9  | 7  | [The Cisterns]({{ site.baseurl }}/floors/floor-07/) | FIFO, queues, AI action scheduling |
| 9 end | Warden II | [Warden of the Middle Gates]({{ site.baseurl }}/floors/warden-02/) | Midterm 2 — Floors 4–7 code-along with the Warden videos |
| 10 | 8  | [The Hall of Lineages]({{ site.baseurl }}/floors/floor-08/)     | General (n-ary) trees — recursion, DFS/BFS, lineage |
| 11 | 9  | [The Halving Hall]({{ site.baseurl }}/floors/floor-09/)         | Binary search trees — fast lookup, the free sort |
| 12 | 9½ | [The Balanced Hall]({{ site.baseurl }}/floors/floor-09h/)     | Self-balancing trees — diagnose the lean, rotate, then reach for std::set |
| 13 | 10 | [The Hall of Names]({{ site.baseurl }}/floors/floor-10/)         | Sets & maps |
| 14 | 11 | [The Hall of Whispers]({{ site.baseurl }}/floors/floor-11/)     | Hash tables — O(1) lookup |
| 14 end | Warden III | [Warden of the Trees]({{ site.baseurl }}/floors/warden-03/) | Midterm 3 — Floors 8–11 code-along with the Warden videos |
| 15 | 12 | [The Labyrinth]({{ site.baseurl }}/floors/floor-12/) / Final Descent ([study guide]({{ site.baseurl }}/study-guide/)) | Graphs, project capstone, final prep |
| 15 | ↓ | [The Archive: The Restoration]({{ site.baseurl }}/archive/) | The end of the story — for those who reach the bottom |

## The Rules of the Dungeon

- **Pre-class.** Watch the floor's video and read the assigned ZyBook sections *before* arriving.
- **In class.** MWF 50-minute sessions. We will not re-read the textbook to you. We will wield it.
- **Lab.** Hands-on exercises. Paired work encouraged.
- **The Project.** A text-based dungeon crawler you grow every single floor. One unlock per week. By Floor 12 it is a real game.
- **Exams.** Three warden fights (end of weeks 4, 9, 14) and a cumulative **Final Descent**. Each warden is a code-along: build the encounter with the recorded Warden videos and submit your code. Prepare for the final with the [Final Descent study guide]({{ site.baseurl }}/study-guide/).
- **The ZyBook Pact.** Finish every zyLab in the chapters we cover (1–8 and 10–13) on your own, any time before the last day of class, and your Final Descent is recorded as 100% — you never have to sit it. The terms are in [the syllabus]({{ site.baseurl }}/syllabus/).

## Before Floor 0

1. **[Arm yourself]({{ site.baseurl }}/setup/visual-studio/)** — install Visual Studio with the C++ compiler and CMake. One afternoon, and the one checkbox everybody misses.
2. **[Start your Chronicle]({{ site.baseurl }}/setup/git/)** — fork this course's repository to your own GitHub account and install GitHub Desktop.
3. Read [the syllabus]({{ site.baseurl }}/syllabus/).
4. Read **ZyBook Chapter 1**, alongside the **[Floor 0 Field Notes]({{ site.baseurl }}/companion/floor-00/)**.
5. Arrive to the first class ready to discuss: *what is a data structure, and why should you care?*

*(Curious what CMake even is? [Why the Forge Needs a Blueprint]({{ site.baseurl }}/setup/cmake/).)*

*The gate creaks open. Floor 0 awaits.*
