// COMP 2450 — Floor 3 starter
// hero/Bag.h — YOU implement the method bodies this week.
//
// A class template. One class declaration; the compiler stamps out a
// fresh copy for every type T you instantiate. By Friday this same
// class will back two very different things — hero.inventory is a
// Bag<Item>, the bestiary is a Bag<Monster>. One mold, two shapes.
//
// What this class owns: nothing fancy. The actual storage is a private
// std::vector<T>. Bag<T> is a WRAPPER — you are NOT reinventing vector
// this week. The point is the template MACHINERY, not the data layout.
// You will reinvent storage on Floor 4 (linked lists), when linked-list
// nodes force the `new`/`delete`/destructor conversation.
//
// Why the bodies live in the header:
//   Template bodies must be visible to every caller, so the compiler
//   can stamp the right version for each T. A .cpp file would only
//   compile the template for the types it happened to name inside
//   itself — useless for a container. Class templates live in headers.
//   That's a language rule, not a style choice.
//
// The stubs below `throw std::logic_error` with a diagnostic message.
// Until you write the real body for a given method, the first call
// to it at runtime crashes with a message that names the method — so
// you always know which TODO is biting you. Replace the `throw`
// with the real body and the diagnostic goes away.

#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

#include "BagException.h"

namespace dungeon {

template <typename T>
class Bag {
public:
    // ---- inspection ----------------------------------------------------
    //
    // Four questions every container has to answer. All can be one-line
    // delegates to the underlying std::vector<T>. Resist the urge to
    // write loops — vector already knows.

    std::size_t size() const {
        // TODO Floor 3 (Wed): return the number of elements in the bag.
        // Hint: std::vector<T> already tracks this.
        throw std::logic_error("TODO: Bag::size() not yet implemented (Floor 3 Wed)");
    }

    bool empty() const {
        // TODO Floor 3 (Wed): return true when the bag holds no elements.
        throw std::logic_error("TODO: Bag::empty() not yet implemented (Floor 3 Wed)");
    }

    // ---- access — unchecked --------------------------------------------
    //
    // operator[] does NOT bounds-check. It is the fast path. Callers
    // who KNOW their index is in range use this — for instance, the
    // body of a `for (size_t i = 0; i < bag.size(); ++i)` loop can't
    // possibly be out of range. When the index came from USER INPUT,
    // prefer at() (below).

    const T& operator[](std::size_t i) const {
        // TODO Floor 3 (Wed): return element at index i, unchecked.
        // One-line delegate to the underlying std::vector<T>.
        (void)i;
        throw std::logic_error("TODO: Bag::operator[] not yet implemented (Floor 3 Wed)");
    }

    T& operator[](std::size_t i) {
        // TODO Floor 3 (Wed): same as the const version, non-const.
        (void)i;
        throw std::logic_error("TODO: Bag::operator[] not yet implemented (Floor 3 Wed)");
    }

    // ---- access — checked ----------------------------------------------
    //
    // at() bounds-checks and THROWS a BagException when the caller asks
    // for something impossible. Use this when the index came from user
    // input — when the main loop handles `inspect 99` but the bag only
    // holds five items, at() throws, the try/catch in main catches, the
    // game keeps going.

    const T& at(std::size_t i) const {
        // TODO Floor 3 (Fri): bounds-check. If i >= size(), THROW a
        // BagException constructed with (i, size()). Otherwise return
        // the element at i — the unchecked [] is fine HERE because you
        // already checked.
        //
        // The one word `throw` IS the exceptions lesson.
        (void)i;
        throw std::logic_error("TODO: Bag::at() not yet implemented (Floor 3 Fri)");
    }

    T& at(std::size_t i) {
        // TODO Floor 3 (Fri): non-const version. Same body.
        (void)i;
        throw std::logic_error("TODO: Bag::at() not yet implemented (Floor 3 Fri)");
    }

    // ---- mutation ------------------------------------------------------

    void push_back(const T& value) {
        // TODO Floor 3 (Wed): append `value` to the end of the bag.
        // One-liner delegating to the underlying std::vector<T>.
        (void)value;
        throw std::logic_error("TODO: Bag::push_back() not yet implemented (Floor 3 Wed)");
    }

    void clear() {
        // TODO Floor 3 (Wed): remove all elements; size becomes 0.
        throw std::logic_error("TODO: Bag::clear() not yet implemented (Floor 3 Wed)");
    }

    // ---- iteration -----------------------------------------------------
    //
    // begin()/end() are what lets `for (auto& x : bag)` work AND lets
    // std::sort / std::find / every STL algorithm work on a Bag. Delegate
    // to the underlying vector's iterators.
    //
    // Why both const and non-const overloads? A function that takes
    // `const Bag<T>&` (like findByName<T> does) only sees the const
    // pair. A function that wants to MUTATE the bag sees the non-const
    // pair. The compiler picks based on what kind of reference you have.

    using iterator       = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    iterator begin() {
        // TODO Floor 3 (Wed): return data_.begin().
        throw std::logic_error("TODO: Bag::begin() not yet implemented (Floor 3 Wed)");
    }
    iterator end() {
        // TODO Floor 3 (Wed): return data_.end().
        throw std::logic_error("TODO: Bag::end() not yet implemented (Floor 3 Wed)");
    }
    const_iterator begin() const {
        // TODO Floor 3 (Wed): return data_.begin().
        throw std::logic_error("TODO: Bag::begin() const not yet implemented (Floor 3 Wed)");
    }
    const_iterator end() const {
        // TODO Floor 3 (Wed): return data_.end().
        throw std::logic_error("TODO: Bag::end() const not yet implemented (Floor 3 Wed)");
    }

private:
    std::vector<T> data_;
};

}  // namespace dungeon
