---
title: The Labyrinth
floor_number: Floor 12
order: 170
flavor_quote: "Eleven halls you filled, and locked each beast in its place. This hall holds no beasts at all — only the dark between the doors, and the one question left: can you reach the bottom, and by the shortest road?"
week: 15
zybook: "Chapter 13 (Graphs) — full chapter; class focus §13.1, §13.2, §13.3, §13.7, §13.8"
---

## The briefing

The stair out of the Hall of Whispers is the last one. You feel it before you see it — the air stops being a room and starts being a *between*, a draft moving through more doorways than you can count, corridor calling to corridor in the dark. There is no wall of niches here, no shelf, no chart. There are only **doors**, and the doors open onto other doors, and somewhere past all of them is the bottom of the Hold. McCown did not follow you down; keepers of order do not go where there is none left to keep. But you are not alone. A low string sounds in the dark, an open note left to ring, and a voice you have heard at the top of every floor — never singing, always *saving* the verse — finally begins.

"Eleven halls," says the Nameless Bard, walking just behind you the way he always has, "and in every one you stored the beasts. A bag of them. A chain. A stack, a queue, a tree, a sorted shelf, a scattered whisper. You learned to *keep* a thing so you could *find* it again." He sets a hand on the nearest doorframe. "This hall keeps nothing. Look — there is no Lich here, no Goblin, no record at all. The Labyrinth does not store the beasts. It stores the **passages between them** — which room opens onto which, door to door. And so the question changes, one last time. Not *where is the Lich?* You answered that eleven times over. The question now is: *can you get to the Lich from here — and what is the shortest way down?*"

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/bard.svg' | relative_url }}" alt="The Nameless Bard — a lean, travel-worn minstrel in a dark hooded cloak with a lute slung across his back, standing in the mouth of a branching stone corridor whose many doorways recede into darkness; he is not singing, one hand resting on a single low string, the other on a doorframe, as if listening to which passages connect to which."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">The Nameless Bard, who has your ending already</span>

I have followed you down every floor and sung none of it yet — I was saving the verse for here, the last room, where the descent finally says what it was always about. You thought it was about *storing*. Eleven halls of containers will make a soul think that. But watch what crossing this place asks of you. To find whether a path runs from this door to the Lich's, you do not build a ninth container — you reach *back*, and the **Queue** you forged on the seventh floor becomes a way to search a maze level by level, and the **Stack** you forged on the sixth becomes a way to plunge down one passage to its end before turning back. Two old friends, waiting in the dark exactly where McCown said they would be. The map itself you keep in a third old friend — the scattered whisper from last floor, a hash set, remembering every room you have already stepped into so a circle of corridors cannot walk you in rings forever. You will not build much here. You will *connect* — and crossing the connection is the whole lesson the storing was teaching you to reach. The descent was never about keeping the beasts. It was about finding the way through.
</div>
</div>

For eleven floors you built containers that held *items* — `Bag`, `Chain`, `Stack`, `Queue`, `Tree`, `BST`, `HashTable`, then the library's `std::set`/`std::map`/`std::unordered_*`. Every one of them answered the same shape of question: *put this thing somewhere; give it back to me later.* The Labyrinth is the finale, and it is a different shape entirely. A **graph** stores **connections** — vertices (here, rooms) joined by edges (here, passages) — and the work is not *keeping* but *crossing*: given a web of rooms, can you get from one to another, and what is the shortest route? This is the **last floor of the dungeon**. There is no new container to build by hand; instead you write a light **adjacency-list `Graph`** of the dungeon's rooms, then **two traversals** that reuse what you already built — and the descent closes.

You will learn three things this week, and the first is the shift the whole course has been walking toward:

