---
title: The Warden of the Trees
floor_number: "Warden · III"
order: 165
flavor_quote: "Four floors taught you to keep order. The Warden is what order becomes when the names are stolen — and it asks whether you can name them back."
covers: "Floors 8 – 11 · Midterm 3"
---

## The gate

Below the Hall of Whispers the stair does not open onto another shelf. It opens onto a low, circular hall hung with masks — hundreds of them, every beast McCown ever catalogued, their faces peeled from the bestiary and pinned to the wall in no order at all. At the centre, in dark iron plate, the third Warden stands wearing five of those faces at once, one over another, and it will not show you its own.

McCown is not here. He said as much at the top of the last stair — *keepers of order do not go where there is none left to keep* — and he meant it. You wield what he taught you, and his bestiary is your arsenal, but he neither fights nor stands at this gate. Four halls stand in the corners in his place, and they are not people: a branching **tree** of lineages, a **searchable shelf** that leans, a **name-rack** of weaknesses, and a **wall of whispered niches**. The Warden is what those four halls become when the Plague has its way with them — the shadow of ordering, the Unsorted Plague's own answer to a cataloguer. It cannot be struck while its stolen menagerie stands. To break a mask you must name the beast beneath it; to break the Warden you must do the one thing the Plague cannot survive — put a beast back in its place, root to leaf. *The shadow of ordering is undone by ordering.*

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/warden-03.svg' | relative_url }}" alt="The Warden of the Trees — a hooded, faceless figure in dark iron plate. Where the first Warden showed two ember eye-points and the second a single horizontal ember slit, this one shows a forked ember — a stem of light splitting upward into two branches, like a small tree. Four sigils on the breastplate: a branching tree, a two-pronged search-fork, a key-and-value nameplate, and a honeycomb of niches — for the four floors above (trees, binary search trees, sets/maps, hash tables). The nameplate at the foot has no name; only the title WARDEN OF THE TREES."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">The Warden, who has no name to give</span>

*Two gates you have opened. I am the third, and I have no face of my own —*
*only the ones you catalogued. Name them back, or wear one here forever.*
</div>
</div>

**This page is your Midterm 3.** It replaces the timed paper exam, and it is one job: **follow along with the recorded Warden III videos, building the encounter in the Warden III starter as you watch, then submit the code you end up with** — pushed to GitHub and turned in on Canvas. There are no written trials and no in-class quiz. The code you build with the videos is the whole submission.

<div class="callout check" markdown="1">
<p class="callout-title">When it is due</p>

The due date is posted on Canvas.

Plan your week. Coding along takes longer than watching — pause whenever the video compiles and runs, and do the same at your keyboard. Several short sittings beat one long night. **Floor 12 (The Labyrinth) opens the same week — and it is the finale.** Do not let the last floor eat your Warden time. Midterm first, Labyrinth second.

</div>

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/faith.svg' | relative_url }}" alt="Brother Faith, Abbot of the Sorted Tome — bearded monk in a dark brown robe with a brass clasp and a stopwatch on a brass cord."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Brother Faith stands with you at the gate</span>

Third gate. You have stood at two of these already, and the shape has not changed — a Warden to build into your own game, and then to fight. What changed is the arsenal: five halls of it now — though the Halving Hall and the Balanced Hall are one craft, so four words carry them all — and the Warden wears them against you. Classify. Recall. Ward. Whisper. If those four words live in your fingers by the time you submit, the stair opens. If they live only in your notes, the hall of masks will see it the moment the first one steps forward.
</div>
</div>

## How to complete this Warden

