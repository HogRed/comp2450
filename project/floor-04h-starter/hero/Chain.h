// COMP 2450 — Floor 4½ starter
// hero/Chain.h — doubly-linked container template. THE WORK IS HERE.
//
// You arrive on Floor 4½ with a working singly-linked Chain<T> from
// Floor 4 — push_front, size, head, the destructor, clear, and copy
// operations that were `= delete`. All of that is preserved below.
//
// This week's three-day arc:
//   Monday    — add `prev` to Node and `tail_` to Chain, then implement
//               tail() and push_back. Rewire push_front to maintain the
//               new pointers.
//   Wednesday — implement the deep copy ctor (replace the old
//               `= delete`). Verify with `selftest chain`.
//   Friday    — implement copy assignment (copy-and-swap or explicit),
//               plus pop_front and pop_back. Final `selftest chain`.
//
// The big lesson is the Rule of Three:
//   Floor 4 wrote the destructor and deleted the copies.
//   Floor 4½ writes the copies properly.
// By the end of this week, `Chain<T>` is a full-fledged owning container.
//
// Why bother with `prev`?  Two reasons:
//   1. You can walk the chain backward — tail → head — without rebuilding
//      it. The `log --oldest` command does exactly that.
//   2. `push_back`, `pop_back`, and `pop_front` all become O(1). Without
//      `prev`, even with a `tail_` pointer, pop_back is O(n) — to delete
//      the tail you must find the *new* tail, which is the second-to-last
//      node, and a singly-linked chain has no cheap way to reach it.
//
// The invariants you must maintain on every mutation:
//   * empty chain:  head_ == nullptr  AND  tail_ == nullptr  AND  size_ == 0
//   * non-empty:    head_->prev == nullptr  AND  tail_->next == nullptr
//   * size_ matches the actual node count
// Break any of these and `log --oldest`, `clone hero`, or `selftest chain`
// will tell you about it loudly.

#pragma once