1. **A graph stores the passages, not the rooms.** Every prior data file described *things* — monster stats, item lists, a taxonomy of kinds. This floor's data file, `labyrinth.txt`, has no stats at all — just which room opens onto which. A **graph** (§13.1) is exactly that: *vertices* (rooms) connected by *edges* (passages). Two rooms are *adjacent* (§13.1) if a door joins them; a *path* (§13.1) is a sequence of doors from one room to another; and the *distance* (§13.1) between two rooms is the number of doors on the **shortest** path between them. The representation you build is the **adjacency list** (§13.3): each room keeps a list of the rooms it has a door to. A dungeon map is *sparse* (§13.3) — a few doors per room, not a door to every other room — and the list stores only the doors that exist, which is exactly why it is the right shape (the alternative, an adjacency *matrix* (§13.4), reserves a slot for every *possible* door, most of them empty).
2. **Crossing reuses two containers you already built — the only difference is FIFO vs LIFO.** To walk the map you need to visit rooms in *some* order, and the order is set by which container holds the rooms you have discovered but not yet visited. Use your Floor 7 **`Queue`** (first-in-first-out) and you get **breadth-first search** (§13.7): you visit the start, then every room one door away, then every room two doors away, level by level, never revisiting. Use your Floor 6 **`Stack`** (last-in-first-out) and you get **depth-first search** (§13.8): you plunge down one passage to its end, then backtrack. *Same map, same start, different order* — and that difference is nothing but the Stack-versus-Queue contrast you learned on Floors 6 and 7, now steering a maze.
3. **Because BFS visits level by level, it hands you the shortest path for free.** §13.7 says breadth-first search reaches distance-1 rooms before distance-2 rooms before distance-3; §13.1 defines *distance* as the doors on the *shortest* path. Join those and the conclusion is automatic: **the level at which BFS first reaches a room is its shortest hop-distance.** In an **unweighted** graph — every door costs one step — the fewest *doors* **is** the shortest way down, and BFS finds it with no extra machinery. (A *weighted* graph (§13.6), where doors cost different amounts, would need Dijkstra's algorithm (§13.9) instead — that is where graphs go next, past this floor.)

<figure class="diagram">
  <img src="{{ '/assets/diagrams/labyrinth-graph-vs-list.svg' | relative_url }}" alt="On the left, the dungeon map drawn as a graph: ten rooms as labelled circles — Entrance, Goblin, Skeleton, Wraith, Necrothrall, Shadow Hound, Lich, Stair, Frostmaw, and an isolated Oubliette off by itself — joined by lines for passages. Entrance, Goblin, and Skeleton form a small triangle (a cycle); Shadow Hound hangs off Wraith as a dead-end branch; Oubliette floats alone with no line to anything. On the right, the same map written as an adjacency list: each room name followed by its list of neighbour rooms (Entrance: Goblin, Skeleton; Goblin: Entrance, Skeleton, Wraith; and so on), with Oubliette showing an empty list. A caption notes that the list stores only the doors that exist."/>
  <figcaption>The dungeon map <em>is</em> a graph: rooms are <strong>vertices</strong>, passages are <strong>edges</strong> (§13.1). On the right, the same map as an <strong>adjacency list</strong> (§13.3) — each room keeps a list of the rooms it has a door to. The map is <strong>sparse</strong>: most pairs of rooms are <em>not</em> connected, so the list stores only the handful of doors that exist (note <code>Oubliette</code>, the sealed pit, with an empty list — a room reachable from nothing). <em>The triangle <code>Entrance–Goblin–Skeleton</code> is a <strong>cycle</strong>; remember it — it is what the visited set must tame.</em></figcaption>
</figure>

<figure class="diagram">
  <img src="{{ '/assets/diagrams/bfs-vs-dfs.svg' | relative_url }}" alt="Two crossings of the same small map from the same start room, side by side. On the left, breadth-first search using a FIFO queue: the start room is shaded, then its immediate neighbours are shaded as level 1, then their neighbours as level 2, in concentric rings fanning outward; a small queue diagram shows rooms entering at the back and leaving at the front. On the right, depth-first search using a LIFO stack: a single bold path plunges from the start straight down one chain of rooms to a dead end, then a dashed backtrack line returns and takes a different branch; a small stack diagram shows rooms pushed and popped from the top. Both share a visited-set box listing the rooms already seen, identical between the two."/>
  <figcaption>The <strong>same map, the same start, two orders</strong>. <strong>BFS</strong> (left) rides your Floor 7 <code>Queue</code> — FIFO — and fans out in rings: the start, then everything one door away, then two doors away (§13.7). <strong>DFS</strong> (right) rides your Floor 6 <code>Stack</code> — LIFO — and plunges down one passage to its end before backtracking (§13.8). The container is what flips the <em>order</em>; the <strong>visited set</strong> (the hash set from Floor 11) is the same for both, and it is what lets either one cross a map that has a cycle. <em>FIFO fans out; LIFO plunges down — exactly the contrast you learned on Floors 6 and 7.</em></figcaption>
</figure>

There is a fourth thing, and it is the trap that gives this floor its monster — the most dangerous bug in the course, because nothing crashes. A graph is not a tree: it has no leaves to stop you, no single root, and its passages can loop. Follow them naively and a single **cycle** (§13.5) — `Entrance–Goblin–Skeleton–Entrance` — feeds you back to where you started, and you walk it forever. The program does not error out; it simply **never returns**, discovered when the demo hangs. The cure is a **visited set**: a `std::unordered_set` (your Floor 11 hash set) that remembers every room the first time you reach it, so you never step into it twice. Check that set before you cross into a room — and the same set serves both BFS and DFS, because the container differs but the *memory* is identical. We meet that one head-on all week; it is the last boss.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/the-cycle.svg' | relative_url }}" alt="The three-room cycle Entrance, Goblin, Skeleton drawn as a triangle of rooms with doors between all three. Two crossings are shown. On the left, WITHOUT a visited set: an arrow loops Entrance to Goblin to Skeleton back to Entrance to Goblin again and again, spiralling, with a label reading hangs forever — never reaches the corridor beyond. On the right, WITH a visited set: each room is checked off in a small unordered-set box the first time it is seen, the walk refuses to re-enter a checked room, and after three rooms the walk exits cleanly down the passage to the rest of the map. A caption stresses the program does not crash — it simply never returns."/>
  <figcaption>The floor's monster, drawn. A graph with a <strong>cycle</strong> (§13.5) and a traversal with <em>no memory</em> walks <code>Entrance → Goblin → Skeleton → Entrance → Goblin → …</code> <strong>forever</strong> — and nothing crashes; the program just never returns, the scariest kind of bug. The fix (right) is the <strong>visited set</strong>: mark each room the first time you discover it, never enter it again, and the walk terminates and moves on. <strong>Mark each room the first time you visit it, and check the set before you step in</strong> — that is what keeps the walk from ever <em>processing</em> a room twice, even around a cycle.</figcaption>
</figure>

## Objectives

By the end of Floor 12 you will be able to:

- Define a **graph** and its vocabulary — *vertex*, *edge*, *adjacent*, *path*, *path length*, *distance* (§13.1) — and say what a graph stores that every prior structure did not (*connections*, not items).
- Explain the **adjacency list** representation (§13.3), build one with `addRoom`/`connect`, and justify why it is the right shape for a **sparse** dungeon map versus an adjacency **matrix** (§13.4).
- Add an **undirected** edge correctly — *both* directions, because a door you walk through you can walk back through (§13.5 contrast) — and return a room's neighbors without a phantom insert (the Floor-10 `find`-not-`operator[]` discipline).
- Implement **breadth-first search** (§13.7) by reusing your Floor 7 **`Queue`** plus an `std::unordered_set` visited set, and use it to answer **reachability** and to walk a map **level by level**.
- Explain why **BFS gives the shortest path in hops** on an **unweighted** graph (§13.7 + §13.1), and name what you would need instead if the passages had **weights** (§13.6 → Dijkstra, §13.9).
- Implement **depth-first search** (§13.8) by reusing your Floor 6 **`Stack`** plus the *same* visited set, and explain how one container choice (FIFO vs LIFO) turns "level by level" into "plunge then backtrack."
- Explain what a **visited set** is, what *exactly* goes wrong without it on a graph with a **cycle** (§13.5) — the program hangs, it does not crash — and why the same set serves both traversals.
- Name the **seven structures you built by hand** across the descent and the **two you reused** to cross the last room, and say why a graph traversal needed *those two* specifically.

## Pre-class

### Reading (ZyBook Ch. 13, *Graphs* — the whole chapter; class focus §13.1, §13.2, §13.3, §13.7, §13.8)

This is the last new reading of the course, and it is the chapter the whole descent has been pointing at. Read **all of Chapter 13**; the sections called out below are only where class lives. The chapter runs further than this floor builds — §13.9 *Algorithm: Dijkstra's shortest path*, §13.10 *Algorithm: Bellman-Ford's shortest path*, and §13.11 *Minimum spanning tree* are part of the assigned read but are *where graphs go next, past this course*. Read them as the horizon, not the homework.

- Before **Monday** — §13.1 **"Graphs: Introduction"**: the whole vocabulary, all defined here — *graph* ("a data structure for representing connections among items, and consists of vertices connected by edges"), *vertex* ("represents an item in a graph"), *edge* ("a connection between two vertices"), *adjacent* (two vertices "are adjacent if connected by an edge"), *path* ("a sequence of edges leading from a source ... vertex to a destination ... vertex"), *path length* ("the number of edges in the path"), and *distance* ("the number of edges on the shortest path between those vertices"). Then §13.2 **"Applications of graphs"** — driving directions, navigation, social networks: graphs model *connections*. The dungeon map is a graph (rooms = vertices, passages = edges), and "what is the shortest way down?" is §13.2's driving-directions motivation in miniature. Then §13.3 **"Graph representations: Adjacency lists"** — *adjacency list* ("each vertex has a list of adjacent vertices, each list item representing an edge") and *sparse graph* ("far fewer edges than the maximum possible"). This is the representation you build.
- Before **Wednesday** — §13.7 **"Graphs: Breadth-first search"**: *breadth-first search* ("a traversal that visits a starting vertex, then all vertices of distance 1 from that vertex, then of distance 2, and so on, without revisiting a vertex"), plus *graph traversal*, *visit*, *discovered*, and *frontier*. The ZyBook's BFS uses a queue and avoids revisiting — exactly your Floor 7 `Queue` plus a visited set. Notice *why* visiting distance-1 then distance-2 is what makes BFS give the shortest path. Skim §13.4 **"Graph representations: Adjacency matrices"** for *contrast only* — *adjacency matrix* ("a matrix element is 1 if the corresponding two vertices have an edge or is 0 otherwise") — it is the representation we are deliberately **not** building (O(V²) space, wasteful for a sparse map).
- Before **Friday** — §13.8 **"Graphs: Depth-first search"**: *depth-first search* ("a traversal that visits a starting vertex, then visits every vertex along each path starting from that vertex to the path's end before backtracking"). The ZyBook shows a *recursive* DFS; ours uses an **explicit `Stack`** (your Floor 6 container) so the cliffhanger's promise holds — same visited set, opposite discipline from BFS. Skim §13.5 **"Directed graphs"** and §13.6 **"Weighted graphs"** for contrast: our doors are **two-way (undirected)** and **unweighted (one step each)**; §13.5 also defines a *cycle* ("a path that starts and ends at the same vertex"), which is exactly what makes a memory-less traversal loop forever — this floor's monster.

> **The connective tissue, stated once.** Four things this floor leans on are *ours*, not the ZyBook's, so we label them **class material** rather than give them a § that does not exist. First: **the dungeon map is a graph** — the ZyBook's §13.2 gives road and social examples; "rooms are vertices, passages are edges" is our framing. Second: **the two traversals reuse the two containers you already built** — the ZyBook's BFS uses *a* queue and its DFS is recursive; the explicit "reach back for *your* Floor 7 `Queue` and *your* Floor 6 `Stack`" through-line is ours, and it is the promise the Hall of Whispers sang you out on. Third: **the visited set is your `std::unordered_set` from Floor 11** — the ZyBook tracks "discovered" with a per-vertex flag; using last floor's hash set is our composition beat. Fourth: **BFS-distance is the shortest path** — §13.7 says BFS visits distance 1 then 2; §13.1 defines distance as the shortest path; joining them ("the level BFS first reaches a room *is* its shortest hop-distance") is our synthesis. When `descend` prints a hop count this week, that number is BFS's level, and the level is the shortest way down.

There are no pre-class videos beyond the chapter's own animations. Class is live coding all three days.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | Build the map — rooms are vertices, passages are edges (the adjacency list) | Open `hero/Graph.h` — a header-only `Graph` whose one member is an adjacency list, `std::unordered_map<std::string, std::vector<std::string>>`. Read the **provided** members, the loader constructor (it reads `data/labyrinth.txt` and calls your `connect`/`addRoom` to build the map), and the trivial accessors. Write `addRoom` (ensure a room is a vertex — a key with a possibly-empty neighbor list, idempotent), `connect` (add an **undirected** edge — push *each* room onto the *other's* list, **both** directions), and `neighbors` (return a room's list via `find`, **never** `operator[]` — no phantom insert). Run `selftest crossing` (phases **1, 2, 3** green; 4–6 still FAIL). Then **`map`** prints every room and its doors — the §13.3 adjacency list made visible — and **`doors Lich`** (→ `neighbors`) lists the rooms one door from the Lich. The map is built; nothing is *traversed* yet. |
| **W** | Cross it breadth-first — reuse the Queue, never revisit (reachability + shortest hops) | Write `reachable` (BFS over your Floor 7 **`Queue`** + an `std::unordered_set` visited set: enqueue the start, mark it; dequeue, return true if it is the target, else enqueue each *unvisited* neighbor, marking as you enqueue; return false if the queue drains) and `bfsOrder` (the same machinery, recording each room as it is visited, **once**). Phases **4, 5** go green. Demo: **`reach Entrance Frostmaw`** (a path exists) and **`reach Entrance Oubliette`** (the sealed pit — no path) → **`cross Entrance --bfs`** (the rooms level by level) → **`descend Entrance Lich`** (the provided `hops`, riding *your* BFS — "the shortest way down is N doors"). The **visited set** (the hash set from Floor 11) is what keeps the `Entrance–Goblin–Skeleton` cycle from looping forever. |
| **F** | Cross it depth-first — reuse the Stack; same map, opposite order; then the whole-arc wrap | Write `dfsOrder` (DFS over your Floor 6 **`Stack`** + the *same* visited set: push the start; pop a room; if unvisited, mark and record it, then push its unvisited neighbors). Phase **6** goes green; `selftest crossing` is all green. Demo: **`cross Entrance --dfs`** — the *same* start and map as `--bfs`, plunging down one path to its end before backtracking — a visibly **different order**. Then **`benchmark crossing`**: an adjacency-**list** crossing (O(V+E)) vs an adjacency-**matrix** crossing (O(V²), the §13.4 contrast) — the list stays flat in the map's sparsity while the matrix climbs. **Close on the whole-arc wrap**: seven structures built by hand, two reused to cross the last room; the Bard sings the ending, and the Abbot returns. |

**A note on five independent indexes.** Floor 10 taught that the Bag, the BST, and the Registry are three independent views of the same monsters; Floor 11 added the HashTable as a fourth. This floor adds a **fifth** — but the Graph is different in kind from the other four. The Bag, BST, Registry, and HashTable are all built from the bestiary; they all index the *monsters*. The `Graph` is built from `labyrinth.txt`, and it indexes the *map* — the passages between rooms, with no monster stats in it at all. So five independent views now coexist, four of the beasts and one of the dungeon itself, and that last one is the natural capstone of the whole "independent views" theme: the data finally stopped being a list of things and became a web of relationships.

## The project — Floor 12

This week's increment is **the dungeon map as a graph** — an undirected, unweighted adjacency-list `Graph` of the Hold's rooms, added *in parallel* to the working `Bag<Monster>`, `BST<Monster>`, `Registry`, and `HashTable` from earlier floors. Nothing earlier is touched or removed. This is the **finale**: you do not build an eighth container by hand. You build a light graph, then cross it twice — and the crossing reaches back for two containers you already built.

You will receive (in your starter drop):

- Everything through Floor 11, fully working — `Bag`, `Chain`, your complete `Stack` and `Queue`, the whole `Tree<T>`, your `BST<T>`, the `Registry`, the `HashTable`, every benchmark and self-test. The prior floors' work ships as finished reference code; this starter is self-contained.
- A new `hero/Graph.h` — a small `Graph` class (header-only, like `BST.h`/`HashTable.h`) keyed on room names, with **six method bodies stubbed** in a clearly marked `// ===== YOU WRITE =====` block: `addRoom`, `connect`, `neighbors` (Monday); `reachable`, `bfsOrder` (Wednesday); `dfsOrder` (Friday). The adjacency-list member, a default constructor, the loader constructor (it reads `data/labyrinth.txt` and calls your `connect`/`addRoom`), the trivial accessors (`roomCount`/`empty`/`hasRoom`), the read-only `adjacency()` view for `map`, and a **provided** `hops` (a BFS that counts the shortest distance, riding your `Graph` primitives — you do not write it) are all provided.
- A `hero/GraphTests.cpp` harness — `selftest crossing` runs a **fixed inline sample graph** through **six phases** (it does *not* depend on `data/labyrinth.txt`, so the expected values are pinned; the fixed graph has a cycle and one isolated room, so the visited set and the disconnected case are exercised). The stubs ship returning empty/false/no-op answers, so the harness **FAILs with a diagnostic** rather than failing to build — and the binary never crashes, because an empty adjacency list and empty returns are always valid.
- New REPL commands, all wired for you: `map`, `doors <room>`, `reach <from> <to>` (→ `reachable`), `cross <start> --bfs|--dfs` (→ `bfsOrder`/`dfsOrder`), `descend <from> <to>` (→ the provided `hops`), and `benchmark crossing [N]`. **`benchmark crossing` builds its adjacency-matrix and reference adjacency-list competitors inline**, so those columns work from day one regardless of your stubs — your `Graph` column prints `--` until your `bfsOrder` exists.

You will write:

1. **Monday:** `addRoom`, `connect`, `neighbors`. After this, `selftest crossing` passes phases 1–3, `map` prints every room and its doors, and `doors <room>` lists the rooms one passage away.
2. **Wednesday:** `reachable`, `bfsOrder`. After this, phases 4–5 pass, `reach <from> <to>` answers yes/no, `cross <start> --bfs` walks the rooms level by level, and `descend <from> <to>` reports the shortest hop-distance.
3. **Friday:** `dfsOrder`. After this, phase 6 passes and `selftest crossing` is all green; then read `benchmark crossing` and explain why the adjacency list scales where the matrix does not.

Demo target (Friday):

```
> selftest crossing
  addRoom: two rooms added, idempotent, absent room absent: OK
  connect: undirected edge — both rooms exist, B in A and A in B: OK
  neighbors: C -> {A,B,D}; absent room empty AND no phantom insert: OK
  reachable: A->E true, A->A true, A->Z false (sealed room): OK
  bfsOrder: from A is [A, B, C, D, E], each room once: OK
  dfsOrder: from A is [A, C, D, E, B], each room once: OK
  all phases OK

> map
  Oubliette -> (no doors)
  Lich -> Wraith, Necrothrall, Stair
  Shadow Hound -> Wraith
  Necrothrall -> Skeleton, Lich
  Wraith -> Goblin, Shadow Hound, Lich
  Skeleton -> Entrance, Goblin, Necrothrall
  Frostmaw -> Stair
  Stair -> Lich, Frostmaw
  Goblin -> Entrance, Skeleton, Wraith
  Entrance -> Goblin, Skeleton
  (10 rooms, 10 passages)

> doors Lich
  From Lich: Wraith, Necrothrall, Stair

> reach Entrance Frostmaw
  Yes — a path runs from Entrance to Frostmaw.

> reach Entrance Oubliette
  No path from Entrance to Oubliette.

> cross Entrance --bfs
  breadth-first from Entrance:
    Entrance
    Goblin
    Skeleton
    Wraith
    Necrothrall
    Shadow Hound
    Lich
    Stair
    Frostmaw
  (9 rooms reached, level by level)

> cross Entrance --dfs
  depth-first from Entrance:
    Entrance
    Skeleton
    Necrothrall
    Lich
    Stair
    Frostmaw
    Wraith
    Shadow Hound
    Goblin
  (9 rooms reached, deepest-first)

> descend Entrance Lich
  The shortest way from Entrance to Lich is 3 doors.

> descend Entrance Entrance
  The shortest way from Entrance to Entrance is 0 doors.

> descend Goblin Frostmaw
  The shortest way from Goblin to Frostmaw is 4 doors.

> benchmark crossing
-- The Labyrinth: adjacency MATRIX vs adjacency LIST crossing (BFS) (100 iterations per cell) --
  V=     50  matrix=     4.320 us  list=     0.688 us  YourGraph=    27.910 us
  V=    200  matrix=    31.791 us  list=     1.842 us  YourGraph=   117.256 us
  V=    800  matrix=   282.936 us  list=     4.764 us  YourGraph=   313.257 us
  V=   3200  matrix=  4401.504 us  list=    17.263 us  YourGraph=  1351.771 us

The map is SPARSE — a few doors per room, not a door to every other room.
The adjacency LIST stores only the doors that exist, so one crossing costs
O(V + E) and stays flat in the map's sparsity. The adjacency MATRIX reserves
V² slots whether or not the doors exist, so finding a room's neighbors means
scanning a whole row of V — one crossing costs O(V²) and the gap explodes as
the map grows. That is why a graph traversal stores the doors that exist: the
list is what scales on a real sparse map. (If the YourGraph column shows `--`,
write bfsOrder() first — see `selftest crossing`.)

> quit
The Bard lets the string fall silent. "Go well. I have your
ending already — I have been singing it since the first floor."
```

*(Two notes on the block above. First, the `map` room order: the rooms print in the hash map's **bucket order** — not alphabetical, and not meaningful — so on your machine the rooms may print in a different order than shown; what is stable is the set of rooms, the doors **within** each room (your `connect`-call order), and the `(10 rooms, 10 passages)` footer. Second, the `benchmark crossing` microsecond numbers are wall-clock — they vary run to run and machine to machine; what is reproducible is the **shape**, i.e. the complexity classes. The adjacency **matrix** climbs O(V²) — each 4× in V is roughly a 9× in time — because it reserves V² slots and scans a whole row of V to find each room's neighbors. The reference adjacency **list** stays O(V+E) — each 4× in V is roughly 3–4× in time — because it walks only the doors that exist. **Your `Graph` sits on the list side of that contrast**: it scales far better than the matrix at large V (at V=3200 it is well under the matrix), but it carries a larger constant factor than the index-based reference list — string hashing, name comparisons, and neighbor copies — so at **small** V it can read slower than the matrix. The honest lesson is the always-present reference `list` column: **adjacency list O(V+E) vs adjacency matrix O(V²)**, and the list is why graph traversals scale on a real sparse map. The closing paragraph the command prints is fixed text.)*

### Lab 12 — *The Crossing* (folded into the project)

There is no separate lab handout. The work you do this week *is* the lab. Commit `floor-12/lab-notes.md` with:

1. A transcript of the demo above (your own hero name); include at least `map`, one `reach` that succeeds and one that fails, both a `cross --bfs` and a `cross --dfs` from the same start, and one `descend`.
2. **Vertices and edges, by hand.** Pick three rooms from `map`. Draw them as a tiny graph — circles for the rooms, lines for the passages between them. In two sentences: what is a **vertex**, what is an **edge**, and what does this graph store that the bestiary Bag did *not*?
3. **Undirected, both ways.** `connect` pushes each room onto the *other's* neighbor list — both directions. In two sentences: why is a dungeon door **undirected**, and what would break if `connect` only added *one* direction? (Hint: which way could you walk, and which way couldn't you?)
4. **BFS vs DFS, same map.** Put your `cross Entrance --bfs` and `cross Entrance --dfs` outputs side by side. In two sentences: which container does each reuse (Queue vs Stack), and how does FIFO-vs-LIFO turn one into "level by level" and the other into "plunge then backtrack"?
5. **The shortest way down.** Run `descend Entrance Entrance` (it is **0** doors) and `descend Entrance Lich`. In two sentences: why does **BFS** give the shortest path in hops on an unweighted graph, and where in the BFS do you count the hop? (And the off-by-one: a room is 0 doors from itself; an adjacent room is 1.)
6. **The Cycle.** The rooms `Entrance`, `Goblin`, `Skeleton` form a cycle. In two sentences: what *exactly* happens if your traversal forgets the **visited set** — and why does the program **hang** rather than crash? Why does the *same* set serve both BFS and DFS?
7. **Five views, one of them different.** The Graph is built from `labyrinth.txt`, not the bestiary, so `reach`/`cross` share no state with `list`/`recall`/`lookup`/`whisper`. In two sentences: why is that *correct*, not a bug — and what makes the Graph the *odd one out* among the five indexes (Bag, BST, Registry, HashTable, Graph)?
8. **One-paragraph reflection.** You have reached the bottom. You built **seven** structures by hand across the descent — name them — and to cross this last room you built none of them; you reached back for **two** you already had. Which two, and why did a graph traversal need *those two* specifically? And: the whole descent, you thought, was about *storing*. Say in your own words what it turned out to be about.

