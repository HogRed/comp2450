# Trial I — *The Foundations Brief*

*Warden of the Foundations · Midterm 1 · 15 %*

Four short answers, one from each floor above. **50–100 words per answer.** No code on this trial (except the lambdas in Q3); just clear thinking.

AI is welcome to *check* your answers; it is not welcome to *write your sentences*. These answers are short enough that an LLM voice is recognisable.

---

## 1. Floor 0 — ADT

> Your battle's "Use item" menu shows the *currently usable* items in your hero's inventory on this turn. Name the right ADT for that menu. Defend the choice against its closest neighbour (e.g., why `bag` instead of `set`, or `list` instead of `bag`).

The right ADT is a List. A List maintain a linear order and allows duplicate elements, and that is essential because a player can carry multiple identical items (like two potions that are the same) and expects them to appear in a determinined display order. A Bag allows duplicates but lacks fixed ordering, while Set prevents multiple identical consumables from existing altogether. So, a ñList preserves order and duplicate items cleanly

---

## 2. Floor 1 — search & Big-O

> Your inventory is kept sorted by healing power (in this codebase an item's `value` measures its potency, so `value` plays the healing-power role). The player types `use Healing potion`. Linear or binary search to find it by name? Justify, and give the Big-O for each.

Use the Linear Search, because the inventory is kept sorted by healing power, which is value, but the player searches by name and because the collection is unsorted respecting to item names, Binary Search O(log n) cannot be used because it requires the keys to be sorted by the lookup attribute. So, a Linear Search O(n) that scan across the inventory is needed to match the string

---

## 3. Floor 2 — sort & comparators

> Your "Use item" menu must be displayable sorted *either* by healing power (meaning `value`) *or* by weight. Show a one-line comparator (lambda) for each. One sentence on what language feature makes one `std::sort` call serve both orders.

*Note: `Item` has no healing field — an item's `value` measures its potency, so `value` plays the healing-power role here and in your battle.*

```cpp
// by healing power — i.e. by value
auto byValue  = [](const Item& a, const Item& b) {return a.value < b.value;};

// by weight
auto byWeight = [](const Item& a, const Item& b) {return a.weight < b.weight;};
```

std::sort use templates to accept any custom call object such as a lambdas as its comparison parameter, that allows to the exact same sorting function to execute with different ordering logic.

---

## 4. Floor 3 — templates & exceptions

> Why does `Bag<T>` live in `Bag.h` instead of `Bag.cpp`? And: when the player types `9` for a 4-option menu, where in your code should the validation **throw**, and where should it **catch**?