1. **Watch the Warden III videos and code along.** Sync your fork, open `project/warden-03-starter/`, and type the encounter into it as the video builds it — pause, compile, and run every time the video does.
2. **Make sure it builds and runs.** `battle trees` should start the encounter, and both endings should be reachable. The Warden's own checks further down this page are a quick way to confirm.
3. **Submit your code** — push to GitHub, then turn it in on Canvas. See [Submission](#submission) below.

That is everything. The rest of this page describes what the videos build, so you can check your version against it.

## The encounter — *The Masked Menagerie*

This is what the videos build. You will program the **Warden of the Trees** into your dungeon app as a real, playable boss encounter, and you will fight it. The Warden does not strike you directly at first — it marshals a line of the Hold's beasts, wearing them as stolen faces in the order McCown's classification tree hands them out, and you cannot touch the Warden while that menagerie stands.

**Do.** In the Warden III starter (`project/warden-03-starter/`), the **`battle trees`** command is already wired to `runTreeWarden` in `battle/Battle.cpp`, alongside the Warden 1 `runWardenBattle` the file already carries. You write `runTreeWarden`. When the player types `battle trees`, the encounter begins with a fixed menagerie of five masks.

**You will receive (in your starter drop).** The public drop at `project/warden-03-starter/` ships everything the gate assumes, so the only thing left to build is the encounter itself:

- The **post-Floor-11 reference baseline** — the general `Tree` (`taxonomy`/`lineage`), the Floor 9 `BST` + Floor 9½ balancing (`catalogue`/`recall`/`balance`), the Floor 10 `Registry` (`std::set`/`std::map`), the finished Floor 11 `HashTable` (`whisper`/`banish`/`load`), every `selftest`, and Warden 1's `runWardenBattle`, all working.
- **`battle/Battle.cpp`** with the `runTreeWarden` TODO scaffold — and, in its comments, the per-tie checklist, a suggested outline, and the 3-phase self-check.
- `main.cpp` and `battle/Battle.h` with **`battle trees` already wired**, and a **README** that also lists the wiring diffs, in case you'd rather build in your own Floor 11 project. **There is no shared-header field to add this time (that was Warden 2's `UndoAction`), and no `CMakeLists.txt` change** — the `Enemy` struct and its `identified`/`exposed` flags live entirely inside `Battle.cpp`, and the four indexes are passed straight through by `const&`.

The videos work in this starter. If you'd rather build in your own Floor 11 project instead, the starter's README lists the three small wiring changes to make first.

**What the encounter does:**

- **Stats.** Player starts at **45 HP**. Each mask starts as just a *name* — its HP, ATK, and weakness are unknown until you identify it. Chip strike (weakness not exposed) is **4** and the mask retaliates its ATK; an exposed strike is **14** with **no** retaliation. Ward, Recall, and Classify each cost **1** retaliation from the current target; Recall costs **+1** when the catalogue leans (it does). Whisper and Study are **free**. The Warden itself has HP **24**, ATK **5**. Numbers are yours to tune; document them.
- **The command.** `battle trees` — **no argument**. The menagerie is a fixed five.
- **The menagerie [F8].** `buildMenagerie` walks `taxonomy.levelOrder()` (breadth-first), keeps only the leaf names the catalogue knows, and enqueues them into a `Queue<Enemy>`. The **front** of that line is the current mask. Note in a one-line comment that `preOrder()` (DFS) would send a different mask forward first.
- **The banish [F11 + forces F9].** At encounter start the Warden *"tears one name from the Hall of Whispers."* Make a **local copy** — `HashTable localWhispers = whispers;` — **then** `erase` **Bone Spider** from the copy. Your battle must **not** mutate the session-wide index: the shared `whispers` stays untouched, so the same beast is still whisperable from the main prompt after the fight. Bone Spider is the first level-order leaf, so whispering it returns `nullptr` and you *must* Recall it.
- **Beast menu — six options.** (1) **Whisper the front mask** *[F11, FREE]* — `whispers.find(name)` fills its stats instantly, no retaliation; the banished mask's niche is silent. (2) **Recall the front mask** *[F9 + F9½]* — `catalogue.find(probe)` fills the same stats the slow way, then prints the balance readout and takes the lean tax. (3) **Ward its weakness** *[F10]* — expose the mask by naming its true weakness. (4) **Strike**. (5) **Study the menagerie** *[FREE]* — the remaining order + your event log. (6) **Flee**. **Strike and Ward require the front mask to be identified first** — an unread mask gives a free guidance line, no beat lost.
- **Warden phase — four options.** When the menagerie is spent the menu swaps to **Classify / Strike / Study / Flee** — Whisper, Recall, and Ward have no purchase on a thing that is not a beast, and there is no Use-item option in this encounter. The Warden is untouchable until **Classify** traces a complete, correct root-to-leaf `lineage` of any Hold beast; then a full 14 Strike lands. Chip (4) is the slow fallback.
- **Win / lose / flee.** Menagerie cleared **and** Warden HP ≤ 0 → Victory (report surviving HP in the closing line). Player HP ≤ 0 → Defeat. Flee → the Warden does not chase; its menagerie still stands.

**How it ties back to Floors 8 – 11** — the videos wire one tie from each floor into the battle code: Each names the exact call:

- **Floor 8 (tree / taxonomy / lineage):** `taxonomy.levelOrder()` builds the menagerie's arrival order, and `taxonomy.lineage(name)` exposes the Warden. `preOrder()` is the DFS alternative. The menagerie is *walked from the tree*, never hardcoded.
- **Floors 9 / 9½ (BST + balance):** `catalogue.find(probe)` is the slow Recall — build the probe with only the name set, exactly as your `recall` command does. The balance judgement gates the lean penalty via `catalogue.isBalanced()`, `catalogue.height()`, and `catalogue.balanceFactor()`. The real catalogue leans, so Recall pays the extra hit; the `balance` command reports it in as many words:

  ```
    McCown's catalogue: 15 names, height 6, root balance factor -3 — NOT balanced (lopsided).
  ```

- **Floor 10 (sets & maps):** `registry.hasWeakness(w)` (a `std::set` membership) validates a Ward; `registry.countWeakness(w)` (a `const std::map` `find`, Phantom-Key-proof) is the "how many kin fear this" hint; `registry.weaknesses()` prints the arsenal.
- **Floor 11 (hash table — the whisper):** `whispers.find(name)` is the free O(1) identify; the banish uses `localWhispers.erase(name)` on the local copy so one beast is forced onto the Recall path. The apparent overlap between Recall and Whisper is the *point* — the associative-run contrast, whisper (free) vs walk (costs a beat) — and the banish makes it tactically real.

