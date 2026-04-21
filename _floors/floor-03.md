---
title: The Forgemaster's Vault
floor_number: Floor 3
flavor_quote: "One mold. Every chest. Every monster. Every name."
week: 4
zybook: "Chapters 4 & 5"
---

## The briefing

The stair from the Crucible's cooling embers descends into a vaulted stone chamber of empty racks, cold crucibles, and **brass molds** — the **Forgemaster's Vault**. In the center stands the **Forgemaster**, silent, a hammer at his belt. He turns and speaks.

*"You have sorted your spoils. But look at the racks — your inventory, the keep's bestiary, the spellbook yet to come. Three lists. Three shapes. Three separate codes. A waste. This week we press one mold. Stamp it for Items. Stamp it for Monsters. And when a caller asks for steel that is not in the chest — the forge does not crack. It cries out. The game above hears, and recovers, and continues."*

Last week you sorted. This week you build the **container shape that stamps out a list for any kind of thing**. By Friday, your hero's inventory and the keep's bestiary will both live inside a `Bag<T>` — one class template you wrote, instantiated twice. From the outside, nothing changes. From the inside, one piece of code does the work of two.

You will learn two things, and they are inseparable:

1. **Templates** — one mold, any metal. Write `Bag<T>` once; the compiler stamps a fresh copy for every `T` you instantiate. Function templates first (`findByName<T>` generalizes Floor 1's linear search); class templates next (`Bag<T>` replaces both `std::vector<Item>` and `std::vector<Monster>`).
2. **Exceptions** — when a caller asks for `inventory.at(999)` on a five-item bag, what does the function *return*? Nothing sensible. It **throws** a `BagException`. The main loop catches. The game keeps going. The caller knows exactly what went wrong.

You are not rewriting `std::vector` this week — `Bag<T>` is a thin wrapper over it. The point is the **template machinery**, not the storage.

## Objectives

By the end of Floor 3 you will be able to:

- Write a **function template** from scratch — `template<typename T> const T* findByName(const Bag<T>&, const std::string&)`.
- Write a **class template** from scratch — `template<typename T> class Bag`.
- Instantiate the same class template with two different types in the same program and point to where the compiler stamped each copy.
- Define a **custom exception** that inherits from `std::exception` and overrides `what()`.
- Distinguish `operator[]` (no bounds check, fast) from `at()` (bounds check, throws).
- Catch a thrown exception at the right level; predict what happens if the exception escapes `main`.
- Read a wall-of-text template compiler error and locate the one line that actually tells you what went wrong.

## Pre-class

### Reading (ZyBook Ch. 4 & 5)

Before **Monday**: §4.1 Function templates; §4.2 Class templates
Before **Wednesday**: §4.3 C++ example — map values using a function template (the worked example ties Monday and Wednesday together)
Before **Friday**: §5.1 Handling exceptions (`try` / `catch`); §5.2 Throwing exceptions; §5.4 User-defined exceptions; skim §5.5 (a second worked example)

You can skip §5.3 (exceptions with files) — we do not touch files this week.

Work the Question Sets and Animations inside each section. The §4.2 animation of a class template being stamped for two different types is exactly what will happen to `Bag<T>` on Wednesday — watch it twice.

There are no pre-class videos. Class time is for live coding and discussion together — your reading is the prep.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | Function templates     | Live-code a generic `findByName<T>` together. Replace Floor 1's monster-only `linearSearch` with the template. Call it on the bestiary *and* on the hero's inventory — same source line, two stampings. Watch the compiler error when we call it on a `Bag<int>`. |
| **W** | Class templates        | Live-code `Bag<T>` from the starter declaration. Rewrite `Hero.h` so the inventory is `Bag<Item>`; rewrite the bestiary to be `Bag<Monster>`. Commit when `inventory`, `bestiary`, `search`, and `sort inventory` all run off one class template. |
| **F** | Exceptions             | Write `BagException : public std::exception`. Make `at()` throw on bad index. Wrap the main command loop in `try`/`catch`. Plant a deliberate bad call — watch the game *not* crash. Discussion: what must *not* go in a catch block, and why `catch(...) { }` is worse than no catch at all. |

## The project — Floor 3

This week's project increment is **one container shape shared across the whole game**.

You will receive (in your starter drop):

- Everything through Floor 2, fully working — `search`, `sort inventory by <criterion>`, all three benchmarks from Floor 1 and Floor 2.
- A header `hero/Bag.h` with the class template declaration — `template<typename T> class Bag`, private `std::vector<T>` member, and signatures for every method you need to implement.
- A header `hero/BagException.h` — a user-defined exception derived from `std::exception` with a `what()` message.
- A header `hero/Search.h` with the new `findByName<T>` function template declaration (Floor 1's monster-specific version is still there for comparison, marked *deprecated*).
- `hero/Hero.h` updated so the inventory is `Bag<Item>`; `bestiary/Bestiary.h` updated so monsters live in `Bag<Monster>`. Every existing command stops compiling until Monday's work is in. That is intentional.

You will write:

1. **Monday:** `findByName<T>` in `hero/Search.h` (templates live in headers). Rewire `main.cpp`'s `search` command to call the template. Demonstrate calling it on both `Bag<Monster>` and `Bag<Item>` from different commands.
2. **Wednesday:** `Bag<T>` — `add`, `size`, `empty`, `operator[]`, `begin`/`end` (delegate to the underlying `std::vector`), `clear`. When this is done, `inventory`, `sort inventory`, and `search` all run through it.
3. **Friday:** `BagException`, `Bag<T>::at()` that throws on bad index, and a `try`/`catch` around the main command dispatch in `main.cpp`. A bad `inspect 99` must print a clean error and return to the prompt.

Demo target (Friday):

```
> inventory
  1.  Rusty sword       (wt 4.0, val 5)
  2.  Healing potion    (wt 0.5, val 12)
  3.  Iron key          (wt 0.1, val 0)
  4.  Loaf of bread     (wt 0.1, val 1)
  5.  Cloak of shadows  (wt 1.5, val 80)
> bestiary
  1.  Goblin            HP 8   ATK 2   weakness: fire
  2.  Kobold            HP 5   ATK 1   weakness: cold
  3.  Lich              HP 30  ATK 6   weakness: radiant
  ...
  (same Bag<T> — different T)
> search Goblin
  Goblin   HP 8   ATK 2   weakness: fire
> search "Iron key"
  Iron key          (wt 0.1, val 0)
  (same findByName<T>, two instantiations)
> inspect 99
  No such item. (BagException: index 99 out of bounds for size 5)
  (game keeps running — the catch block handled it)
> sort inventory by weight
  (still works — now running on your Bag<Item>)
```

### Lab 3 — *One Mold, Many Shapes* (folded into the project)

There is no separate lab handout. The work you do this week *is* the lab.

Commit `floor-03/lab-notes.md` to your project repo with:

1. A transcript of the demo above. Paste it from your terminal.
2. **Instantiate `Bag<int>` in a throwaway test and call `findByName` on it.** Paste the full compiler error. Circle (or describe) the one line that actually tells you what's wrong. Why is this harder to read than a normal type error, and what does that tell you about templates?
3. **Swap `at()` for `operator[]`** in the `inspect` command. Pass a bad index. Describe what the program does now — does it crash? Print garbage? Silently return? Restore `at()` when done. One-sentence answer: when would you ever want `operator[]` over `at()` in production?
4. **Remove the `try`/`catch` around the main loop.** Trigger a bad `inspect`. Describe exactly what you see on stderr (the unhandled-exception message the runtime prints before `std::terminate`). Put the catch back. Then try the anti-pattern `catch(...) { }` — empty body — and describe what is now *worse* than having no catch at all.
5. **Change `BagException` so it does NOT inherit from `std::exception`.** What still compiles? What behavior (if any) do you lose? One sentence on why inheriting from `std::exception` is the right default.

Your commit history this week should show at least three commits — Mon (function template), Wed (class template), Fri (exceptions + lab notes).

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 3</p>

**The Swallowed Scream** — *HP: ∞. Damage: invisible, cumulative.*
Strikes when a programmer, panicked by a crash, wraps bad code in `catch(...) { }` to make the error "go away." The program no longer crashes. It also no longer works. Bugs are no longer reported. The inventory command silently does nothing half the time. A boss fight ends with the player at zero HP and no message. The Swallowed Scream does not kill you. It removes your ability to *notice* that something is killing you.

Counter by:

- **Catch at the level where you can actually do something about it.** The main command loop can say "command failed: {what}" and return to the prompt. A two-line helper in the middle of a subsystem usually cannot — let the exception propagate.
- **Never write `catch(...) { }` with an empty body.** At a minimum, log. Better: rethrow after logging. Best: don't catch at this level at all.
- **Inherit from `std::exception`.** The catch block can then ask `e.what()` and print something useful. A raw `throw 42;` is technically legal C++ and produces an error message a caller cannot read.
</div>

<div class="callout check" markdown="1">
<p class="callout-title">Check for understanding</p>

Before you face the first boss, you should be able to answer these without looking:

1. What does `template<typename T>` buy you that a non-templated class doesn't? Give a concrete example from *this week*: one class, two instantiations, both appearing in the same program.
2. Where does `Bag<T>`'s code have to live — `Bag.h` or `Bag.cpp`? Why?
3. Your function template uses `x.name` somewhere inside its body. You instantiate it with `T = int`. At what point do you find out there's a problem — writing the code, compiling, or running? Why?
4. `operator[]` does not bounds-check; `at()` does and throws. Give one situation in the game where you'd reach for each.
5. Your custom exception inherits from `std::exception` and overrides `what()`. What would change if it didn't inherit from anything — what still works, and what becomes awkward for the caller?
6. A thrown exception is not caught anywhere before it reaches `main`. What does the C++ runtime do? Demonstrate in your lab notes, and explain in one sentence why that default behavior is actually *good* — i.e., what it forces you to do.

Answers are discussed at the start of Floor 4 — but first, **Midterm 1** stands between you and Floor 4. The boss tests Floors 0 – 3 together. Prepare for it.
</div>

---

*The Forgemaster sets down his hammer. The mold cools. One forge has stamped two containers, and when the forge is asked for steel that isn't there, it cries out instead of cracking. Above the far door the first warden stirs — the **Boss of the Foundations** — where every lesson from the Antechamber through the Vault is put to the test. Pass, and the stair to Floor 4 — the **Chain Vault** — opens.*
