// COMP 2450 — Floor 11 starter
// hero/HashTable.h — the Hall of Whispers. THE WORK IS HERE (the six bodies
// in the "===== YOU WRITE =====" block near the bottom).
//
// For five floors you built pointer structures by hand: Chain, Stack, Queue,
// Tree, BST. Floor 9½ said "don't hand-roll the balanced tree — reach for
// std::set / std::map." Floor 10 reached. This floor is the LAST thing you
// build by hand — and it is the satisfying capstone of the whole build-your-own
// arc, because the thing you build is made out of a thing you already built.
//
// The Hall of Names answered in O(log n) by keeping everything in ORDER — a
// balanced tree the library walked for you. The Hall of Whispers answers in
// O(1) AVERAGE by giving up order entirely: it COMPUTES a bucket from the name
// (a hash) and goes straight there. A whisper, not a walk.
//
// =====================================================================
// What this class is — an array of the linked lists you wrote on Floor 4
// =====================================================================
// A separate-chaining hash table. The bucket array is a std::vector, and each
// slot is one of YOUR OWN Chain<Monster> linked lists (hero/Chain.h, Floor 4)
// holding every monster that hashes to that index. That is the composition
// payoff, said out loud: a hash table IS an array of the chain you already
// built. When two different names land in the same bucket — a COLLISION, which
// is not a bug but inevitable (more names than buckets) — the bucket's Chain
// simply holds both, and you scan a short list of two instead of all 15.
//
// It is keyed on the monster's NAME (a std::string), and it STORES the whole
// Monster as the value, so find() hands back a const Monster* exactly like
// Floor 10's Registry::recall. It is a concrete class (not a template) on
// purpose: the headline beat is "I wrote a hash function for a STRING key,"
// and a generic HashTable<K,V> would bury that behind a Hasher parameter.
//
// It is added IN PARALLEL to the working Bag<Monster>, BST<Monster> (Floor 9),
// and Registry (Floor 10) — nothing earlier is touched or removed. `banish`-ing
// a monster from this table does NOT touch the Bag, the BST, or the Registry;
// four independent indexes can legitimately diverge after a mutation (the
// Floor 10 page already taught the three-way version — this makes it four).
//
// =====================================================================
// What is provided vs. what you write
// =====================================================================
// PROVIDED (complete — do NOT edit): the two members, both constructors
// (including the one that walks the bestiary and insert()s each monster),
// size()/empty()/bucketCount()/loadFactor(), the read-only buckets() view the
// REPL/self-test print, and rehash() — the resize DRIVER. The driver allocates
// a fresh bucket vector and re-files every existing item into it; the ONE line
// inside it that recomputes an item's new home is yours (it is the same
// bucketIndex call — see #6).
//
// YOU WRITE (six small bodies, near the bottom):
//   Monday    — hashKey, bucketIndex, and the BASIC body of insert
//               (compute where the name lives, and put it there)
//   Wednesday — finish insert (no duplicates), find, erase
//               (one short list, not the whole table)
//   Friday    — rehashIfNeeded (+ the one re-file line in rehash above)
//               (keep the whisper a whisper as the table grows)
//
// =====================================================================
// Two traps to notice now
// =====================================================================
// (1) bucketIndex must reduce the hash code with `% bucketCount`, NEVER an
//     unchecked index. The provided constructor guarantees bucketCount() >= 1,
//     so a correct `% bucketCount` can never divide by zero.
// (2) find must compare the NAME, not just the bucket. Two names can share a
//     bucket (a collision); returning the first item in the bucket is the bug.

#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Chain.h"
#include "../bestiary/Monster.h"

namespace dungeon {

class HashTable {
public:
    // The starting bucket count. Small ON PURPOSE: with 15 monsters this fills
    // past the load-factor threshold and the table GROWS itself during load —
    // the whole point of the floor made visible (see rehashIfNeeded).
    static constexpr std::size_t kInitialBuckets = 8;

    // The load-factor trigger (ZyBook §12.4). When items / buckets exceeds this,
    // rehashIfNeeded() doubles the bucket count so chains stay short.
    static constexpr double kMaxLoadFactor = 0.75;

    // ----- construction (PROVIDED, complete) -------------------------
    // Size the bucket array and zero the count. We std::max with 1 so the table
    // ALWAYS has at least one bucket — that is what lets a correct bucketIndex
    // do `% bucketCount()` without ever dividing by zero, even if someone asks
    // for 0 buckets.
    explicit HashTable(std::size_t initialBuckets = kInitialBuckets)
        : buckets_(initialBuckets < 1 ? 1 : initialBuckets), count_(0) {}

