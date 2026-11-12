// COMP 2450 — Floor 10 starter
// bestiary/Registry.h — the Hall of Names. THE WORK IS HERE (the six bodies
// in the "===== YOU WRITE =====" block near the bottom).
//
// For five floors you built pointer structures by hand: Chain, Stack, Queue,
// Tree, BST, and the AVL balancing you deliberately did NOT finish. The lesson
// of Floor 9½ was: don't hand-roll the balanced tree — reach for std::set /
// std::map, which ARE balanced trees. This floor is where you finally reach.
//
// You do NOT implement an associative container. You write CLIENT code against
// std::set, std::map, and std::pair. The payoff, said out loud: the library
// already gives you the balanced tree you just studied, and a key->value index
// on top of it, for free.
//
// =====================================================================
// What this class is
// =====================================================================
// Registry is a thin wrapper that re-indexes McCown's existing bestiary (the
// same Bag<Monster> loaded from data/monsters.txt) into three standard
// associative containers, added IN PARALLEL to the working Bag and BST —
// nothing earlier is touched:
//
//   byName_         std::map<std::string, Monster>  — the bestiary indexed by
//                   name. One declaration + one find() does what the whole
//                   hand-written BST<Monster> + recall/forget did on Floor 9.
//   weaknesses_     std::set<std::string>           — the DISTINCT weaknesses
//                   present (the raw data has duplicates: fire 6x, holy 3x...).
//                   A set collapses duplicates, stays sorted, answers
//                   membership in one call.
//   weaknessCounts_ std::map<std::string, int>      — how many monsters share
//                   each weakness. The home of the operator[] discussion.
//
// =====================================================================
// What is provided vs. what you write
// =====================================================================
// PROVIDED (complete — do NOT edit): the three members, the constructor (which
// walks the bestiary and calls index() once per monster), size()/empty(), and
// the three const accessors byName()/weaknesses()/weaknessCounts() the REPL and
// self-test use to print ordered output.
//
// YOU WRITE (six small bodies, near the bottom):
//   Monday    — index, knows                    (build once, ask in one call)
//   Wednesday — recall, hasWeakness, countWeakness (three containers, three
//                                                   one-liners)
//   Friday    — forget                          (mutate, staying consistent)
//
// =====================================================================
// One trap to notice now: operator[] inserts on read
// =====================================================================
// std::map::operator[] on a MISSING key default-constructs and INSERTS it. So
// reading a count with weaknessCounts_["thunder"] does not just return 0 — it
// CREATES a "thunder" entry with count 0, a weakness no beast has. That is the
// Phantom Key. countWeakness (method #5) is your defense: it is const, so [] will
// not even compile there — use find() to ask, reserve [] for keys you mean to set.

#pragma once

#include <cstddef>
#include <map>
#include <set>
#include <string>

#include "../hero/Bag.h"
#include "Monster.h"

namespace dungeon {

class Registry {
public:
    // ----- construction (PROVIDED, complete) -------------------------
    // Walk the bestiary in FILE order (deliberately NOT alphabetical) and
    // index() each monster into all three containers. The std::set / std::map
    // sort on insert, so weaknesses() and byName() come out alphabetical even
    // though we inserted out of order — the exact "free sort" you got from an
    // in-order BST walk on Floor 9, now with no tree code of your own.
    //
    // Until index() is written (Monday), this loop runs but adds nothing, so
    // size() is 0 and `selftest registry` FAILs with a diagnostic.
    explicit Registry(const Bag<Monster>& bestiary) {
        for (const Monster& m : bestiary) index(m);
    }

    // ----- trivial accessors (PROVIDED, complete) --------------------
    std::size_t size()  const { return byName_.size(); }   // distinct names
    bool        empty() const { return byName_.empty(); }

    // Read-only views the REPL and self-test iterate. byName() lets the `names`
    // command walk the registry in name order with structured bindings;
    // weaknesses() prints the sorted distinct set; weaknessCounts() backs the
    // count display.
    const std::map<std::string, Monster>& byName()         const { return byName_; }
    const std::set<std::string>&          weaknesses()     const { return weaknesses_; }
    const std::map<std::string, int>&     weaknessCounts() const { return weaknessCounts_; }

    // ===== YOU WRITE — the six method bodies =========================
    // Each ships as a stub that COMPILES but returns an empty/false answer, so
    // `selftest registry` FAILs with a diagnostic rather than failing to build
    // (same pattern as the Floor 9 BST stubs). Fill them in MWF.

    // ----- Monday: build once, ask in one call -----------------------

