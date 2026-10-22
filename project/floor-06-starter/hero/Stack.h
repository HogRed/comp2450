// COMP 2450 — Floor 6 starter
// hero/Stack.h — LIFO stack template. THE WORK IS HERE.
//
// One header, four one-line method bodies you write, one day:
//   Monday — push, pop, size, empty.  (top() ships written; see below.)
//
// This week's lesson, on one slide: a stack is NOT a data structure.
// It is an *ADT* — a contract that says "the last thing pushed is the
// next thing returned." Many data structures can implement that
// contract. We pick one (your Floor-4½ Chain<T>) and we ADAPT it —
// five trivial delegates is the entire implementation.
//
// `std::stack<T>` is exactly this pattern: by default it adapts
// `std::deque<T>`. We're recreating the standard library's own move.
//
//
// =====================================================================
// Why a Chain<T> and not a Bag<T>?
// =====================================================================
// Either would WORK (both have push_back / pop_back). The Chain<T>
// choice is deliberate because Chain<T>::push_front and pop_front are
// both O(1), and our Stack<T>'s top will be the *front* of the chain.
// On a Bag<T> (which wraps std::vector), pop_back is O(1) but
// pop_front would be O(n). Choosing the right underlying container is
// the entire point of an adapter; we want O(1) push and pop on both
// ends, but specifically on the SAME end. front-of-Chain wins.
//
// =====================================================================
// Why the bodies are TODOs even though they are one line each.
// =====================================================================
// Brevity IS the lesson. These trivial bodies look like nothing, but
// the choice you make — which Chain operation maps to which Stack
// operation — determines whether your Stack is actually LIFO. If you
// pick push_back here and pop_front for pop(), you have built a queue
// instead. Reading the test failures is how you find out.

#pragma once

#include <cstddef>

#include "Chain.h"

namespace dungeon {

template <typename T>
class Stack {
public:
    using value_type = T;

    // Default ctor, dtor, copy, and assign are all fine: Chain<T> has
    // a working Rule of Three (Floor 4½). Stack just inherits that
    // behavior transitively.
    Stack()                              = default;
    ~Stack()                             = default;
    Stack(const Stack& other)            = default;
    Stack& operator=(const Stack& other) = default;

    // ----- THE FOUR THINGS YOU WRITE THIS WEEK (top() is provided) ---

    // Push `value` onto the top of the stack. O(1).
    //
    // TODO Floor 6 (Monday). Body is one line.
    //     chain_.push_front(value);
    void push(const T& value) {
        chain_.push_front(value);
        // TODO Monday
    }

    // Remove the top of the stack. O(1). No-op on empty (matches std::stack
    // behavior — calling pop on an empty stack is undefined, but our
    // Chain::pop_front is already a safe no-op on empty).
    //
    // TODO Floor 6 (Monday). Body is one line.
    //     chain_.pop_front();
    void pop() {
        chain_.pop_front();
        // TODO Monday
    }

    // Read the top of the stack. O(1). PROVIDED — you do not write this.
    // A reference-returning method (T& / const T&) can't be honestly
    // stubbed: there is no safe placeholder T& to hand back before push()
    // works, so we ship the real one-line body. It returns
    // chain_.head()->data — which means top() on an EMPTY stack
    // dereferences nullptr (the Hollow Echo on the Floor 6 page). The
    // `selftest stack` harness checks empty() before every top(), so the
    // build stays runnable while push()/size() are still stubbed. Read
    // this body: it's the same delegate-to-chain_ move your four methods
    // make.
    T& top() {
        return chain_.head()->data;
    }
    const T& top() const {
        // same body as the non-const version above
        return chain_.head()->data;
    }

    // Number of items currently on the stack.
    //
    // TODO Floor 6 (Monday). Body is one line.
    //     return chain_.size();
    std::size_t size() const {
        return chain_.size();   // TODO Monday
    }

    // True iff size() == 0.
    //
    // TODO Floor 6 (Monday). Body is one line — and you don't even need
    // to touch chain_; you can delegate to your own size().
    //     return size() == 0;
    bool empty() const {
        return size() == 0;   // TODO Monday
    }

private:
    Chain<T> chain_;
};

}  // namespace dungeon
