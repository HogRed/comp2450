---
title: The Hall of Whispers
floor_number: Floor 11
order: 160
flavor_quote: "The shelf you must walk; the whisper you need only breathe. Speak a name into the dark and the dark answers in the same breath — already, exactly, at once — unless two names breathe as one."
week: 14
zybook: "Chapter 12 (Hash Tables) — full chapter; class focus §12.1, §12.2, §12.4, §12.5, §12.8, §12.9"
---

## The briefing

The corridor out of the Hall of Names runs downhill, and the quiet changes as you go — the soft, ordered settle of a shelf filing itself gives way to something faster and stranger ahead, a sound like a hundred names spoken at once into a dark room and a hundred answers coming straight back. The doorway opens on a wall so vast you cannot see its edges, and it has *no alphabet at all*. Where McCown's shelves ran Bone Spider to Wraith in a ruled line, this wall is a honeycomb of **niches**, thousands of them, and the names sit wherever a whispered word *sends* them — Goblin here, Lich a hundred niches away, Wraith and Goblin somehow crowded into the *same* niche near the floor. There is no order to find. McCown is here, and his hands are not resting now; they are cupped at his mouth.

"Up there," he says, "I kept everything in its place, so I could find it. Here —" he speaks a name to the wall, *Lich*, and a niche far off answers instantly, the whole record there at once, no walk, no descent, no comparing — "here I keep nothing in its place, and I find it *faster*. I do not look for the name. I **speak** it, and the niche it belongs to answers. The trick is the speaking: the same word must always send to the same niche, or the wall would lose track of its own beasts." He lowers his voice, the way you lower a voice for something true and a little dangerous. "And sometimes — not rarely, *always*, in the end — two different names whisper to the *same* niche. When that happens, the niche keeps a little **chain** of everything that landed there." He looks at you sidelong. "You built that chain on the fourth floor. You are about to build the wall out of it."

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/mccown.svg' | relative_url }}" alt="Scrivener McCown, Keeper of the Bestiary — an aging scholar-monk in grey robes with ink-stained fingers and spectacles, hands cupped at his mouth as if whispering, standing before a vast honeycomb wall of unlabeled niches with no alphabet; one niche near the floor holds two name-plates crowded together where two names landed in the same place."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Scrivener McCown, Keeper of the Bestiary</span>

This is the last thing I have to teach you, and it is the opposite of everything I taught. For four floors I kept order — a tree, then a level tree, then a shelf that ordered itself — so that I could *find* a name by walking to where order said it must be. Forget all of it for one room. Here there is no order: I do not *walk* to the name, I **compute** where it lives and go straight there in a single breath. Speak "Lich" and the *speaking itself* tells me the niche — the same niche, every time, because the same word always makes the same whisper. That is a *hash*. And the wall is not magic; look closely and it is a thing you have already built — an array of niches, and every niche is one of the little **chains** you forged on Floor 4, holding the few beasts whose names happened to whisper alike. You did not need me for the library's shelf last floor; you reached for it and were right to. You do not need me for this one either — `std::unordered_map` *is* this wall, finished. But I will not let you trust a whisper you have never made yourself. Build it once. Then, on Friday, meet the one the library built — and know, to your bones, exactly what it is doing.
</div>
</div>

For five floors you built containers on raw pointers — `Chain`, `Stack`, `Queue`, `Tree`, `BST` — and then, across the Hall of Names, you *stopped* building and reached for `std::set` and `std::map`, the balanced trees the library already perfected. This floor is where the build-your-own arc gets its **last and best build**, because the thing you build is made out of a thing you already built: a separate-chaining **hash table** whose buckets are your own `Chain<Monster>` from Floor 4. A hash table is *an array of the linked list you wrote on the fourth floor.* You will implement the core motions — **hash a key, find its bucket, insert, find, erase, and rehash when the table gets crowded** — meet the new failure mode (**collisions**, and the **load factor** that controls them), and then, exactly as Floor 9's BST turned out to be `std::map`, you will reach for **`std::unordered_set`/`std::unordered_map`** and see that you just built it. This is the **second and final floor of the associative-structures run** (Floors 10–11); the next floor down is the graphs finale.

You will learn three things this week, and the first is the trade the Hall of Names left ringing in your ears:

1. **You compute where the name lives instead of searching for it.** Floor 1's `search` walked all fifteen records; Floor 9's `recall` descended a whole tree; Floor 10's `lookup` walked a balanced tree, log *n* steps, every time. The Hall of Whispers does none of that. A **hash function** turns the name into a **hash code** (§12.5), the modulo reduces that code to a **bucket index** (§12.5), and you go *straight to that bucket* — no comparisons on the way. Average **O(1)**, not O(log n): the whisper is instant because there was no walk.
2. **Collisions are inevitable, and chaining absorbs them.** Two different names can compute to the same bucket — a **collision** (§12.1) — and that is not a bug you can avoid; with more names than buckets it is a *certainty* (pigeonhole). The fix is **chaining** (§12.2): every bucket holds a *list* of everything that landed there, so a collision degrades gracefully into "scan a short list" instead of corrupting anything. And the list each bucket holds is *your `Chain`* — which is the whole point of building it by hand.
3. **The load factor controls the speed, and rehashing protects it.** The **load factor** (§12.4) is items ÷ buckets — how crowded the table is. Let it creep too high and the chains grow long and your O(1) whisper decays toward an O(n) walk. The cure is to **rehash**: double the bucket array and re-file every item into its new home. Watch the table do it to *itself* during load this week, and you will have *seen* why the whisper stays a whisper.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/hash-pipeline.svg' | relative_url }}" alt="The three steps from a name to its slot, drawn left to right. The string 'Lich' enters a hash function box labelled multiplicative string hash, h = h times 31 plus each character, which emits a large hash code 2368194. An arrow labelled mod 32 (the modulo hash) reduces that code to a bucket index, 2. A final arrow points into bucket 2 of a vertical array of 32 buckets, where bucket 2 holds a Chain of Monster — Lich's full record sitting in a one-node linked list. The caption notes each step's ZyBook section."/>
  <figcaption>From a name to its niche, in three computed steps — no comparisons, no walk. The <strong>hash function</strong> (§12.5) mixes every character of <code>"Lich"</code> into a big <strong>hash code</strong>; <strong>mod bucketCount</strong> (§12.5) folds that code down to a valid <strong>bucket index</strong>; and that bucket is one of your <code>Chain&lt;Monster&gt;</code> lists from Floor 4. <em>You do not search for the name. You compute where it lives and go straight there — that is the whole O(1) average.</em></figcaption>
</figure>

There is a fourth thing, and it is the trap that gives this floor its monster. A collision is harmless when it is one or two names sharing a niche — you scan a list of two. It becomes deadly when collisions **pile up**: either your hash function *clumps* keys instead of scattering them, or the load factor creeps high because you never rehash, and suddenly a "bucket" is a forty-deep linked list and `find` crawls — a hash table that has quietly decayed back into the linear scan you were trying to escape. We meet that head-on: a hash that *scatters* (Monday), chaining so a collision degrades instead of corrupting (Wednesday), and watching the load factor and rehashing before the chains get long (Friday).

<figure class="diagram">
  <img src="{{ '/assets/diagrams/hash-chaining-collision.svg' | relative_url }}" alt="A small hash table of eight buckets drawn as a vertical array, indices 0 through 7. Most buckets are empty. Bucket 3 holds a Chain with one node: Skeleton. Bucket 6 holds a Chain with two nodes linked head to tail: Goblin then Wraith — two different names that hashed to the same bucket, a collision, absorbed by the bucket's linked list. An inset shows that to find Wraith the table computes bucket 6 once, then scans that two-node chain comparing names, returning Wraith on the second node. A separate caption notes that returning the first item in the bucket — Goblin — would be the bug."/>
  <figcaption>A <strong>collision</strong>, absorbed by chaining. <code>Goblin</code> and <code>Wraith</code> both whispered to bucket 6, so the bucket's <code>Chain</code> holds <em>both</em> — and to find <code>Wraith</code> the table computes bucket 6 once, then scans that short chain, <strong>comparing names</strong>, until it matches. <strong>That last part is load-bearing: <code>find</code> must compare the name, not just the bucket</strong> — returning the first item it sees (<code>Goblin</code>) would be the bug. A single collision costs you a list of two; that is the graceful degradation §12.2 buys.</figcaption>
