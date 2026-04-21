// COMP 2450 — Floor 3 starter
// hero/Sort.cpp — reference implementation from Floor 2.
//
// This file IS already done. You wrote it last week (or something like it);
// the starter ships the reference version so `sort inventory by …` still
// works while you focus on templates and exceptions this week. Read it
// as a worked example — every choice below is defensible, and the
// comments say WHY.

#include "Sort.h"
#include <algorithm>
#include <sstream>

namespace dungeon {

// ---- 1. Merge sort ------------------------------------------------------
//
// Stable, O(n log n) always, O(n) scratch space. The stability-preserving
// line is the merge tie-break: on equal elements we take from the LEFT
// half first, which is why Iron key (weight 0.1, listed first) stays
// ahead of Loaf of bread (weight 0.1, listed second) after a `sort by
// weight`. Lose that and you lose stability.

namespace {

void merge(std::vector<Item>& v,
           std::size_t lo, std::size_t mid, std::size_t hi,
           const Comparator& cmp) {
    // Copy both halves out. Merging in place is possible but much slower;
    // the scratch buffer IS the algorithm.
    std::vector<Item> left (v.begin() + lo,  v.begin() + mid);
    std::vector<Item> right(v.begin() + mid, v.begin() + hi);

    std::size_t i = 0, j = 0, k = lo;
    while (i < left.size() && j < right.size()) {
        // Stability is hiding in THIS condition. `!cmp(right[j], left[i])`
        // evaluates to "right[j] is not strictly less than left[i]" — so
        // on a tie, we take from left first. If you wrote
        // `cmp(left[i], right[j])` instead, equal elements from the
        // right half would sneak ahead. Same Big-O, broken stability.
        if (!cmp(right[j], left[i])) {
            v[k++] = left[i++];
        } else {
            v[k++] = right[j++];
        }
    }
    while (i < left.size())  v[k++] = left[i++];
    while (j < right.size()) v[k++] = right[j++];
}

void mergeSortImpl(std::vector<Item>& v,
                   std::size_t lo, std::size_t hi,
                   const Comparator& cmp) {
    // Base case for the half-open [lo, hi) form: zero or one element.
    if (hi - lo < 2) return;
    std::size_t mid = lo + (hi - lo) / 2;
    mergeSortImpl(v, lo,  mid, cmp);
    mergeSortImpl(v, mid, hi,  cmp);
    merge        (v, lo,  mid, hi, cmp);
}

}  // anonymous namespace

void mergeSort(std::vector<Item>& inventory, const Comparator& cmp) {
    mergeSortImpl(inventory, 0, inventory.size(), cmp);
}

// ---- 2. Quicksort -------------------------------------------------------
//
// Middle-element pivot, Lomuto partition, closed range [lo, hi]. Unstable
// (swaps across ties), O(n log n) average, O(n^2) worst. The middle-
// element pivot kills the sorted-input pathology that a first-element
// pivot would hit — see Bench.cpp's badQuicksort for what that looks
// like when it goes wrong.

namespace {

std::size_t partition(std::vector<Item>& v,
                      std::size_t lo, std::size_t hi,
                      const Comparator& cmp) {
    // Pick the MIDDLE element as pivot (the defense against sorted
    // input) and swap it to the end so the standard Lomuto scan — which
    // assumes the pivot is at v[hi] — still works.
    std::size_t mid = lo + (hi - lo) / 2;
    std::swap(v[mid], v[hi]);
    const Item pivot = v[hi];

    std::size_t store = lo;
    for (std::size_t i = lo; i < hi; ++i) {
        if (cmp(v[i], pivot)) {
            std::swap(v[store], v[i]);
            ++store;
        }
    }
    std::swap(v[store], v[hi]);
    return store;
}

void quicksortImpl(std::vector<Item>& v,
                   std::size_t lo, std::size_t hi,
                   const Comparator& cmp) {
    if (lo >= hi) return;
    std::size_t p = partition(v, lo, hi, cmp);
    // `p - 1` with std::size_t underflows when p == 0 — 18 quintillion
    // and counting. Guard it.
    if (p > lo) quicksortImpl(v, lo, p - 1, cmp);
    quicksortImpl(v, p + 1, hi, cmp);
}

}  // anonymous namespace

void quicksort(std::vector<Item>& inventory, const Comparator& cmp) {
    if (inventory.size() < 2) return;
    quicksortImpl(inventory, 0, inventory.size() - 1, cmp);
}

// ---- 3. sortInventory (the seam) ---------------------------------------
//
// Parse the "weight desc" / "name" / "value asc" string the user typed,
// build the right comparator, dispatch to one sort. std::sort is the
// production pick — smaller constants, cache-aware, introsort fallback.
// Your two hand-rolled sorts exist to TEACH the shape; std::sort exists
// to SHIP. The whole game calls only through this function, so if you
// ever want to change the pick, one line here flips it.

bool sortInventory(Hero& hero, const std::string& criterion) {
    std::istringstream in(criterion);
    std::string key, dir;
    in >> key >> dir;

    Comparator asc;
    if      (key == "name")   asc = [](const Item& a, const Item& b){ return a.name   < b.name;   };
    else if (key == "weight") asc = [](const Item& a, const Item& b){ return a.weight < b.weight; };
    else if (key == "value")  asc = [](const Item& a, const Item& b){ return a.value  < b.value;  };
    else return false;

    // A descending comparator is an ascending one with the arguments
    // swapped. Two-line lambda. No need for a second family of
    // comparators.
    Comparator cmp = asc;
    if (dir == "desc") {
        cmp = [asc](const Item& a, const Item& b) { return asc(b, a); };
    }

    std::sort(hero.inventory.begin(), hero.inventory.end(), cmp);
    return true;
}

}  // namespace dungeon