    // Insert m into ALL THREE containers. This single helper is what the
    // constructor calls once per monster, so getting it right populates
    // everything. Three lines, one per container:
    //   * byName_[m.name] = m;              (index the record by name)
    //   * weaknesses_.insert(m.weakness);   (a set dedups automatically)
    //   * weaknessCounts_[m.weakness] += 1; (bump the count for this weakness;
    //                                        here [] is correct — we MEAN to
    //                                        create/update the entry)
    //
    // TODO Floor 10 (Monday). Stub is empty -> phases 1–6 all FAIL (everything
    // is empty). Monday's first edit lights phase 1.
    void index(const Monster& m) {
        // TODO Monday — populate byName_, weaknesses_, and weaknessCounts_.
        byName_[m.name] = m; // file the whole record under its name
        weaknesses_.insert(m.weakness); // add the weakness to the set
        weaknessCounts_[m.weakness] += 1; // bump my tally
                                          // new ket begins at 0
    }

    // Return whether a monster with that name is in the registry — using the
    // MAP, in one call, NOT a loop. The "membership in one call" beat.
    //
    // TODO Floor 10 (Monday). Sketch:
    //     return byName_.count(name) > 0;     // (or: find(name) != end())
    bool knows(const std::string& name) const {
        // TODO Monday — stub never knows anyone
        // count is either 0 or 1 on a map
        // map --> keys are unique
        return byName_.count(name) > 0;
    }

    // ----- Wednesday: three containers, three one-liners -------------

    // Return a pointer to the named monster, or nullptr if absent — using
    // map::find and checking against end(). The direct one-liner replacement
    // for Floor 9's whole recursive findFrom descent. Return &it->second.
    //
    // TODO Floor 10 (Wednesday). Sketch:
    //     auto it = byName_.find(name);
    //     return it == byName_.end() ? nullptr : &it->second;
    const Monster* recall(const std::string& name) const {
        // TODO Wednesday — stub finds no one
        // map will do our descent
        auto it = byName_.find(name);
        return it == byName_.end() ? nullptr : &it->second;
    }

    // Return whether ANY monster has weakness w — using set::count on
    // weaknesses_. Membership against the SET, contrasted with knows()'s
    // membership against the MAP.
    //
    // TODO Floor 10 (Wednesday). Sketch:
    //     return weaknesses_.count(w) > 0;
    bool hasWeakness(const std::string& w) const {
        // TODO Wednesday — stub claims nothing is weak to anything
        // same shape as knows()
        // this is a set::
        return weaknesses_.count(w) > 0;
    }

    // Return how many monsters share weakness w, or 0 if none — by reading
    // weaknessCounts_ with FIND, never operator[]. This method is const, so []
    // won't even compile here: the design makes the Phantom Key bug impossible.
    //
    // TODO Floor 10 (Wednesday). Sketch:
    //     auto it = weaknessCounts_.find(w);
    //     return it == weaknessCounts_.end() ? 0 : it->second;
    int countWeakness(const std::string& w) const {
        // TODO Wednesday — stub counts nothing
        // do not use [] --> we don't add an entry by accident
        // find is only going to LOOK (no key will ever be
        auto it = weaknessCounts_.find(w);
        // missing = 0, and the map is left untouched
        // weaknessCounts_[w] would not compile here 
        // const
        return it == weaknessCounts_.end() ? 0 : it->second;
    }

    // ----- Friday: mutate, staying consistent ------------------------

    // Remove the named monster and keep the derived structures consistent:
    //   * erase from byName_;
    //   * decrement weaknessCounts_[that monster's weakness];
    //   * if that count hits 0, erase the weakness from BOTH weaknessCounts_
    //     AND weaknesses_ (so the distinct-weakness shelf never keeps a
    //     weakness no monster has — the inverse of the Phantom Key).
    // Return whether anything was removed. The ONLY multi-container-consistency
    // method; it mirrors Floor 9's forget, but now it is library calls, not
    // pointer surgery.
    //
    // TODO Floor 10 (Friday). Sketch:
    //     auto it = byName_.find(name);
    //     if (it == byName_.end()) return false;
    //     const std::string w = it->second.weakness;
    //     byName_.erase(it);
    //     if (--weaknessCounts_[w] == 0) {
    //         weaknessCounts_.erase(w);
    //         weaknesses_.erase(w);
    //     }
    //     return true;
    bool forget(const std::string& name) {
        // TODO Friday — stub forgets nothing
        // find the plate first
        auto it = byName_.find(name);
        if (it == byName_.end()) return false;
        const std::string w = it->second.weakness;
        byName_.erase(it); // erase the record
        if (--weaknessCounts_[w] == 0) {
            weaknessCounts_.erase(w);
            weaknesses_.erase(w);
        }
        return true;
    }

private:
    std::map<std::string, Monster> byName_;          // name -> the record
    std::set<std::string>          weaknesses_;       // distinct weaknesses
    std::map<std::string, int>     weaknessCounts_;   // weakness -> how many
};

}  // namespace dungeon