Your commit history should show at least three commits — Mon (`addRoom`/`connect`/`neighbors`), Wed (`reachable`/`bfsOrder`), Fri (`dfsOrder` + lab notes).

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix the Opportunist crosses the Labyrinth</span>

Grix is crossing the maze, and Grix is FAST, because Grix does not waste time *remembering* things. Walk to a door! Walk to the next door! Walk to the next one! Why write down where you have been — that is extra work, and Grix is making PROGRESS! Entrance to Goblin to Skeleton — back to Entrance — to Goblin — to Skeleton — Grix is *flying* now, the same three rooms blurring past, and any second the corridor to the bottom will appear... any second... Grix has been walking for an hour. Grix has visited Goblin four hundred times. The corridor two steps past the loop, the one with the stair down — Grix has never once reached it, because Grix keeps falling back into the circle and calling it *forward*. ...Oh. *Oh.* The maze has a LOOP in it. A circle of doors with no end. And Grix, with no memory of where he has been, just rides the circle forever, delighted, never escaping. A crossing has to REMEMBER. Mark each room the first time you see it; never step into it twice. (Grix needed a visited set. Grix always builds the bug the textbook warned about — and this one does not even crash; it just runs until the torches burn out.)
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 12</p>

**The Cycle (the Infinite Loop)** — *HP: infinite, until you remember. Damage: traps your traversal in an endless walk around a loop, or floods memory re-visiting rooms you have already seen.*
Strikes the moment a graph contains a **cycle** (§13.5: *"a path that starts and ends at the same vertex"*) and your BFS or DFS **forgets to mark rooms visited.** A graph is not a tree — it has no leaves to stop you and no single root; follow the passages naively and the loop `Entrance–Goblin–Skeleton–Entrance` feeds you back to where you started, forever. Nothing crashes. The program simply **never returns** — the worst kind of bug, the one you discover only when the demo hangs and the cursor never comes back.