**Demo target.** Below is a real transcript of the reference encounter (`battle trees`), captured over piped input — so what the player typed is not echoed after the `>` prompt, and the bracketed lines are trims and notes, not program output. It shows the required *behaviours*, not required text — your wording, your numbers, and your menu order may all differ. (The reference reports the hero's surviving HP in its closing line; your win condition asks for the same.)

```
  The Warden of the Trees rises — a hooded figure wearing the Hold's
  beasts as stolen faces. It cannot be struck while its menagerie stands.
  It tears one name from the Hall of Whispers: "Bone Spider" — banished.
  The masks marshal in McCown's level-order (BFS):
    1. Bone Spider
    2. Ratking
    3. Marsh Lurker
    4. Stone Sentinel
    5. Skeleton
  (pre-order — DFS — would send a different mask forward first.)

  -- Your turn --   you HP 45    masks remaining: 5
    front mask: Bone Spider  (unread — Whisper or Recall it)
    1. Whisper the front mask (free)
    2. Recall the front mask (the leaning shelf)
    3. Ward its weakness
    4. Strike
    5. Study the menagerie (free)
    6. Flee
  >   index 9 out of bounds for size 6 — choose a listed option.
[the same six-option menu prints under every state line below; the Warden phase swaps in a four-option menu — trimmed from here down]

  -- Your turn --   you HP 45    masks remaining: 5
    front mask: Bone Spider  (unread — Whisper or Recall it)
  >   You whisper "Bone Spider" — the niche is silent. Recall it instead.

  -- Your turn --   you HP 45    masks remaining: 5
    front mask: Bone Spider  (unread — Whisper or Recall it)
  >   You walk the leaning shelf down to Bone Spider:
    HP 6, ATK 2, weakness fire.
    the catalogue reads: height 6, balance factor -3, NOT balanced.
    the long walk costs 2 retaliations of 2 = 4.  Your HP -> 41.

  -- Your turn --   you HP 41    masks remaining: 5
    front mask: Bone Spider  (HP 6, ATK 2, weakness fire)
  >   Your wards (the distinct weaknesses): blunt, fire, holy, light, slashing, water
  Ward which weakness?
  >   6 of the Hold's kin fear fire.
  Bone Spider's mask cracks — it is EXPOSED.
  Bone Spider strikes as you reach for the rack: 2.  Your HP -> 39.

  -- Your turn --   you HP 39    masks remaining: 5
    front mask: Bone Spider  (HP 6, ATK 2, weakness fire, EXPOSED)
  >   You strike Bone Spider true for 14.  HP -> 0.
  Bone Spider falls. The next mask steps forward.

  -- Your turn --   you HP 39    masks remaining: 4
    front mask: Ratking  (unread — Whisper or Recall it)
  >   You whisper "Ratking" and the niche answers at once:
    HP 9, ATK 2, weakness fire.  (free — a whisper does not walk.)
[... Ratking is Warded on fire and struck down for 14; then the three masks behind it — Marsh Lurker (HP 18, ATK 3, fire), Stone Sentinel (HP 30, ATK 5, blunt), and Skeleton (HP 10, ATK 3, blunt) — are each cleared the same way: a free Whisper identifies, a Ward on the true weakness EXPOSES it (one retaliation), then exposed 14-damage Strikes fell it with no retaliation between. Stone Sentinel, the tank, takes three exposed strikes. Player HP falls to 37 (Ratking), then 34, 29, 26 across the three that follow. Then the last mask, Skeleton, falls and the Warden stands alone: ...]
  Skeleton falls. The next mask steps forward.
  The menagerie is spent. The Warden stands alone —
  CLASSIFY a lineage to unmask it.

  -- Your turn --   you HP 26    Warden HP 24
    the menagerie is spent — the Warden stands shrouded (classify it).
    1. Classify a beast's lineage (expose the Warden)
    2. Strike
    3. Study (free)
    4. Flee
  >   Name a Hold beast to classify:
  >   Lineage: Creatures of the Hold > Undead > Corporeal > Skeleton
  You trace the order true. The Warden's stolen faces peel away — it is EXPOSED.
  The Warden lashes out for 5.  Your HP -> 21.

  -- Your turn --   you HP 21    Warden HP 24
    the menagerie is spent — the Warden stands UNMASKED (exposed).
  >   You strike the unmasked Warden for 14.  Warden HP -> 10.

  -- Your turn --   you HP 21    Warden HP 10
    the menagerie is spent — the Warden stands UNMASKED (exposed).
  >   You strike the unmasked Warden for 14.  Warden HP -> 0.

  The Warden's last stolen face falls away; you stand over an empty
  suit of iron.  You end at 21 HP.

The menagerie is unmasked. The Warden falls. The Archive stair opens.
```

And the other ending, so you know both are reachable — the final beat of a chip-only run (never Ward, so every surviving chip draws the mask's ATK back), dying to Stone Sentinel, the tank:

```
  -- Your turn --   you HP 3    masks remaining: 2
    front mask: Stone Sentinel  (HP 14, ATK 5, weakness blunt)
    1. Whisper the front mask (free)
    2. Recall the front mask (the leaning shelf)
    3. Ward its weakness
    4. Strike
    5. Study the menagerie (free)
    6. Flee
  >   You chip Stone Sentinel for 4.  HP -> 10.
  Stone Sentinel retaliates for 5.  Your HP -> 0.

  Your guard fails. The Warden lifts your face to wear among the rest.

The Warden files you among its faces. The stair stays shut.
```

<div class="callout check" markdown="1">
<p class="callout-title">The Warden's own checks — run these before you push</p>

A quick self-check, also printed in the header comments of the starter's `battle/Battle.cpp`. Every line is something you can verify at your own keyboard.

**Phase 1 — the skeleton holds.**
- `battle trees` starts an encounter; the menagerie steps forward in level-order (**Bone Spider** first); the state line prints every beat.
- Both endings are reachable: win one run, and lose one on purpose (chip the tank only, or drop your start HP).

**Phase 2 — the gate holds.**
- Whisper and Study are **free** — no retaliation, same beat.
- Typing `9` or `abc` at the menu re-prompts the *same* beat without crashing; a mistyped weakness in Ward errors cleanly and costs no beat.

**Phase 3 — the four indexes hold.**
- Put a finger on each tie in your source: `[F8]` `levelOrder`/`lineage`; `[F9]` `catalogue.find` + `[F9½]` `isBalanced`/`height`; `[F10]` `hasWeakness`/`countWeakness`; `[F11]` `whispers.find`.
- Confirm the banished beast forces a Recall — and that the shared `whispers` is untouched (whisper Bone Spider from the main prompt *after* the fight; it is still there).

If all three phases pass at your keyboard, your encounter matches the video's and you are ready to submit.

</div>

## Submission

**The due date is posted on Canvas.**

1. **Push to GitHub.** Commit the code you built along with the videos — `battle/Battle.cpp` (plus `main.cpp` and `battle/Battle.h` if you worked in your own Floor 11 project) — and push it to your fork of the course repo.
2. **Submit on Canvas.** Turn in the Warden III assignment with the link to your GitHub repo.

Your code must build. If it does not compile, the Warden has won by default.

**Folder + code shape:**

```
[your fork]/
  project/warden-03-starter/
    battle/Battle.cpp                 <-- runTreeWarden(...): the code you write
    main.cpp, battle/Battle.h         <-- `battle trees` already wired (no edits)
    hero/Tree.h                       <-- levelOrder / preOrder / lineage (Floor 8)
    hero/BST.h                        <-- find / height / balanceFactor / isBalanced (Floors 9, 9½)
    bestiary/Registry.h               <-- hasWeakness / countWeakness / weaknesses (Floor 10)
    hero/HashTable.h                  <-- find / erase (Floor 11)
```

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/faith.svg' | relative_url }}" alt="Brother Faith, Abbot of the Sorted Tome — bearded monk in a dark brown robe with a brass clasp and a stopwatch on a brass cord."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Faith, thumb on his stopwatch at the gate</span>

There is no forge-keeper for this stretch — McCown kept all five halls himself, and he does not come this low. So wield what he taught you, one index at a time. Walk the tree first and watch a mask step forward; nothing else. Compile. Run. *Then* the whisper, free, and the recall that pays for the lean. Compile. Run. *Then* the ward, and the strike that lands when the mask is cracked. Compile. Run. *Then* the lineage that unmasks the Warden. A hero who wires all five halls at once has five bugs at once and cannot say which hall owns which. Add the next index only after the last one held. That is the whole of my advice.
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Warden of the Trees</p>

**The Warden of the Trees** — *HP: 24. ATK: 5. Untouchable while its stolen menagerie stands; exposed only by a complete, correct root-to-leaf lineage. Weakness: a hero who re-imposes the order it stole.*

The third of four wardens. Cataloguers never caught its face, because it has none of its own — the page pinned to its entry shows armour, four sigils (a branching tree, a search-fork, a name-rack, a honeycomb of niches), a forked ember where eyes should be, and a struck-through name slot. It is the Unsorted Plague's answer to a cataloguer: the shadow of ordering. It wears the Hold's beasts as stolen faces, in the exact level-order McCown's classification tree hands them out, and it cannot be struck while that menagerie stands. Break each mask by naming the beast beneath it — the free whisper if the niche still holds it, the slow recall down the leaning shelf if the Warden has banished it. Then, when the menagerie is spent, do the one thing the Plague cannot survive: trace a beast root to leaf, and the stolen faces peel away. *The shadow of ordering is undone by ordering.*

McCown does not stand at this gate — he kept order while there was order to keep, and there is none here. But his five halls are your arsenal, and every tie in your battle is one of his lessons pointed back at the thing that stole it.

Counter by:

- **Whisper before you recall.** The whisper is free — no retaliation, a computed jump to the niche. The recall walks the shelf and costs a beat, *plus* an extra beat while the catalogue leans. Recall only the mask the Warden banished; whisper the rest.
- **The catalogue leans — prefer the whisper, and never hand-roll a fix.** McCown's real catalogue reads *height 6, balance factor −3, NOT balanced*. One rotation at the root will not level a deep stick; that is why you reach for the library and the hash table, not for a hand-rolled rebalance. (F9½ / Leaning Tower.)
- **Banish on a local copy — never mutate the session index.** `HashTable localWhispers = whispers;` **then** `erase`. Erase the shared `whispers` and every later whisper in the game lies. Four independent indexes are allowed to diverge; corrupting the shared one is not. (F11 / The Collision.)
- **Identify before you strike.** An unread mask carries no ATK and no weakness — you cannot Ward it and your blade slides off it. Whisper or Recall first, then Ward, then strike true. (F10 / Phantom Key: Ward with `find`/`count`, never `operator[]`.)
- **Catch inside the loop, not around it.** A bad menu index or an unknown weakness must re-prompt the *same* beat, not forfeit the fight. A `catch` around the whole encounter unmasks nothing and ends everything.

</div>

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix has a study tip</span>

*Friend!* The masks — easy! Skip all the whispering and warding nonsense — *just chip everything, friend!* Four damage a swing, over and over, down the whole line! (It is four damage a swing until the tank steps forward. Stone Sentinel has 30 HP: that is eight chips to fell it, and every chip you don't kill on draws its ATK 5 straight back — seven retaliations, thirty-five damage, more than your whole bar, from *one* mask. Ward it once and a single 14-strike ends it for one hit of retaliation. Grix has been chipping since Floor 8. Grix's hero is at zero.)
</div>
</div>

---

*The gate answers one way or the other. The Warden does not announce the result; the Archive stair opens, or it does not. Beyond the stair the last hall waits — the **[Labyrinth]({{ site.baseurl }}/floors/floor-12/)** (Floor 12, graphs), which opens this same week and is the finale; the Queue and the Stack you already built become the two ways to walk it. The Nameless Bard tunes a low string and does not sing your ending yet. He is saving the verse — for the bottom, where no warden stands in the Hold at all, only a last reckoning on paper: the **Architect of Errors**, who has been reading over your shoulder the whole way down.*
