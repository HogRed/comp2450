// COMP 2450 — Floor 2 starter
// hero/Sort.cpp — YOU implement this file.
//
// Three functions to write. Read Sort.h for their contracts.
//
// The big idea this week: sorting is not one algorithm, it is a family
// of tradeoffs. Merge sort is predictable but copies. Quicksort is fast
// on average but betrays you on bad inputs. std::sort is what you
// actually ship. You will write the first two, race all three in
// `benchmark sort`, and argue — in a commit message — which one the
// game should call.
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
#include <sstream>

namespace dungeon {

// ---- 1. Merge sort ------------------------------------------------------

void merge(std::vector<Item>& v, std::vector<Item>& scratch,
        std::size_t lo, std::size_t mid, std::size_t hi,
        const Comparator& cmp) {
        std::size_t i = lo, j = mid, k = lo;
        while (i < mid && j < hi) {
            if (cmp(v[j], v[i])) scratch[k++] = v[j++];   // right strictly smaller
            else                 scratch[k++] = v[i++];   // left smaller or tie
        }
        while (i < mid) scratch[k++] = v[i++];
        while (j < hi)  scratch[k++] = v[j++];
        for (k = lo; k < hi; ++k) v[k] = scratch[k];
    }
void mergeSortRange(std::vector<Item>& v, std::vector<Item>& scratch,
        std::size_t lo, std::size_t hi, const Comparator& cmp) {
        if (hi - lo < 2) return;                          // base case: 0 or 1 items
        std::size_t mid = lo + (hi - lo) / 2;
        mergeSortRange(v, scratch, lo, mid, cmp);
        mergeSortRange(v, scratch, mid, hi, cmp);
        merge(v, scratch, lo, mid, hi, cmp);
    }

void mergeSort(std::vector<Item>& inventory, const Comparator& cmp) {
    // TODO Floor 2 (Mon): implement merge sort.
    //
    // Think before you type:
    //   - Merge sort is STABLE. That means: when two items compare equal
    //     (same weight, say), the one that started earlier stays earlier.
    //     The starter inventory has Iron key and Loaf of bread both at
    //     weight 0.1, Iron key first. After `sort inventory by weight`,
    //     which must come first? *Which line in your merge code is the
    //     one that enforces that?* (Hint: the tie-breaking comparison.)
    //   - Merge sort needs O(n) scratch space to merge. Could you merge
    //     two sorted halves in place without extra memory? Yes — and it's
    //     wildly slower. Don't try. The scratch buffer IS the algorithm.
    //   - Base case for a half-open range [lo, hi): when does the range
    //     hold zero or one element? That range is already "sorted" —
    //     return immediately.
    //   - Computing mid: `lo + (hi - lo) / 2`, not `(lo + hi) / 2`. Same
    //     overflow habit you built in Floor 1.
    //
    // If you need structural hints — write two helpers in an anonymous
    // namespace above this function:
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
    // In merge(): copy both halves into a scratch buffer, then walk both
    // halves taking the smaller front element. On a tie take from the
    // LEFT half — that one line is what keeps the sort stable.
    std::vector<Item> scratch(inventory.size());
    mergeSortRange(inventory, scratch, 0, inventory.size(), cmp);
}

// ---- 2. Quicksort -------------------------------------------------------

std::ptrdiff_t partition(std::vector<Item>& v, std::ptrdiff_t lo,
    std::ptrdiff_t hi, const Comparator& cmp) {
    const Item pivot = v[lo + (hi - lo) / 2];         
    std::ptrdiff_t i = lo - 1;
    std::ptrdiff_t j = hi + 1;
    for (;;) {
        do { ++i; } while (cmp(v[i], pivot));         
        do { --j; } while (cmp(pivot, v[j]));         
        if (i >= j) return j;
        std::swap(v[i], v[j]);
    }
}

void quicksortRange(std::vector<Item>& v, std::ptrdiff_t lo,
    std::ptrdiff_t hi, const Comparator& cmp) {
    if (lo >= hi) return;                           
    std::ptrdiff_t p = partition(v, lo, hi, cmp);
    quicksortRange(v, lo, p, cmp);
    quicksortRange(v, p + 1, hi, cmp);
}

void quicksort(std::vector<Item>& inventory, const Comparator& cmp) {
    // TODO Floor 2 (Wed): implement quicksort.
    //
    // Think before you type:
    //   - Quicksort's whole performance story depends on the PIVOT. If
    //     the pivot splits the range roughly in half each time, you get
    //     O(n log n). If the pivot always ends up at one end (everything
    //     goes to one side), you get O(n^2). Why does the FIRST element
    //     cause that on sorted input? Sketch it on paper for [1,2,3,4,5].
    //   - Your fix is the MIDDLE element. It's not bulletproof — an
    //     adversary could still construct a worst-case input — but it
    //     kills the most common pathology (sorted / reverse-sorted data),
    //     which is exactly the shape real users produce.
    //   - `std::size_t` is unsigned. When `p == 0`, what is `p - 1`?
    //     That wrap-around will send your left-side recursion to index
    //     18 quintillion. Guard it.
    //   - Is quicksort stable? (Answer: no — and that is why production
    //     std::sort is ALSO not stable. If you need stability, reach for
    //     std::stable_sort or your mergeSort.)
    //
    // If you need structural hints — helpers in an anonymous namespace:
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
    quicksortRange(inventory, 0,
        static_cast<std::ptrdiff_t>(inventory.size()) - 1, cmp);
}

// ---- 3. sortInventory (the seam) ----------------------------------------

bool sortInventory(Hero& hero, const std::string& criterion) {
    std::istringstream in(criterion);
        std::string key, dir;
        in >> key >> dir;
    if (dir.empty()) dir = "asc";
    if (dir != "asc" && dir != "desc") return false;
    std::string extra;
    if (in >> extra) return false;                   
    Comparator asc;
    if (key == "name")        asc = [](const Item& a, const Item& b) { return a.name < b.name;   };
    else if (key == "weight") asc = [](const Item& a, const Item& b) { return a.weight < b.weight; };
    else if (key == "value")  asc = [](const Item& a, const Item& b) { return a.value < b.value;  };
    else return false;

    Comparator cmp = asc;
    if (dir == "desc") cmp = [asc](const Item& a, const Item& b) { return asc(b, a); };

    mergeSort(hero.inventory, cmp);
    return true;
}

}  // namespace dungeon
