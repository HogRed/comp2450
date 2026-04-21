// COMP 2450 — Floor 2 starter
// hero/Sort.cpp — YOU implement this file.
//
// Three functions to write. Read Sort.h for their contracts.
//
// Tips for the Pivot Wraith:
//   * If you pick the FIRST element as your quicksort pivot, a sorted
//     input becomes O(n^2). The `benchmark sort --bad-pivot --sorted`
//     harness exists to show you exactly that.
//   * The middle element is the cheapest defense. Good enough for this
//     week. Real production code (std::sort) does median-of-three and
//     switches algorithms on bad recursion depth.
//
// Submit when:  `sort inventory by weight` produces ascending weight,
//               `sort inventory by name desc` produces reverse alphabetical,
//               and `benchmark sort` gives three timing columns on every row.

#include "Sort.h"
#include <algorithm>  // you will want std::sort in sortInventory

namespace dungeon {

// ---- 1. Merge sort ------------------------------------------------------

void mergeSort(std::vector<Item>& inventory, const Comparator& cmp) {
    // TODO Floor 2 (Mon): implement merge sort.
    //
    // Recommended pattern — write two helpers in an anonymous namespace
    // above this function:
    //
    //   static void mergeSortImpl(std::vector<Item>& v,
    //                             std::size_t lo, std::size_t hi,
    //                             const Comparator& cmp);
    //   static void merge         (std::vector<Item>& v,
    //                             std::size_t lo, std::size_t mid,
    //                             std::size_t hi,
    //                             const Comparator& cmp);
    //
    // mergeSort() itself just calls mergeSortImpl(v, 0, v.size(), cmp).
    //
    // Half-open range convention: [lo, hi) — hi is ONE PAST the last
    // valid index. This avoids size_t underflow at the low end.
    //
    // In merge(): copy both halves into a scratch buffer as you walk
    // them, always taking the smaller front element. On a tie take
    // from the LEFT half — that keeps the sort stable.
    (void)inventory;
    (void)cmp;
}

// ---- 2. Quicksort -------------------------------------------------------

void quicksort(std::vector<Item>& inventory, const Comparator& cmp) {
    // TODO Floor 2 (Wed): implement quicksort.
    //
    // Recommended pattern — helpers in an anonymous namespace:
    //
    //   static std::size_t partition  (std::vector<Item>& v,
    //                                  std::size_t lo, std::size_t hi,
    //                                  const Comparator& cmp);
    //   static void        quicksortImpl(std::vector<Item>& v,
    //                                    std::size_t lo, std::size_t hi,
    //                                    const Comparator& cmp);
    //
    // Closed range convention for quicksort: [lo, hi] — both inclusive.
    // Textbooks use this for Lomuto partition; it is fine here. Guard
    // the recursive call `quicksortImpl(v, lo, p - 1, cmp)` with
    // `if (p > lo) ...` so you do not underflow when p == 0.
    //
    // PIVOT: use the middle element — `lo + (hi - lo) / 2`. Move it to
    //        the end (swap it with v[hi]) and then do the standard
    //        Lomuto scan with the pivot now at v[hi].
    //
    // If you are curious what the FIRST-element pivot looks like: the
    // benchmark harness has a `--bad-pivot` option that runs exactly
    // that. You do NOT need to implement it yourself; the harness
    // ships its own copy for Lab purposes.
    (void)inventory;
    (void)cmp;
}

// ---- 3. sortInventory (the seam) ----------------------------------------

bool sortInventory(Hero& hero, const std::string& criterion) {
    // TODO Floor 2 (Fri): parse criterion, build the right comparator,
    // dispatch to a sort.
    //
    // Criterion looks like "weight"  or  "name desc"  or  "value asc".
    // You can parse it with std::istringstream:
    //
    //     std::istringstream in(criterion);
    //     std::string key, dir;
    //     in >> key >> dir;       // dir is "" if absent
    //
    // Build an ascending Comparator for each key ("name", "weight",
    // "value"). If dir == "desc", wrap it in a Comparator that swaps
    // the arguments of the ascending one.
    //
    // Return false on an unknown key (main.cpp will print an error).
    //
    // Dispatch: for this week std::sort is the right production choice.
    // Your mergeSort and quicksort are correct too — pick one and
    // defend it in your commit message. (`std::sort` is the answer we
    // converge on as a class on Friday; implementing it here is fine.)
    (void)hero;
    (void)criterion;
    return false;
}

}  // namespace dungeon