    // Build the table from McCown's bestiary: walk it in FILE order and insert()
    // each monster. This is the exact Registry pattern. Until insert() is
    // written (Monday) this loop runs but stores nothing, so size()==0 and
    // `selftest whispers` FAILs with a diagnostic.
    explicit HashTable(const Bag<Monster>& bestiary)
        : buckets_(kInitialBuckets), count_(0) {
        for (const Monster& m : bestiary) insert(m);
    }

    // ----- trivial accessors (PROVIDED, complete) --------------------
    std::size_t size()        const { return count_; }
    bool        empty()       const { return count_ == 0; }
    std::size_t bucketCount() const { return buckets_.size(); }

    // The §12.4 load factor: items divided by buckets. Provided so the page can
    // SHOW it without making the arithmetic the student's job. The TRIGGER
    // threshold (kMaxLoadFactor) is applied inside rehashIfNeeded (#6).
    double loadFactor() const {
        return static_cast<double>(count_) / static_cast<double>(buckets_.size());
    }

    // Read-only view of the bucket array, so the `buckets` REPL command and the
    // self-test can print WHICH names share a bucket (the collision picture) and
    // iterate the whole table.
    const std::vector<Chain<Monster>>& buckets() const { return buckets_; }

    // ----- the resize DRIVER (PROVIDED skeleton; ONE student line, see #6) ---
    // Allocate a fresh bucket vector of newBucketCount, then RE-FILE every
    // existing item into it. The driver loop (walk the old buckets, walk each
    // chain) is provided; the single line that computes each item's NEW home and
    // pushes it there is yours. Hash the name, then mod by newBucketCount. Do
    // not call bucketIndex(): it still uses the OLD count, and an item's home
    // bucket CHANGES when the bucket count changes. When the new
    // table is full, swap it in. We build a fresh vector and swap rather than
    // mutating buckets_ while iterating it (mutating a container you are walking
    // is the aliasing bug §8 warns about).
    void rehash(std::size_t newBucketCount) {
        if (newBucketCount < 1) newBucketCount = 1;
        std::vector<Chain<Monster>> grown(newBucketCount);
        for (const Chain<Monster>& bucket : buckets_) {
            for (const Monster& m : bucket) {
                // ===== YOU WRITE (Friday, with rehashIfNeeded #6) =====
                // Compute m's home in the NEW, bigger array and push it there.
                // It is one line. Sketch:
                //     grown[hashKey(m.name) % newBucketCount].push_back(m);
                // (count_ does NOT change here — rehash moves items, it does not
                //  add or drop any. The TODO stub below files nothing, so after a
                //  rehash the table would be EMPTY and self-test phase 6 FAILs.)
                (void)m;   // TODO Friday — re-file m into grown[...]
            }
        }
        buckets_.swap(grown);
    }

    // ===== YOU WRITE — the six method bodies =========================
    // Each ships as a stub that COMPILES but returns an empty/false/no-op
    // answer, so `selftest whispers` FAILs with a diagnostic rather than failing
    // to build (same pattern as the Floor 9 BST / Floor 10 Registry stubs). The
    // stubs never crash: bucketIndex returns a VALID index (0) so `% 0` can
    // never happen. Fill them in MWF.

    // ----- Monday: compute where the name lives ----------------------

    // Compute a hash CODE for a name — a MULTIPLICATIVE STRING HASH (§12.5).
    // Start an accumulator at 0 and, for each character, fold it in:
    //     h = h * 31 + static_cast<unsigned char>(c);
    // The multiply-and-add mixes EVERY character, so similar names ("Wisp" vs
    // "Wraith") land far apart — a good hash SCATTERS. (Contrast Grix's hash,
    // which returns the name's length: every 6-letter beast piles into one
    // bucket, and the "hash table" decays back into one long Chain — O(n).)
    // Returns the raw code; it is NOT yet reduced to a bucket. The static_cast
    // to unsigned char matters: a plain char can be negative.
    //
    // TODO Floor 11 (Monday). Stub returns 0 -> every name hashes to the same
    // code, so phase 1 (deterministic-AND-distinct) FAILs.
    std::size_t hashKey(const std::string& name) const {
        (void)name;
        return 0;   // TODO Monday — multiplicative string hash (h = h*31 + c)
    }