#include <cstddef>
#include <utility>   // std::swap, used by copy-and-swap if you take that route

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
    // Node — one link of the chain. Holds the payload T plus pointers
    // to BOTH neighbors. On Floor 4 this struct had only `next`; the
    // new field below is the doubly-linked upgrade.
    //
    // The ctor's defaults give you a freestanding node with no
    // neighbors — handy when you `new Node(value)` and wire it in by
    // hand afterward.
    // -----------------------------------------------------------------
    struct Node {
        T     data;
        Node* prev;   // NEW on Floor 4½ — points to the previous node, or nullptr at head.
        Node* next;   // unchanged from Floor 4 — nullptr at tail.

        explicit Node(const T& v, Node* p = nullptr, Node* n = nullptr)
            : data(v), prev(p), next(n) {
            ++detail::NodeStats::allocations;
        }
        ~Node() { ++detail::NodeStats::deallocations; }
    };

    // -----------------------------------------------------------------
    // Construction / destruction
    // -----------------------------------------------------------------
    Chain() = default;

    // Destructor (Floor 4 — unchanged). Walks the chain forward and
    // deletes each node, saving `next` before the delete so we never
    // read freed memory. The `prev` pointers are not used here — once
    // you've decided to free a node you don't care where it came from.
    ~Chain() { clear(); }

    // -----------------------------------------------------------------
    // The Rule of Three — closing the loop opened on Floor 4.
    //
    // On Floor 4 you `= delete`d these. This week you implement them
    // properly. Without them, `Hero` cannot be copied — and `clone hero`
    // is exactly the demo that needs the copy.
    //
    // The trap: the *default* copy ctor copies the raw pointer fields
    // (head_, tail_, size_). That is a *shallow* copy — two Chains
    // pointing at the same nodes, and both destructors fighting over
    // them when scope ends. The DEEP copy walks the source chain and
    // allocates fresh nodes into the destination.
    // -----------------------------------------------------------------

    // TODO Floor 4½ (Wednesday) — implement deep copy.
    //
    // Walk `other` from head to tail; for each node, push_back that
    // value into *this. By the time the loop ends, *this owns size_
    // freshly-allocated nodes whose data equals `other`'s, but whose
    // addresses are entirely independent.
    //
    //     Chain(const Chain& other) {
    //         for (const Node* p = other.head_; p != nullptr; p = p->next) {
    //             push_back(p->data);
    //         }
    //     }
    //
    // Leaving this empty means a copy of a chain is silently empty.
    // The `clone hero` command will demonstrate the bug.
    Chain(const Chain& other) {
        // TODO Wednesday — deep copy.
        for (const Node* p = other.head_; p != nullptr; p = p->next) {
            push_back(p->data);
		}
    }

    // TODO Floor 4½ (Friday) — implement copy assignment.
    //
    // Two paths:
    //
    //   (a) Copy-and-swap — the safe and short one:
    //         Chain& operator=(const Chain& other) {
    //             Chain tmp(other);   // uses the copy ctor (Wed's work)
    //             swap(tmp);
    //             return *this;
    //             // tmp's destructor cleans up our old contents
    //         }
    //       Uses the `swap` member below.
    //
    //   (b) Explicit — clear and rebuild:
    //         Chain& operator=(const Chain& other) {
    //             if (this == &other) return *this;   // self-assignment guard
    //             clear();
    //             for (const Node* p = other.head_; p; p = p->next) {
    //                 push_back(p->data);
    //             }
    //             return *this;
    //         }
    //       More code, but no surprise about why it works.
    //
    // Pick one. Defend it in your lab notes.
    Chain& operator=(const Chain& other) {
        // TODO Friday — copy assignment.
        Chain tmp(other);
        swap(tmp);
		return *this;


    }

    // Member swap — useful for copy-and-swap, useful for nothing else.
    // Pre-built so you can use it on Friday without thinking about it.
    void swap(Chain& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

    // -----------------------------------------------------------------
    // Inspection — Floor 4 versions, working, with `tail()` added.
    // -----------------------------------------------------------------
    std::size_t size()  const { return size_; }
    bool        empty() const { return size_ == 0; }

    const Node* head() const { return head_; }
    Node*       head()       { return head_; }

    // TODO Floor 4½ (Monday) — return tail_.
    // Used by `log --oldest`, which walks the chain backward via prev.
    const Node* tail() const { return tail_; /* TODO Monday */ }
    Node*       tail()       { return tail_; /* TODO Monday */ }

    // -----------------------------------------------------------------
    // Mutation — Floor 4's push_front kept, with a Floor 4½ extension.
    //
    // push_front currently does Floor 4's job: it allocates a new node
    // whose `next` points at the old head, and updates head_ and size_.
    // It does NOT yet set the new node's `prev` (which must be nullptr
    // — the new node is the head), it does NOT yet update the OLD
    // head's prev to point back at the new node, and it does NOT update
    // tail_ when the chain was previously empty. All three are bugs
    // waiting to surface the first time you walk the chain backward.
    //
    // TODO Floor 4½ (Monday) — extend the body:
    //
    //     Node* n = new Node(value, /*prev=*/nullptr, /*next=*/head_);
    //     if (head_ != nullptr) head_->prev = n;   // old head now has a prev
    //     else                  tail_ = n;          // chain was empty; n is also the tail
    //     head_ = n;
    //     ++size_;
    // -----------------------------------------------------------------
    void push_front(const T& value) {
        // TODO Monday — wire prev/tail consistency (see comment above).
		Node* n = new Node(value, nullptr, head_);
        if (head_ != nullptr) head_->prev = n;   // old head now has a prev
		else                  tail_ = n;          // chain was empty; n is also the tail
		head_ = n;
		++size_;        
    
        
    }

    // TODO Floor 4½ (Monday) — append `value` at the tail. O(1) thanks
    // to `tail_`. The body is the mirror of push_front's Floor 4½ form:
    //
    //     Node* n = new Node(value, /*prev=*/tail_, /*next=*/nullptr);
    //     if (tail_ != nullptr) tail_->next = n;
    //     else                  head_ = n;
    //     tail_ = n;
    //     ++size_;
    void push_back(const T& value) {
        Node* n = new Node(value, tail_, nullptr);
        if (tail_ != nullptr) tail_->next = n;
        else head_ = n;
        tail_ = n;
        ++size_;
    }
    // TODO Floor 4½ (Friday) — remove the front node. O(1).
    //
    // Sketch:
    //   1. If empty, return (no-op is fine — document your choice).
    //   2. Save head_->next as the new head.
    //   3. delete head_.
    //   4. head_ = new head; if it isn't null, set new_head->prev = nullptr.
    //      If it IS null, the chain is now empty — set tail_ = nullptr too.
    //   5. --size_.
    void pop_front() {
        // TODO Friday

        // No-op on empty chain
    if (head_ == nullptr) return;

    Node* old_head = head_;
    head_ = old_head->next;          // advance head_ first
    delete old_head;

    if (head_ != nullptr) {
        head_->prev = nullptr;       // new head has no previous
    } else {
        tail_ = nullptr;             // chain became empty
    }

    --size_;
			
	}
    

    // TODO Floor 4½ (Friday) — remove the back node. O(1) BECAUSE of prev.
    //
    // Sketch:
    //   1. If empty, return.
    //   2. Save tail_->prev as the new tail.
    //   3. delete tail_.
    //   4. tail_ = new tail; if it isn't null, set new_tail->next = nullptr.
    //      If it IS null, head_ = nullptr too.
    //   5. --size_.
    //
    // Question for the lab: why is this O(n) on a singly-linked chain
    // *even if it has a tail_ pointer*?
    void pop_back() {
        // TODO Friday
		if (tail_ == nullptr) return;
		Node* old_tail = tail_;
		Node* new_tail = tail_->prev;
		delete old_tail;
		tail_ = new_tail;
        if (new_tail != nullptr) 
            new_tail->next = nullptr;
        else 
            head_ = nullptr; // chain is now empty
		--size_;
    }

    // Walk and delete every node. Floor 4 version — unchanged loop body,
    // with `tail_` zeroed at the end to keep both endpoints consistent.
    //
    // Note: at the end we zero BOTH endpoint pointers. Forgetting tail_
    // here is a subtle bug — the chain would *look* empty (size_ == 0,
    // head_ == nullptr) but tail_ would dangle at a freed address.
    void clear() {
        Node* p = head_;
        while (p != nullptr) {
            Node* n = p->next;
            delete p;
            p = n;
        }
        head_ = nullptr;
        tail_ = nullptr;   // NEW on Floor 4½ — keep tail_ in lockstep with head_.
        size_ = 0;
    }

private:
    Node*       head_ = nullptr;
    Node*       tail_ = nullptr;   // NEW on Floor 4½.
    std::size_t size_ = 0;
};

// Free-function swap — lets std::swap(a, b) work on chains, and lets
// copy-and-swap call `swap(*this, tmp)` without qualification.
template <typename T>
void swap(Chain<T>& a, Chain<T>& b) noexcept { a.swap(b); }

}  // namespace dungeon