Counter by:

- **Keep a visited set.** Mark every room the first time you visit it — a `std::unordered_set` (your Floor 11 hash set) keyed on the room name — and skip it if you reach it again. §13.7 names this: BFS proceeds "without revisiting a vertex." Without the set, a single cycle is an infinite loop.
- **Mark each room the first time you reach it.** BFS marks a room as it *enqueues* it, so it never enters the queue twice; the explicit-stack DFS marks it as it *pops* it (a room can sit on the stack more than once, but the pop-time check visits it only once). Either way the rule that matters is the same: each room is *visited* exactly once — so a cycle can't loop forever. (Omit the set entirely and it will.)
- **The same memory serves both traversals.** BFS rides the **Queue**, DFS rides the **Stack** — the data structure differs, but the visited set is **identical**. The thing that ends the walk is not the container; it is the memory of where you have been.
- **The Cycle completes the family.** Floor 9's False Heir broke *order*, so `find` lied. Floor 9½'s Leaning Tower kept order but lost *balance*, so `find` crawled. Floor 10's Phantom Key kept both and silently *grew the container on a read*. Floor 11's Collision traded order for speed and decayed under load. The **Cycle** is the finale's monster — the structure that has **no end of its own**, so *you* must supply the memory that ends the walk. It is the most fundamental graph lesson, and a fitting last boss: every traversal you will ever write rests on it.
</div>