    // Reduce the hash code to a valid bucket index — the MODULO HASH step
    // (§12.5). This is where "compute where the name lives" becomes literal:
    //     return hashKey(name) % buckets_.size();
    // It MUST be `% bucketCount`, never an unchecked index. The constructor
    // guarantees buckets_.size() >= 1, so this can never divide by zero.
    //
    // TODO Floor 11 (Monday). Stub returns 0 (a VALID index, so no crash), but
    // it sends every name to bucket 0, so distinctness/correctness phases FAIL.
    std::size_t bucketIndex(const std::string& name) const {
        (void)name;
        return 0;   // TODO Monday — return hashKey(name) % buckets_.size();
    }

    // Add m to its home bucket. Compute bucketIndex(m.name); if no monster with
    // that name is ALREADY in that bucket's chain, push_back it and ++count_
    // (return true). If the name is already present, UPDATE it in place and
    // return false (no double-count). THEN call rehashIfNeeded() so the table
    // grows if it just got too crowded.
    //
    // Monday writes the BASIC body (compute the bucket, push_back, ++count_) —
    // enough that `buckets` shows names landing in computed slots. Wednesday
    // adds the dup-check + update-in-place. The rehashIfNeeded() call is part of
    // the final body (Friday it actually does something).
    //
    // TODO Floor 11 (Monday→Wednesday). Sketch:
    //     std::size_t i = bucketIndex(m.name);
    //     for (Monster& existing : buckets_[i]) {       // Wed: dup-check
    //         if (existing.name == m.name) { existing = m; return false; }
    //     }
    //     buckets_[i].push_back(m);
    //     ++count_;
    //     rehashIfNeeded();                              // Fri: grow if crowded
    //     return true;
    bool insert(const Monster& m) {
        (void)m;
        return false;   // TODO Monday — push m into bucketIndex(m.name)'s chain
    }

    // ----- Wednesday: one short list, not the whole table ------------

    // Return a pointer to the named monster, or nullptr — compute
    // bucketIndex(name), then scan ONLY that one bucket's Chain for a node whose
    // name matches (a range-for over the chain). This is the O(1)-average
    // lookup: you search ONE short list, not all 15. Contrast Floor 10's recall
    // (one map::find, O(log n)) and Floor 9's whole tree descent.
    //
    // find must compare the NAME, not just the bucket: two names can collide
    // into the same bucket, so returning the first item there is wrong.
    //
    // TODO Floor 11 (Wednesday). Sketch:
    //     std::size_t i = bucketIndex(name);
    //     for (const Monster& m : buckets_[i])
    //         if (m.name == name) return &m;
    //     return nullptr;
    const Monster* find(const std::string& name) const {
        (void)name;
        return nullptr;   // TODO Wednesday — scan only bucketIndex(name)'s chain
    }

    // Remove the named monster: compute its bucket, remove the matching item
    // from THAT bucket's chain, --count_; return whether anything was removed.
    // Your Chain<T> has eraseFirst(pred) (provided reference code) — pass it a
    // predicate that matches the name and it removes the first match and tells
    // you whether it found one. Do NOT clear the whole bucket: erase removes
    // only the one item, leaving any colliding neighbors in place.
    //
    // TODO Floor 11 (Wednesday). Sketch:
    //     std::size_t i = bucketIndex(name);
    //     bool removed = buckets_[i].eraseFirst(
    //         [&](const Monster& m) { return m.name == name; });
    //     if (removed) --count_;
    //     return removed;
    bool erase(const std::string& name) {
        (void)name;
        return false;   // TODO Wednesday — eraseFirst from bucketIndex(name)'s chain
    }

    // ----- Friday: keep the whisper a whisper ------------------------

    // If the load factor has crept above the threshold (ZyBook §12.4), DOUBLE
    // the bucket count and rehash; return the (possibly new) bucket count. This
    // is what keeps O(1) from decaying toward O(n): without it, chains lengthen
    // as you insert and the whisper becomes a walk. The actual re-filing happens
    // in the provided rehash() driver above — and the ONE line inside it (the
    // re-file) is the other half of this beat.
    //
    // TODO Floor 11 (Friday). Sketch:
    //     if (loadFactor() > kMaxLoadFactor)
    //         rehash(buckets_.size() * 2);
    //     return buckets_.size();
    std::size_t rehashIfNeeded() {
        return buckets_.size();   // TODO Friday — grow when loadFactor() > kMaxLoadFactor
    }

private:
    std::vector<Chain<Monster>> buckets_;   // the bucket array: an array of YOUR Chain
    std::size_t                 count_;      // items stored (so size()/loadFactor() are O(1))
};

}  // namespace dungeon
