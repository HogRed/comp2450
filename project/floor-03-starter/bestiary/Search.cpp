// COMP 2450 — Floor 2 starter
// bestiary/Search.cpp — reference implementation from Floor 1.
//
// This file IS already done. You wrote it last week (or something like it);
// the starter ships the reference version so the `search` command still
// works while you focus on Floor 2. Read it as a worked example — every
// choice below is defensible, and the comments say WHY.

#include "Search.h"

namespace dungeon {

// ---- 1. Linear search ---------------------------------------------------
//
// O(n). Works on any order — no precondition. For 15 monsters this is
// *faster* than binary search because the constants are smaller and
// cache behavior is better on a contiguous scan. Beginners assume binary
// is always better; this one line of code disproves that.

const Monster* linearSearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {
    // `const auto&` binds a reference — no copy of the Monster per step.
    // `return &m;` takes the address of the element inside the vector.
    // The caller gets a pointer into storage owned by the vector; they
    // must not outlive it past a reallocation. For a lookup API that's
    // fine — nobody keeps the pointer across inserts.
    for (const auto& m : bestiary) {
        if (m.name == name) return &m;
    }
    return nullptr;
}

// ---- 2. Iterative binary search -----------------------------------------
//
// O(log n), but only if the bestiary is sorted. Half-open interval
// [low, high): `low` is inclusive, `high` is EXCLUSIVE (one past the end).
// Why half-open?
//   - The initial range is always [0, size), no "size - 1" temptation.
//   - `low == high` is the natural empty-range signal, whatever the types.
//   - When the target is smaller than every element, `high` steps down
//     to `mid` (never `mid - 1`), so we never underflow `std::size_t`.
// The closed-range [low, high] version works too, but the underflow trap
// bites if you forget to guard `high = mid - 1` when mid == 0.

const Monster* binarySearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {
    std::size_t low  = 0;
    std::size_t high = bestiary.size();
    while (low < high) {
        // `low + (high - low) / 2` instead of `(low + high) / 2`. For
        // std::size_t-on-64-bit this overflow is nearly impossible, but
        // the habit travels to int-sized code where it matters. Free.
        std::size_t mid = low + (high - low) / 2;
        const std::string& here = bestiary[mid].name;
        if (here == name) return &bestiary[mid];
        if (here <  name) low  = mid + 1;   // target is to the right of mid
        else              high = mid;       // target is to the left (exclusive)
    }
    return nullptr;
}

// ---- 3. Recursive binary search -----------------------------------------
//
// Same algorithm, same O(log n). The public function just seeds the
// helper with the initial range. The helper is in an anonymous namespace
// (equivalent to `static` for file scope) so it has internal linkage —
// no symbol conflicts if another .cpp happens to define a function of
// the same name.

namespace {
const Monster* binSearchRec(const std::vector<Monster>& bestiary,
                            const std::string&         name,
                            std::size_t                low,
                            std::size_t                high) {
    // Base case: empty range. Nothing to find.
    if (low >= high) return nullptr;
    std::size_t mid = low + (high - low) / 2;
    const std::string& here = bestiary[mid].name;
    if (here == name) return &bestiary[mid];
    // Each recursive call STRICTLY SHRINKS the range, so we always
    // terminate. (If you ever wrote `mid` instead of `mid + 1` on the
    // right-half case, you'd infinite-loop on a single-element range.)
    if (here <  name) return binSearchRec(bestiary, name, mid + 1, high);
    return                   binSearchRec(bestiary, name, low,     mid);
}
}  // namespace

const Monster* binarySearchRecursive(const std::vector<Monster>& bestiary,
                                     const std::string&         name) {
    return binSearchRec(bestiary, name, 0, bestiary.size());
}

// ---- 4. The seam --------------------------------------------------------
//
// Iterative binary is the production pick: same Big-O as recursive, no
// per-step call overhead, and no risk of stack overflow on huge inputs
// (log2 of 10^9 is 30 — safe — but *why* accept the risk when you don't
// have to?). This is the single line that wires the search command to a
// concrete algorithm; change it and every caller upstream follows.

const Monster* findMonster(const std::vector<Monster>& bestiary,
                           const std::string&         name) {
    return binarySearch(bestiary, name);
}

}  // namespace dungeon
