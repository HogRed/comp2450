// COMP 2450 — Floor 5 starter
// hero/Chain.h — doubly-linked container template with custom iterators.
//
// You arrive on Floor 5 with a fully working doubly-linked Chain<T> from
// Floor 4½ — push_front, push_back, pop_front, pop_back, the destructor,
// clear, deep copy ctor, and copy assignment. All preserved below.
//
// This week's three-day arc:
//   Monday    — define Chain<T>::iterator (Node*-wrapped) with five ops:
//               * -> ++ == !=  ;  wire begin() and end().
//   Wednesday — define Chain<T>::const_iterator (const Node*-wrapped);
//               wire cbegin() and cend(); rewrite findByName<T> to call
//               std::find_if internally (one line — in bestiary/Search.h).
//   Friday    — implement operator-- on iterator and const_iterator so
//               std::reverse and rbegin/rend work.  Demonstrate that
//               std::sort still refuses to compile (random-access only).
//
// Once your iterators are right, four things change at once:
//   * range-based for works:    for (const auto& s : eventLog) ...
//   * std::find / std::find_if work on Chain just like on Bag
//   * the same templated printLog walks Chain<std::string> AND Bag<Item>
//   * std::reverse works (after Friday's operator--)
//
// The reverse_iterator helpers (rbegin/rend/crbegin/crend) are wired
// pre-built below using std::reverse_iterator.  They piggyback on your
// underlying iterator's operator++ / operator-- — so they start working
// the moment those work.

#pragma once

#include <cstddef>
#include <iterator>   // std::reverse_iterator, std::bidirectional_iterator_tag
#include <utility>

namespace dungeon {

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
    struct Node {
        T     data;
        Node* prev;
        Node* next;

        explicit Node(const T& v, Node* p = nullptr, Node* n = nullptr)
            : data(v), prev(p), next(n) {
            ++detail::NodeStats::allocations;
        }
        ~Node() { ++detail::NodeStats::deallocations; }
    };

    // =================================================================
    // iterator — Floor 5's main work. A small object that points at one
    // Node and knows how to advance (++), retreat (--), dereference (*),
    // member-access (->), and compare (== / !=).
    //
    // The five typedefs below let std::iterator_traits<iterator> work —
    // which is what std::find_if, std::reverse, range-based for, and the
    // rest of the standard library use to discover what your iterator
    // can do.  Leave the typedefs as written; the OPERATORS are your job.
    // =================================================================
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        iterator() : p_(nullptr), owner_(nullptr) {}
        iterator(Node* n, Chain* o) : p_(n), owner_(o) {}

        // TODO Floor 5 (Monday) — return a reference to the node's data.
        //
        //     return p_->data;
        //
        // The stub returns *p_'s data through a deliberate nullptr deref
        // ONLY IF callers reach it; in practice they don't, because the
        // stubbed operator== (below) makes begin() == end() always true,
        // so range loops never enter the body.  Once you wire == and
        // ++ correctly, this is the only operator that needs a real body.
        reference operator*()  const { return p_->data; }
        pointer   operator->() const { return &p_->data; }

        // TODO Floor 5 (Monday) — advance to the next node.
        //   pre-increment:    p_ = p_->next;  return *this;
        //   post-increment:   iterator tmp = *this;  ++(*this);  return tmp;
        iterator& operator++()    { p_ = p_->next;  return *this; }
        iterator  operator++(int) { iterator t = *this;  ++(*this);  return t; }

        // TODO Floor 5 (Friday) — retreat to the previous node.  Needed
        // by std::reverse and by std::reverse_iterator (rbegin/rend).
        //
        // The subtle bit: when *this is end() (p_ == nullptr), we still
        // need to step back to the LAST node — but there's no `nullptr->prev`
        // to follow.  That's why iterators carry an owner_ pointer to
        // their Chain: when p_ is null, hop to owner_->tail_ instead.
        //
        //   pre-decrement:    p_ = p_ ? p_->prev : owner_->tail_;  return *this;
        //   post-decrement:   iterator tmp = *this;  --(*this);  return tmp;
        iterator& operator--()    { /* TODO Friday */                       return *this; }
        iterator  operator--(int) { /* TODO Friday */ iterator t = *this;   return t;     }

        // TODO Floor 5 (Monday) — compare the underlying Node*.
        // (owner_ is not part of identity — two iterators into the same
        // chain that point at the same node are equal regardless of how
        // they were spelled.)  The stub returns TRUE so begin() == end()
        // and loops skip, which keeps the program runnable while
        // operator++ is still empty.
        //   return p_ == other.p_;
        bool operator==(const iterator& other) const { return p_ == other.p_; }
        bool operator!=(const iterator& other)     const { return !(*this == other); }

        // Const-correctness handles for the const_iterator's converting ctor.
        Node*  node()  const { return p_; }
        Chain* owner() const { return owner_; }