</figure>

<figure class="diagram">
  <img src="{{ '/assets/diagrams/hash-rehash-doubling.svg' | relative_url }}" alt="A before-and-after of a rehash. On the left, a four-bucket table whose buckets are getting full: bucket 1 holds a three-node chain, bucket 2 holds a two-node chain, with a load factor label reading items over buckets above the threshold of 0.75. A wide arrow labelled rehash: double the buckets and re-file every item points right. On the right, an eight-bucket table where the same items have been spread out — each item recomputed its home as hash code mod 8 instead of mod 4, so the long chains have broken up into mostly one-node buckets, and the load factor label now reads well under 0.75. A caption stresses that every item's bucket is recomputed against the NEW bucket count, because mod 8 sends a name to a different slot than mod 4."/>
  <figcaption>The <strong>load factor</strong> (§12.4) crept past the threshold, so the table <strong>rehashed</strong>: it doubled the bucket array and <strong>re-filed every item</strong>. The catch that makes rehash honest — each item recomputes its home against the <em>new</em> bucket count (<code>hash % 8</code>, not <code>hash % 4</code>), because a bigger array sends a name to a different slot. The long chains break up, the load factor drops, and the whisper stays a whisper. <em>A rehash that copied items into the same index would be silently wrong.</em></figcaption>
</figure>

## Objectives

By the end of Floor 11 you will be able to:

