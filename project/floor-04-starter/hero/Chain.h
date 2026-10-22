// COMP 2450 — Floor 4 starter
// hero/Chain.h — singly-linked container template. THE WORK IS HERE.
//
// One header, three pieces of work, three days:
//   Monday    — push_front, size, empty, head().
//   Wednesday — ~Chain() (the destructor) and clear().
//   Friday    — convert the copy ctor and copy assignment to `= delete`.
//
// Why this header exists at all: last week's Bag<T> was a thin wrapper
// over std::vector<T>. The vector did the work; Bag was a stamp around
// it. This week's container is yours from the metal up — your nodes,
// your `new`, your `delete`, your destructor. You also do not get
// std::vector's free random access: walking a chain is O(n), and
// indexed access is O(n) per access. That is the trade.
//
// We use Chain<T> for the hero's event log — a running history of every
// notable thing the hero has done or suffered, prepended one link at a
// time, displayed newest-first. push_front is O(1) on a chain. On a
// vector it is O(n) because every existing element must shift down by
// one. The benchmark on Friday will let you feel that difference at
// N = 100,000.
//
// Why not iterators (begin()/end()): those are Floor 5's lesson. This
// week, callers walk the chain by hand:
//
//     for (const Node* p = chain.head(); p != nullptr; p = p->next) {
//         use(p->data);
//     }
//
// Awkward on purpose. Floor 5 will fix it.

#pragma once

#include <cstddef>

namespace dungeon {

// =====================================================================
// Diagnostic counters — every Node ctor/dtor bumps a global tally so the
// `selftest chain` harness can verify, even without a leak detector,
// that every allocation got a matching deallocation. You do not edit
// these. They are not thread-safe; we only poke them from main.
// =====================================================================
namespace detail {
struct NodeStats {
    static inline std::size_t allocations   = 0;
    static inline std::size_t deallocations = 0;

    static void reset() { allocations = 0; deallocations = 0; }
};
}  // namespace detail


template <typename T>
class Chain {
public:
    // -----------------------------------------------------------------
    // Node — one link of the chain. Holds the payload T and a pointer
    // to the next node. The pointer must be a POINTER, not a Node by
    // value — otherwise the struct would be infinitely large (a Node
    // containing a Node containing a Node …). A pointer is one word.
    //
    // The ctor/dtor bump the diagnostic counters above. That is the
    // only reason Node is not a plain aggregate.
    // -----------------------------------------------------------------
    struct Node {
        T     data;
        Node* next;

        explicit Node(const T& v, Node* n = nullptr) : data(v), next(n) {
            ++detail::NodeStats::allocations;
        }
        ~Node() { ++detail::NodeStats::deallocations; }
    };

    // -----------------------------------------------------------------
    // Construction / destruction
    // -----------------------------------------------------------------
    Chain() = default;

    // TODO Floor 4 (Wednesday) — write the destructor.
    //
    // Walk the chain from head to tail. For each node: SAVE the next
    // pointer FIRST, then delete the current node, then advance.
    //
    //     Node* p = head_;
    //     while (p != nullptr) {
    //         Node* n = p->next;   // <- save BEFORE delete; you cannot
    //                              //    read p->next after delete p.
    //         delete p;            // runs ~Node(), bumps the counter.
    //         p = n;
    //     }
    //     head_ = nullptr;
    //     size_ = 0;
    //
    // (clear() does the same job — implement it below and call it from
    // the destructor body if you prefer one source of truth.)
    ~Chain() {
        // TODO Floor 4 (Wednesday)
        clear();
    }

    // -----------------------------------------------------------------
    // The Rule of Three.
    //
    // Once a class owns raw memory (a `new`'d resource it must `delete`),
    // the compiler-generated copy constructor and copy assignment are
    // WRONG: they would copy the head pointer, leaving two Chain objects
    // sharing the same nodes — and both would try to delete those nodes
    // at end-of-scope. That is a double-free, undefined behaviour, and
    // an immediate crash on most platforms.
    //
    // You have two options:
    //   (a) implement copy properly — walk the source chain and clone
    //       every node into a fresh chain (deep copy);
    //   (b) `= delete` the copy operations so the compiler refuses any
    //       attempt to copy.
    //
    // Floor 4's answer is (b). Floor 4½'s answer is (a).
    //
    // TODO Floor 4 (Friday) — change `= default` to `= delete` on both.
    // -----------------------------------------------------------------
    Chain(const Chain&)            = delete;   // TODO Friday: = delete
    Chain& operator=(const Chain&) = delete;   // TODO Friday: = delete

    // -----------------------------------------------------------------
    // Inspection
    // -----------------------------------------------------------------

    // TODO Floor 4 (Monday) — return the cached size_.
    // We cache size so size() is O(1). Walking the chain to count would
    // be O(n) on every call; the log is queried by `log <n>` constantly.
    std::size_t size() const { return size_; }
    bool        empty() const { return size() == 0; }

    // Raw head pointer. Callers walk the chain by hand:
    //     for (const Node* p = chain.head(); p; p = p->next) ...
    // The non-const overload exists for completeness; we don't use it
    // this week.
    //
    // TODO Floor 4 (Monday) — return head_.
    const Node* head() const { return head_; }
    Node* head() { return head_; }

    // -----------------------------------------------------------------
    // Mutation
    // -----------------------------------------------------------------

    // Prepend a new node holding `value`. O(1) — that is the whole point
    // of having a linked list at all.
    //
    // TODO Floor 4 (Monday). The body is three lines:
    //     Node* n = new Node(value, head_);
    //     head_   = n;
    //     ++size_;
    void push_front(const T& value) {
        // TODO Monday
        // splicing
        Node* n = new Node(value, head_);
        // the chain head pointer points at our new node
        head_ = n;
        // bump chain size
        ++size_;
    }

    // Walk and delete every node. Leaves the chain empty.
    //
    // TODO Floor 4 (Wednesday). Same loop as the destructor.
    void clear() {
        Node* p = head_; //start at the head
        while (p != nullptr) {
            // save the NEXT pointer into a loccal pointer first
            Node* n = p->next;//save befor delete
            delete p;//freeing the current node
            p = n; //advance to the next saved
        }
        head_ = nullptr;//chain is gone... :(
        size_ = 0;// so size is 0
    }

private:
    Node*       head_ = nullptr;
    std::size_t size_ = 0;
};

}  // namespace dungeon
