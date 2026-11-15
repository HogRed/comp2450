// COMP 2450 — Floor 1 starter
// bestiary/Search.cpp — YOU implement this file.
//
// Four functions to write. Read Search.h for their contracts.
//
// The big idea this week: the same question ("is X in the list?") has
// three implementations with three VERY different Big-O costs. You will
// write all three, race them in `benchmark`, and then argue — in a commit
// message — which one the rest of the game should use. The code is easy.
// The *thinking* is the point.
//
// Tips for the Unsorted Lich:
//   * binarySearch (and binarySearchRecursive) only work if the bestiary
//     is sorted by name.
//   * main.cpp already calls sortBestiary() right after loading,
//     so you can assume the precondition holds when these run.
//   * If you ever doubt, scan the vector and assert it is sorted.
//
// Submit when:  `search Goblin` returns the goblin's stats and `search Ghost`
//               reports "no such creature." Then run `benchmark` and capture
//               the output for your lab-notes.md.

#include "Search.h"

namespace dungeon {

const Monster* linearSearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {
    // TODO Floor 1 (Mon): walk every entry; return its address when name matches.
    //                     If you reach the end without a match, return nullptr.
    //
    // Think before you type:
    //   - You return `const Monster*` (a pointer into the vector), NOT
    //     `Monster` (a copy). Why a pointer? What would you even return
    //     from a "copy" version when the name is not found?
    //   - In the range-for loop, `for (auto m : bestiary)` makes a COPY
    //     of each monster each iteration. `for (const auto& m : bestiary)`
    //     does not. Which do you want — and why does the difference matter
    //     more for a `Monster` than for an `int`?
    //   - How do you take the address of the element you're looking at?
    //     (Two common idioms. Pick whichever makes your loop read cleanly.)
    // const& avoids copying every Monster; &m is the address of the element
    // inside the vector, so the caller gets a pointer, not a copy.
    for (const auto& m : bestiary) {
        if (m.name == name) return &m;
    }
    return nullptr;  // reached the end: not found
}

const Monster* binarySearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {
    // TODO Floor 1 (Wed): iterative binary search.
    //   PRECONDITION: bestiary is sorted ascending by name.
    //
    // Think before you type:
    //   - Decide your invariant FIRST, then write code: does `high` mean
    //     "the last valid index" (closed range, [low, high]) or "one past
    //     the last valid index" (half-open, [low, high))? Pick one. Every
    //     off-by-one bug starts with mixing the two.
    //   - `std::size_t` is UNSIGNED. If your search range shrinks to empty
    //     and you compute `high - 1`, does that value wrap around to a
    //     huge number? Try in your head: what happens on `search Aardvark`
    //     when Aardvark comes before every monster? Does your loop end?
    //   - A name comparison has THREE outcomes: equal, less, greater. Each
    //     goes in a different direction. If you collapse two branches into
    //     one (e.g., an `if/else` instead of three cases), you've probably
    //     broken binary search. Write all three explicitly.
    //   - Middle index: `(low + high) / 2` is textbook but can overflow for
    //     huge N. `low + (high - low) / 2` is the safe version. Write the
    //     safe one — it's free, and it's a habit worth building.
    // Invariant: half-open range [low, high). The target, if present, is in it.
    // With half-open ranges, `high` is never decremented below `low`, so the
    // unsigned size_t never wraps around on an empty range.
    std::size_t low  = 0;
    std::size_t high = bestiary.size();
    while (low < high) {
        std::size_t mid = low + (high - low) / 2;   // overflow-safe midpoint
        const std::string& probe = bestiary[mid].name;
        if (probe == name)      return &bestiary[mid];   // equal: found
        else if (probe < name)  low  = mid + 1;          // less: look right
        else                    high = mid;              // greater: look left
    }
    return nullptr;  // range empty: not found
}

// Private helper (static = internal linkage) carrying the search range [low, high).
static const Monster* binarySearchRecursiveHelper(const std::vector<Monster>& bestiary,
                                                  const std::string& name,
                                                  std::size_t low, std::size_t high) {
    if (low >= high) return nullptr;                 // base case: empty range
    std::size_t mid = low + (high - low) / 2;
    const std::string& probe = bestiary[mid].name;
    if (probe == name) return &bestiary[mid];        // base case: found
    if (probe < name)  return binarySearchRecursiveHelper(bestiary, name, mid + 1, high);
    return binarySearchRecursiveHelper(bestiary, name, low, mid);  // strictly smaller range
}

const Monster* binarySearchRecursive(const std::vector<Monster>& bestiary,
                                     const std::string&         name) {
    // TODO Floor 1 (Fri): same contract as binarySearch, but recursive.
    //   Recommended pattern: write a `static` helper in this file with extra
    //   (low, high) parameters, and have this public function call it with
    //   the initial range. Same precondition: bestiary must be sorted.
    //
    // Think before you type:
    //   - Every recursion needs a BASE CASE and a RECURSIVE CASE. What is
    //     the smallest range where you already know the answer without
    //     looking further? That is your base case.
    //   - Convince yourself, for each recursive call, that the new range
    //     is a STRICT SUBSET of the old one. If it isn't, you will recurse
    //     until the stack blows up. (Try it at N=100,000 if curious.)
    //   - Why `static` for the helper? It has nothing to do with OOP here.
    //     Look up "internal linkage" — it keeps the helper private to this
    //     .cpp, so two files can have `helper(...)` without a link error.
    //   - After it works: run `benchmark`. Does the recursive version cost
    //     more per call than the iterative one? A little? A lot? Why might
    //     that be? Write the answer in lab-notes.md.
    return binarySearchRecursiveHelper(bestiary, name, 0, bestiary.size());
}

const Monster* findMonster(const std::vector<Monster>& bestiary,
                           const std::string&         name) {
    // TODO Floor 1: pick ONE of the three searches above and delegate.
    //
    // Think before you type:
    //   - At the real bestiary's size (15 monsters), does it matter which
    //     you pick? Run benchmark at N=10 and look at the microseconds.
    //   - At N=100,000, does it matter? By how much?
    //   - This is a JUDGMENT, not a fact. Whatever you pick, write WHY in
    //     your commit message. That reasoning is the graded artifact.
    // Judgment: iterative binary search. At 15 monsters any choice is instant,
    // but at 100,000 binary is hundreds of times faster than linear; the
    // iterative form beats the recursive one by avoiding per-call overhead
    // and any stack-depth concern. Precondition (sorted) is upheld by
    // sortBestiary() in main.cpp.
    return binarySearch(bestiary, name);
}

}