- Trace the three steps from a key to its slot — **key → hash code** (the hash function, §12.5) → **bucket index** (the modulo hash, §12.5) → the **chain** in that bucket — and name the ZyBook section behind each.
- Write a **multiplicative string hash** (§12.5) and explain why mixing every character makes a *good* hash — one that **scatters** keys evenly — and why a "fast" hash that clumps (Grix's name-length hash) gives back the O(n) scan.
- Reduce a hash code to a valid bucket with `% bucketCount`, and explain why it must be the modulo, never an unchecked index (and why the provided constructor guarantees `bucketCount() >= 1` so `% 0` can never happen).
- Implement `insert` against a separate-chaining table — compute the home bucket, **reject duplicates / update in place**, and `push_back` into that bucket's `Chain` — and `find`, which scans **one short chain** comparing names (not just buckets) for average **O(1)** lookup.
- Implement `erase` so it removes **only** the matching item from its bucket's chain, leaving any colliding neighbors intact, and explain why clearing the whole bucket is wrong.
- Define **load factor** (§12.4), explain why a high load factor makes `find` slow, and implement the **rehash** trigger — double the buckets and re-file every item by recomputing its home against the new bucket count.
- Explain what a **collision** is and why it is *inevitable* rather than a bug, and why **chaining** (the bucket being **your `Chain`**) is the graceful way to absorb one.
- Reach for `std::unordered_set`/`std::unordered_map` (§12.8/§12.9) and articulate that they *are* the hash table you just built — the unordered cousins of `std::set`/`std::map` — and name what you **trade away** (sorted iteration order) to get the O(1) whisper.

## Pre-class

### Reading (ZyBook Ch. 12, *Hash Tables* — the whole chapter; class focus §12.1, §12.2, §12.4, §12.5, §12.8, §12.9)

This is new reading, and it is the chapter you finally get to **build** from. Read **all of Chapter 12**; the sections called out below are only where class lives. One section is special in the opposite way from last floor: where Chapter 11 had a "here is the BST under the hood I'm choosing *not* to build" section, Chapter 12's §12.2 **Chaining** is the section you *do* build — its `ChainingHashTable` (`Hash`/`Insert`/`Get`/`Remove`) runs on the same core operations as the table you write this week.

- Before **Monday** — §12.1 **"Hash tables"**: the whole vocabulary, all defined here — *hash code* ("a fixed-size, non-negative integer that attempts to uniquely identify a key"), *hash function* ("computes a hash code for a given value"), *hash table* ("stores unordered items by mapping (or hashing) each item to a location in an array"), *bucket* ("each hash table array element"), and *collision* ("two different keys map to the same bucket index"). Then §12.5 **"Common hash functions"** — the *modulo hash* and the *multiplicative string hash* ("repeatedly multiplies the hash value and adds the ASCII (or Unicode) value of each character in the string"), which is exactly the hash you write for the monsters' string names.
- Before **Wednesday** — §12.2 **"Chaining"**: the collision strategy this floor uses — "Chaining handles hash table collisions by using a list for each bucket, where each list stores the items that map to that bucket." Skim §12.3 **"Linear probing"** for *contrast only* — it is the *other* strategy (one item per bucket, probe forward to the next empty slot) that we are deliberately **not** building; notice the *empty-since-start vs. empty-after-removal* subtlety probing carries that chaining avoids.
- Before **Friday** — §12.4 **"Hash table resizing"**: *resize* ("increases the number of buckets while preserving all existing items") and **_load factor_** ("the number of items in the hash table divided by the number of buckets") — the trigger behind rehashing. Then the payoff sections, §12.8 **"C++ unordered_set class"** and §12.9 **"C++ unordered_map class"** — "an unordered_map class that implements a map ADT **using a hash table**" — the library version of exactly what you built.

> **The connective tissue, stated once.** Three things this floor leans on are *ours*, not the ZyBook's, so we label them **class material** rather than give them a § that doesn't exist. First: **a hash table composes from a prior floor — the bucket is your `Chain`.** The ZyBook builds its own list type; our through-line is that each bucket is the linked list you forged on Floor 4. Second: **`std::unordered_map` is the hash-table cousin of `std::map`, the way a hash table is the unordered cousin of a BST.** §12.8/§12.9 describe the classes but do not frame them against `std::set`/`std::map`; that contrast is the spine of the associative run. Third: **a hash container's iteration order is unspecified — "scrambled."** The ZyBook says hash tables store items "unordered" (§12.1); the explicit teaching beat — *and therefore you cannot rely on iteration order, unlike last floor* — is ours. When `buckets` prints out of alphabetical order this week, that scramble *is* the trade you made for the instant whisper.

There are no pre-class videos beyond the chapter's own animations. Class is live coding all three days.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | Compute where the name lives — the hash, the bucket | Open `hero/HashTable.h` — a header-only `HashTable` keyed on a monster's name, whose **buckets are your own `Chain<Monster>`**. Read the **provided** members, the constructor (it walks the bestiary and calls `insert` once per monster), and the trivial accessors. Write `hashKey` — the **multiplicative string hash** (§12.5), `h = h * 31 + c` over the characters — and `bucketIndex` (one line: `% buckets_.size()`, the modulo hash). Then the **basic body of `insert`** (compute the bucket, `push_back`, `++count_`). Run `selftest whispers` (phase 1 green) → `hash Lich` makes "compute where the name lives" *visible* → `buckets` shows the fifteen monsters scattered across the slots, with at least one bucket holding **two names** — your first sight of a *collision*. |
| **W** | One short list, not the whole table — find, erase, no duplicates | Finish `insert` (the dup-check: re-inserting a name updates in place and returns `false`, no double-count), then `find` (compute the bucket, scan **only that one chain**, compare names) and `erase` (remove **only** the matching item via the provided `Chain::eraseFirst`). Phases 2–5 go green. Demo: `whisper Lich` finds the full record by scanning *one* bucket → `whisper Drake` (absent) → `banish Goblin` removes one item from a chain (its colliding neighbor stays) → re-run `buckets` to watch that chain shrink. Frame the contrast: Floor 10 walked a balanced tree (log n); this scans one short chain (≈O(1)). |
| **F** | Keep the whisper a whisper — load factor & rehash; then meet the library | Write `rehashIfNeeded` (if `loadFactor()` crosses the threshold, double the buckets and rehash) and the **one re-file line** inside the provided `rehash` driver (recompute each item's home against the *new* bucket count). Phase 6 goes green; `selftest whispers` is all green. `load` shows the §12.4 numbers — and note the table already grew itself **from 8 buckets to 32** during load, the load factor doing its job live. Then `benchmark whispers`: your `HashTable::find` vs Floor 10's `std::map::find` vs Floor 9's `BST::find` vs Floor 1's `linearSearch` — the hash table is **flat** (O(1) avg) and beats even the balanced tree at large N, and `std::unordered_map` (built inline) tracks it because it *is* a hash table. Close on the whisper into Floor 12. |

**A note on four independent indexes.** Floor 10 taught that the Bag, the BST, and the Registry are three independent views of the same monsters, free to disagree after you change one of them. This floor adds a **fourth**: the `HashTable` is built from the same bestiary, but `banish`-ing a monster from it touches **only the HashTable** — not the Bag, not the BST, not the Registry. So when you `banish Goblin` from the Hall of Whispers and then `recall Goblin` (the Floor 9 *BST* command, unchanged) and Goblin is *still there* — nothing is broken. Four views, one source, free to diverge. That is four tools doing four jobs, not a bug.

## The project — Floor 11

This week's increment is **the bestiary as a hand-built hash table** — McCown's catalogue re-indexed as a separate-chaining `HashTable` whose buckets are your own `Chain<Monster>`, added *in parallel* to the working `Bag<Monster>`, `BST<Monster>`, and `Registry` from earlier floors. Nothing earlier is touched or removed; this is the last container you build by hand, and it is built out of one you already built.

You will receive (in your starter drop):

- Everything through Floor 10, fully working — `Bag`, `Chain` (now with a provided `eraseFirst` helper), `Stack`, `Queue`, the whole `Tree<T>`, your complete `BST<T>` and its balance helpers, the `Registry`, every benchmark and self-test. The prior floors' work ships as finished reference code; this starter is self-contained.
- A new `hero/HashTable.h` — a small `HashTable` class (header-only, like `BST.h`) keyed on the monster's name, with **six method bodies stubbed** in a clearly marked `// ===== YOU WRITE =====` block: `hashKey`, `bucketIndex`, `insert` (Monday → finished Wednesday); `find`, `erase` (Wednesday); `rehashIfNeeded` plus the one re-file line in `rehash` (Friday). The two members, both constructors, `size()`/`empty()`/`bucketCount()`/`loadFactor()`, the read-only `buckets()` view, and the `rehash` **driver** (you write only the single re-file line inside it) are **provided**.
- A `hero/HashTableTests.cpp` harness — `selftest whispers` runs a **fixed inline sample** through **six phases** (it does *not* depend on `data/monsters.txt`, so the expected values are pinned; phases 4 and 6 force collisions with a tiny bucket count). The stubs ship returning empty/false/no-op answers, so the harness **FAILs with a diagnostic** rather than failing to build — and the binary never crashes, because the stubbed `bucketIndex` returns a valid 0.
- New REPL commands, all wired for you: `hash <name>`, `buckets`, `whisper <name>` (→ `find`), `banish <name>` (→ `erase`), `load`, and `benchmark whispers [N]`. **`benchmark whispers` builds its `std::unordered_map`/`std::map`/BST/linear competitors inline**, so every column but your own `HashTable` works from day one regardless of your stubs.

You will write:

1. **Monday:** `hashKey`, `bucketIndex`, and the basic body of `insert`. After this, `selftest whispers` passes phase 1, `hash <name>` prints a name's hash code and computed bucket, and `buckets` shows the fifteen monsters scattered across the slots with at least one collision.
2. **Wednesday:** finish `insert` (no duplicates), `find`, `erase`. After this, phases 2–5 pass, `whisper <name>` finds a record by scanning one bucket, and `banish <name>` removes one item from a chain.
3. **Friday:** `rehashIfNeeded` and the re-file line in `rehash`. After this, phase 6 passes and `selftest whispers` is all green; then read `benchmark whispers` and explain why the hash table is flat and beats even the balanced tree.

Demo target (Friday):

```
> selftest whispers
  hashKey/bucketIndex: deterministic, distinct, in range: OK
  insert+find: size==5, Wraith->hp==14, Lich->hp==35: OK
  no-dup insert: re-insert Lich updates in place, size stays 5, returns false: OK
  collision find: absent==nullptr in a full bucket; present names still found: OK
  erase: Wisp removed (size 4), Wraith/Lich intact, absent==false: OK
  rehash: bucketCount grew, all 7 still findable, loadFactor<=threshold: OK
  all phases OK

> hash Lich
  hash('Lich') = 2368194  ->  bucket 2 of 32
> hash Goblin
  hash('Goblin') = 2138207223  ->  bucket 23 of 32

> buckets
  [1] Bone Spider
  [2] Lich
  [4] Shadow Hound
  [12] Necrothrall
  [13] Ironclaw Bear, Skeleton
  [15] Wisp
  [23] Goblin, Wraith
  [28] Marsh Lurker, Ratking
  [30] Cave Troll, Cinder Bat
  [31] Frostmaw, Stone Sentinel
  (15 monsters across 32 buckets, load factor 0.469)

> load
  count 15, buckets 32, load factor 0.469

> whisper Lich
Lich   HP 35   ATK 6   weakness: holy
  (whispered in the Hall — found in one bucket)

> whisper Drake
  The Hall of Whispers has no answer for 'Drake'.

> banish Goblin
  Banished 'Goblin' from the Hall of Whispers. (14 remain.)
> whisper Goblin
  The Hall of Whispers has no answer for 'Goblin'.

> buckets
  [1] Bone Spider
  [2] Lich
  [4] Shadow Hound
  [12] Necrothrall
  [13] Ironclaw Bear, Skeleton
  [15] Wisp
  [23] Wraith
  [28] Marsh Lurker, Ratking
  [30] Cave Troll, Cinder Bat
  [31] Frostmaw, Stone Sentinel
  (14 monsters across 32 buckets, load factor 0.438)

> recall Goblin
Goblin   HP 8   ATK 2   weakness: fire
  (recalled from the tree)

> benchmark whispers
-- The Hall of Whispers: linear vs BST vs std::map vs unordered_map vs YOUR HashTable (1000 iterations per cell) --
  N=    100  query=hit     linear=     0.287 us  BST=   0.129 us  std::map=   0.098 us  unordered_map=   0.020 us  HashTable=   0.018 us
  N=    100  query=absent  linear=     0.047 us  BST=   0.084 us  std::map=   0.056 us  unordered_map=   0.020 us  HashTable=   0.018 us
  N=   1000  query=hit     linear=     2.302 us  BST=   0.213 us  std::map=   0.197 us  unordered_map=   0.029 us  HashTable=   0.037 us
  N=   1000  query=absent  linear=     0.438 us  BST=   0.104 us  std::map=   0.075 us  unordered_map=   0.023 us  HashTable=   0.021 us
  N=  10000  query=hit     linear=    20.977 us  BST=   0.410 us  std::map=   0.576 us  unordered_map=   0.123 us  HashTable=   0.139 us
  N=  10000  query=absent  linear=     4.819 us  BST=   0.130 us  std::map=   0.101 us  unordered_map=   0.097 us  HashTable=   0.027 us
  N= 100000  query=hit     linear=   241.994 us  BST=   1.386 us  std::map=   1.244 us  unordered_map=   0.210 us  HashTable=   0.195 us
  N= 100000  query=absent  linear=   174.099 us  BST=   0.145 us  std::map=   0.126 us  unordered_map=   0.125 us  HashTable=   0.054 us

Your HashTable is FLAT — average O(1) — and at large N it beats even the
balanced tree (std::map / your BST grow with log N; the hash table does not
grow at all). std::unordered_map tracks your HashTable because it IS a hash
table — the library version of exactly what you built. The trade for that
speed: ORDER. std::map's keys come out sorted; a hash container's do not —
iterate `buckets` and the names are scrambled. And the speed only holds while
the load factor stays low: let the chains grow long and the whisper becomes a
walk. (If the HashTable column shows `--`, write insert() first — see
`selftest whispers`.)

> quit
McCown notes your departure. "You leave in order."
```

*(The `benchmark whispers` microsecond numbers above are wall-clock — they vary run to run and machine to machine; what is reproducible is the **shape**, i.e. the complexity classes: `linear` climbs steeply with N (O(n)); `BST` and `std::map` grow slowly (O(log n)); `std::unordered_map` and **your** `HashTable` stay flat and small (O(1) average); and at large N the `HashTable` is at or below `std::map` — the hash table beats even the balanced tree — and tracks `unordered_map` because it **is** a hash table. In a Visual Studio **Debug** build every column is slower, `HashTable` often beats `unordered_map` (the Debug library double-checks every `unordered_map` step), and the `HashTable` column climbs a little at large N (about 0.1 µs to 1.5 µs) — compare that to `linear`, which grows thousands of times over the same range; that is still flat. The closing paragraph the command prints is fixed text. One thing the page does **not** promise: which real names share which bucket — that depends on the hash function you write, so the `buckets` dump above is one correct hash's output, shown for shape: some buckets hold two names, some are empty, and the order is scrambled, not alphabetical. Note too that `load` reports **32** buckets, not the 8 the table started with — loading the fifteen monsters crossed the load-factor threshold twice during construction, so the table rehashed 8 → 16 → 32 on its own.)*

### Lab 11 — *The Whisper and the Niche* (folded into the project)

There is no separate lab handout. The work you do this week *is* the lab. Commit `floor-11/lab-notes.md` with:

1. A transcript of the demo above (your own hero name); include at least `hash` of two creatures, a `whisper` of three (one of them absent), and one `banish` followed by a `buckets` showing the chain shrink.
2. **The three steps, by hand.** Pick a creature. Write out its path from a name to a slot: the **hash code** (you may quote what `hash <name>` prints), then the **bucket index** (`code % bucketCount`), then which **chain** it lives in. Name the ZyBook section behind each step.
3. **Why one bucket, not all fifteen?** Floor 1's `search` scanned all fifteen monsters; `whisper`/`find` scans one short chain. In two sentences: what makes that average O(1) — and what single thing could turn it back into O(n)?
4. **A collision, on purpose.** Run `buckets` and find a bucket holding two or more names — a collision. In two sentences: why is a collision *inevitable* rather than a bug, what does **chaining** do about it, and why does it matter that the bucket is *your `Chain`* from Floor 4?
5. **Load factor and rehash.** Run `load`. The table reports more buckets than it started with — why? In two sentences: define the **load factor**, say why a high one makes `find` slow, and describe what `rehash` had to do to *every* item (and why recomputing each item's bucket against the *new* count is the part that makes it correct).
6. **Grix's hash.** Grix's hash function returns the name's length: `hashKey = name.size()`. In two or three sentences: what goes wrong with his "Hall of Whispers," and why doesn't rehashing to more buckets rescue him? What makes the **multiplicative string hash** (§12.5) a *good* hash where his is not?
7. **Order, traded away.** Floor 10's `names` printed in alphabetical order; this floor's `buckets` prints scrambled. In two sentences: why? What did you **trade away** to get the O(1) whisper, and when would you pick `std::map` over `std::unordered_map` *because* of it?
8. **One-paragraph reflection.** You have now built six pointer structures, and this one was built out of another — its buckets are the `Chain` you wrote on Floor 4. What did it feel like to have an old structure *become a part of* a new one? And: you traded order for speed this week — name one task where that trade is exactly right, and one where it is exactly wrong.

Your commit history should show at least three commits — Mon (`hashKey`/`bucketIndex`/`insert`), Wed (`find`/`erase`/no-dup `insert`), Fri (`rehashIfNeeded` + re-file line + lab notes).

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix the Opportunist builds a faster wall</span>

Grix has his OWN Hall of Whispers now! And Grix made the whisper *simple*, because Grix is clever. Why do all that multiply-multiply-add nonsense on every letter? Grix's hash is just the LENGTH of the name! "Goblin" — six! "Wraith" — six! Same niche! "Lich" — four, "Wisp" — four — same niche again! So fast, Grix barely has to think! ...But now Grix wants "Wraith," and niche six already holds Goblin, so Grix walks *past* Goblin to reach Wraith — and over in niche four he walks past Lich to reach Wisp. Every same-length name piles into the same niche, while the niches for lengths no beast has — three, five, nine — sit *empty*. Half the wall wasted, and Grix *still walking*! ...Oh. *Oh.* A whisper has to SCATTER. If the whisper sends "Goblin" and "Wraith" to the same niche just because they are the same LENGTH, it is not a whisper at all — it is the old long walk wearing a wall's hat. (Grix needed a hash that mixes every letter. Grix always builds the thing that gives back exactly what he was running from.)
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 11</p>

**The Collision** — *HP: grows with the load factor. Damage: turns your O(1) whisper back into an O(n) walk, one bucket at a time.*
Strikes when two different keys hash to the same bucket — which is **not rare; it is inevitable** (pigeonhole: more keys than buckets, and two of them must share). A *single* collision is harmless: the bucket's chain holds both, and `find` scans a list of two. The Collision becomes deadly only when collisions **pile up** — a hash that *clumps* (Grix's name-length hash, where every six-letter beast lands in one niche), or a **load factor** that creeps too high because you never **rehash**, until a "bucket" is a forty-deep linked list and `find` crawls. Nothing looks broken: the answers are still correct, the tests still pass — the table has just quietly decayed back into the linear scan you built it to escape, discovered in production with a real dataset that happened to clump.

Counter by:

- **Use a hash that scatters.** The multiplicative string hash (§12.5) mixes every character, so similar names land far apart. A "fast" hash that clumps — name length, first letter, a constant — gives back O(n). Grix learned this the hard way; you can *see* exactly what he mistook by reading his lopsided `buckets` dump.
- **Chain, so a collision degrades instead of corrupting.** Chaining (§12.2) gives every bucket a list — *your `Chain`* — so two names sharing a niche cost you a scan of two, not a wrong answer. A correct `find` compares the **name**, not just the bucket; returning the first item in the bucket is the bug.
- **Watch the load factor and rehash before the chains get long.** The load factor (§12.4) is items ÷ buckets. When it crosses the threshold, double the buckets and re-file every item — recomputing each item's home against the *new* count. This is what keeps O(1) from decaying toward O(n) as the table fills.
- **The Collision completes the family.** Floor 9's False Heir broke *order*, so `find` lied. Floor 9½'s Leaning Tower kept order but lost *balance*, so `find` crawled. Floor 10's Phantom Key kept both and silently *grew the container on a read*. The Collision gives up order entirely for speed — and the price of that speed is that you must keep the table from crowding, or the whisper becomes a walk.
</div>

<div class="callout check" markdown="1">
<p class="callout-title">The Keeper's Check</p>

You have closed the associative-structures run (Floors 10–11) and finished the last data structure you build by hand. This floor ends the way every floor ends: with a short stack of paper questions in class — closed notes, closed laptop, answers in your own hand — drawn from everything this floor taught. If you did the reading, wrote the code, and can say *why* this week's work earns its place in the game, it takes ten minutes.

The [**Warden of the Trees**]({{ site.baseurl }}/floors/warden-03/) (Midterm 3) is assigned this week. It covers the whole tree-and-associative arc — trees, BSTs, balanced trees, sets/maps, and hash tables. To complete it, sync your fork, open the Warden III starter (`project/warden-03-starter/`), and follow along with the Warden III videos as you build the encounter, then push your code to your repo and submit the repo link on Canvas. That's the whole requirement. The due date is on Canvas.
</div>

---

*McCown speaks one last name into the wall, and the niche answers in the same breath it always has, and he lowers his cupped hands for the final time in the stretch. "Five floors I kept for you," he says, "a lineage, a search, a level, a name, and a whisper. They are yours now." He does not follow you to the stair; keepers of order do not go where there is none left to keep. But between his last hall and the stair down, a gallery waits where no gallery should be — masks on iron stands, each wearing a face you have catalogued — and at its centre a shape in dark plate stirs: the third of four. The **Warden of the Trees** bars the way. Beyond it, from the dark below comes a sound unlike any hall you have crossed — not a shelf, not a tree, not a whispered niche, but a draft moving through a thousand doorways at once, corridor calling to corridor. Every hall above stored the monsters as separate things — in a bag, a chain, a stack, a queue, a tree, a sorted shelf, and now a scattered whisper. The hall below stores something else entirely: not the beasts, but the **passages between them**, room to room, door to door. And the question is no longer "where is the Lich?" but "**can I get from here to the Lich, and what is the shortest way down?**" The Nameless Bard tunes a low string and does not sing yet; he is saving the verse. Two old friends are waiting for you in the dark, McCown says as you go — the **Queue** you built becomes a way to search a maze breadth-first, and the **Stack** you built becomes a way to search it depth-first. The last descent is not about storing. It is about **connecting**. **Floor 12 — The Labyrinth (graphs, ZyBook Chapter 13) — is the next floor down.***