<div class="callout check" markdown="1">
<p class="callout-title">The Keeper's Check</p>

You have reached the bottom of the Hold. The descent is finished — eleven halls of containers behind you, and one last room that asked you to *cross* instead of *keep*. This floor ends the way every floor ends: with a short stack of paper questions in class — closed notes, closed laptop, answers in your own hand — drawn from everything the floor taught. If you can still say *why* each structure earned its place in the game, the check takes ten minutes.

The questions of the **Final Descent** ask after all of it. There is no next floor — only the bottom, and the way back up. Prepare for it with the [Final Descent study guide]({{ '/study-guide/' | relative_url }}).
</div>

---

*The Bard finds the rest of the verse at last. He has watched you build a `Bag`, a `Chain`, a `Stack`, a `Queue`, a `Tree`, a `BST`, and a `HashTable` — seven structures forged by hand, floor by floor — and watched you reach, in the Hall of Names, for the shelves the library already kept. And here, in the last room, he watched you build no eighth thing at all: you reached **back**, and the `Queue` you forged on the seventh floor became a breadth-first crossing, and the `Stack` you forged on the sixth became a depth-first one, and the scattered whisper from the eleventh remembered every room so a circle of doors could not keep you. The descent was never about storing. It was about **connecting** — and the connecting was done with the very tools the storing taught you to build. Then a stopwatch clicks in the dark, and you turn, and **Brother Faith** is there — the silent Abbot who sent you down on the first day, who said he had made this descent once and would not say how deep. He has made it again, beside you, the whole way. He looks at the map you drew of the bottom, and at the seven structures behind you, and at the two you reached back for, and he spends his words the way he always has — only on what matters. "You reached the bottom," he says, "and you reached it by the shortest road." He shows you the stopwatch one last time, and stops it. (Grix is there too, somehow, with tea — he has finally learned to keep a visited set, and he would like to thank you for teaching him by dying so many times.) There is no Floor 13. There is the **Final Descent** — the cumulative exam, where the fourth and last Warden waits: **the Architect of Errors**, who fights on paper — and the dungeon crawler you have grown into a real game, one floor at a time. Climb back into the light. The Bard has your ending already; he has been singing it since the first floor. (He has not sung the last verse. When your descent is done, the last stair is open: **[The Archive: The Restoration]({{ '/archive/' | relative_url }})**.)*