    private:
        Node*  p_;
        Chain* owner_;   // Needed by operator-- so --end() can find the tail.
    };

    // =================================================================
    // const_iterator — Wednesday's work.  Identical shape to iterator
    // except its * / -> return const references / pointers, and it can
    // be constructed FROM an iterator (one-way: iterator → const_iterator
    // is fine; the reverse would let callers strip const).
    // =================================================================
    class const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;

        const_iterator() : p_(nullptr), owner_(nullptr) {}
        const_iterator(const Node* n, const Chain* o) : p_(n), owner_(o) {}
        // Allow implicit iterator → const_iterator (the SAFE direction).
        const_iterator(const iterator& it) : p_(it.node()), owner_(it.owner()) {}

        // TODO Floor 5 (Wednesday) — return a const reference to p_->data.
        //     return p_->data;
        reference operator*()  const { return p_->data; }
        pointer   operator->() const { return &p_->data; }

        // TODO Floor 5 (Wednesday) — advance via p_->next, exactly as
        // iterator does, just on a const Node*.
        const_iterator& operator++()    { p_ = p_->next;  return *this; }
        const_iterator  operator++(int) { const_iterator t = *this;  ++(*this);  return t; }

        // TODO Floor 5 (Friday) — retreat via p_->prev, with the same
        // end-of-chain fallback as iterator::operator--:
        //   p_ = p_ ? p_->prev : owner_->tail_;
        const_iterator& operator--()    { /* TODO Friday */                               return *this; }
        const_iterator  operator--(int) { /* TODO Friday */ const_iterator t = *this;     return t;     }

        // TODO Floor 5 (Wednesday) — return p_ == other.p_;  stub is TRUE
        // for the same reason as iterator (loops skip; build stays green).
        bool operator==(const const_iterator& other) const { return p_ == other.p_; }
        bool operator!=(const const_iterator& other)     const { return !(*this == other); }

    private:
        const Node*  p_;
        const Chain* owner_;
    };

    // Reverse iterators are wired pre-built using std::reverse_iterator.
    // They depend on the underlying iterator's operator-- — so they
    // return the right answers AS SOON AS Friday's operator-- works.
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // -----------------------------------------------------------------
    // Construction / destruction — unchanged from Floor 4½.
    // -----------------------------------------------------------------
    Chain() = default;
    ~Chain() { clear(); }

    Chain(const Chain& other) {
        for (const Node* p = other.head_; p != nullptr; p = p->next) {
            push_back(p->data);
        }
    }

    Chain& operator=(const Chain& other) {
        Chain tmp(other);
        swap(tmp);
        return *this;
    }

    void swap(Chain& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

    // -----------------------------------------------------------------
    // Inspection — Floor 4½ versions, plus the new begin/end family.
    // -----------------------------------------------------------------
    std::size_t size()  const { return size_; }
    bool        empty() const { return size_ == 0; }

    const Node* head() const { return head_; }
    Node*       head()       { return head_; }
    const Node* tail() const { return tail_; }
    Node*       tail()       { return tail_; }

    // TODO Floor 5 (Monday) — return iterator(head_, this) and
    // iterator(nullptr, this).
    //
    // The "end" iterator points ONE PAST the last element. For a
    // null-terminated chain, that's nullptr — there's no real node beyond
    // the tail. Loops run while `it != end`, advancing via ++.  We pass
    // `this` so the iterator can find the tail in operator-- when
    // walking backward from end (see iterator::operator--).
    iterator begin() { return iterator(head_, this); }
    iterator end()   { return iterator(nullptr, this); }

    // TODO Floor 5 (Wednesday) — same shape, but const_iterator.
    // The cbegin / cend overloads give callers a way to ASK for a
    // const_iterator from a non-const Chain (useful for templated code).
    const_iterator begin()  const { return const_iterator(head_, this); }
    const_iterator end()    const { return const_iterator(nullptr, this); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend()   const { return end(); }
    
    // Reverse iterators — wired automatically.
    reverse_iterator       rbegin()        { return reverse_iterator(end()); }
    reverse_iterator       rend()          { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin()  const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()    const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend()   const { return rend(); }

    // -----------------------------------------------------------------
    // Mutation — Floor 4½ versions, unchanged.
    // -----------------------------------------------------------------
    void push_front(const T& value) {
        Node* n = new Node(value, /*prev=*/nullptr, /*next=*/head_);
        if (head_ != nullptr) head_->prev = n;
        else                  tail_       = n;
        head_ = n;
        ++size_;
    }

    void push_back(const T& value) {
        Node* n = new Node(value, /*prev=*/tail_, /*next=*/nullptr);
        if (tail_ != nullptr) tail_->next = n;
        else                  head_       = n;
        tail_ = n;
        ++size_;
    }

    void pop_front() {
        if (head_ == nullptr) return;
        Node* old_head = head_;
        Node* new_head = old_head->next;
        delete old_head;
        head_ = new_head;
        if (new_head != nullptr) new_head->prev = nullptr;
        else                     tail_          = nullptr;
        --size_;
    }

    void pop_back() {
        if (tail_ == nullptr) return;
        Node* old_tail = tail_;
        Node* new_tail = old_tail->prev;
        delete old_tail;
        tail_ = new_tail;
        if (new_tail != nullptr) new_tail->next = nullptr;
        else                     head_          = nullptr;
        --size_;
    }

    void clear() {
        Node* p = head_;
        while (p != nullptr) {
            Node* n = p->next;
            delete p;
            p = n;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

private:
    Node*       head_ = nullptr;
    Node*       tail_ = nullptr;
    std::size_t size_ = 0;
};

template <typename T>
void swap(Chain<T>& a, Chain<T>& b) noexcept { a.swap(b); }

}  // namespace dungeon
