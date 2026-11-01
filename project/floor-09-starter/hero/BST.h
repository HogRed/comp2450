// COMP 2450 — Floor 9 starter
// hero/BST.h — a binary search tree template. THE WORK IS HERE.
//
// Last floor you built a GENERAL tree: a node had a whole Bag of children,
// and to search it you had to ask *every* child (findFrom recursed into all
// of them). A general tree gives no search speedup over McCown's old flat
// list — you scan everything.
//
// This week the tree gets ONE rule, and that rule changes everything:
//
//     For every node:  everything in its LEFT subtree is smaller,
//                       everything in its RIGHT subtree is larger.
//
// That is the BINARY SEARCH TREE invariant. A node has at most two children
// — left and right — and they are *sorted*. The payoff: to search, you no
// longer ask every child. You compare once and go ONE way — left if you
// want something smaller, right if larger. Each step throws away half the
// tree. O(n) scan becomes O(log n) descent. This is the "tree that knows
// how to search itself" the Bard promised you on the stair.
//
// =====================================================================
// What is provided vs. what you write
// =====================================================================
// PROVIDED (read it, do not edit): the Node struct, the access methods,
// size()/height() (you wrote those exact recursive shapes on Floor 8), and
// the public operations (insert/find/contains/inOrder/remove) — which all
// delegate to the six private helpers below.
//
// YOU WRITE (the six TODOs):
//   Monday    — insertInto, findFrom
//   Wednesday — inOrderInto, destroy, clone
//   Friday    — removeFrom
//
// =====================================================================
// Wednesday is a promise from Floor 8 coming due
// =====================================================================
// On Floor 8 the recursive destructor and deep-copy ctor were PROVIDED —
// you read them as worked examples and were told you'd write them "next
// week, because a BST owns its nodes the same way." This is next week.
// destroy() and clone() are yours now. Their shape is exactly the Floor 8
// Tree versions with `left` and `right` instead of a bag of children.

#pragma once

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

namespace dungeon {

template <typename T>
class BST {
public:
    struct Node {
        T     data;
        Node* left = nullptr;
        Node* right = nullptr;
        explicit Node(const T& v) : data(v) {}
    };

    // ----- lifetime --------------------------------------------------
    // The destructor and copy ctor delegate to destroy() and clone() —
    // the two recursive helpers that are your Wednesday work. Until you
    // write them, a BST destroys nothing and copies to empty (safe, but
    // leaks and doesn't really deep-copy — selftest phase 6 catches it).
    BST() = default;
    ~BST() { destroy(root_); }

    BST(const BST& other) { root_ = clone(other.root_); }
    BST& operator=(const BST& other) {
        if (this != &other) {
            BST tmp(other);
            std::swap(root_, tmp.root_);   // free once you've written clone + destroy
        }
        return *this;
    }

    // ----- access (PROVIDED) -----------------------------------------
    const Node* root()  const { return root_; }
    bool        empty() const { return root_ == nullptr; }

    // size() and height() are PROVIDED — you wrote these exact recursive
    // shapes (countFrom / heightFrom) on Floor 8. Height is in EDGES: an
    // empty tree and a single node are both 0.
    std::size_t size()   const { return countFrom(root_); }
    std::size_t height() const { return heightFrom(root_); }

    // ----- public operations (PROVIDED delegators) -------------------
    // Each forwards to one of the six private helpers you write below.
    void insert(const T& value) { root_ = insertInto(root_, value); }
    bool contains(const T& value) const { return findFrom(root_, value) != nullptr; }
    const Node* find(const T& value) const { return findFrom(root_, value); }
    std::vector<T> inOrder() const {
        std::vector<T> out;
        inOrderInto(root_, out);
        return out;
    }
    bool remove(const T& value) {
        bool removed = false;
        root_ = removeFrom(root_, value, removed);
        return removed;
    }

private:
    Node* root_ = nullptr;

    // ===== size/height — PROVIDED (Floor 8 muscle memory) ============
    static std::size_t countFrom(const Node* n) {
        if (n == nullptr) return 0;
        return 1 + countFrom(n->left) + countFrom(n->right);
    }
    static std::size_t heightFrom(const Node* n) {
        if (n == nullptr || (n->left == nullptr && n->right == nullptr)) return 0;
        return 1 + std::max(heightFrom(n->left), heightFrom(n->right));
    }

    // ===== recursive helpers — YOU WRITE =============================

    // ----- Monday ----------------------------------------------------

    // Insert `value` in its sorted place. A null `n` is the empty slot
    // where the value belongs — make the node there. Otherwise go left if
    // value is smaller, right if larger; equal means it's already in the
    // tree (do nothing — no duplicates). Return the subtree root so the
    // caller can re-link to it.
    //
    // TODO Floor 9 (Monday). Sketch:
    //     if (n == nullptr) return new Node(value);
    //     if (value < n->data)      n->left  = insertInto(n->left,  value);
    //     else if (n->data < value) n->right = insertInto(n->right, value);
    //     return n;
    static Node* insertInto(Node* n, const T& value) {
		if (n == nullptr) return new Node(value);
		if (value < n->data) {
			n->left = insertInto(n->left, value);
		} else if (n->data < value) {
			n->right = insertInto(n->right, value);
		}
		return n;
    }

    // Search for `value`. Unlike Floor 8's findFrom, you do NOT ask every
    // child — you compare once and recurse into ONE side. That is the whole
    // O(log n) win. Return the matching node, or nullptr if absent.
    //
    // TODO Floor 9 (Monday). Sketch:
    //     if (n == nullptr)    return nullptr;
    //     if (value < n->data) return findFrom(n->left,  value);
    //     if (n->data < value) return findFrom(n->right, value);
    //     return n;   // equal => found
    static const Node* findFrom(const Node* n, const T& value) {
		if (n == nullptr) return nullptr;
		if (value < n->data) return findFrom(n->left, value);
		if (n->data < value) return findFrom(n->right, value);
		return n;
    }

    // ----- Wednesday -------------------------------------------------

    // Append every value in sorted (ascending) order. The trick is the
    // ORDER of the three lines: recurse left, push THIS node, recurse
    // right. Because left holds everything smaller and right everything
    // larger, that walk emerges sorted — the BST's "free sort."
    //
    // TODO Floor 9 (Wednesday). Sketch:
    //     if (n == nullptr) return;
    //     inOrderInto(n->left, out);
    //     out.push_back(n->data);
    //     inOrderInto(n->right, out);
    static void inOrderInto(const Node* n, std::vector<T>& out) {
        if (n == nullptr) return;
        inOrderInto(n->left, out);
        out.push_back(n->data);
        inOrderInto(n->right, out);
    }

    // Free the whole subtree. This is the Floor 8 Tree destructor with
    // left/right instead of a child bag: free both children FIRST, then
    // delete the node (delete the node first and you lose its subtrees).
    //
    // TODO Floor 9 (Wednesday). Sketch:
    //     if (n == nullptr) return;
    //     destroy(n->left);
    //     destroy(n->right);
    //     delete n;
    static void destroy(Node* n) {
        if (n == nullptr) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    // Deep-copy the whole subtree. Mirrors destroy(): make this node, then
    // copy each side beneath it. Returns the new subtree (or nullptr).
    //
    // TODO Floor 9 (Wednesday). Sketch:
    //     if (n == nullptr) return nullptr;
    //     Node* copy = new Node(n->data);
    //     copy->left  = clone(n->left);
    //     copy->right = clone(n->right);
    //     return copy;
    static Node* clone(const Node* n) {
        if (n == nullptr) return nullptr;
        Node* copy = new Node(n->data);
        copy->left = clone(n->left);
        copy->right = clone(n->right);
        return copy;
    }

    // ----- Friday ----------------------------------------------------

    // Remove `value`. Descend to find it (same one-way walk as findFrom),
    // set `removed` to true when you do, and handle THREE cases:
    //   * no left child  -> return the right child (this also covers a leaf,
    //                       whose right child is nullptr)
    //   * no right child -> return the left child
    //   * two children   -> copy the in-order SUCCESSOR (the leftmost node
    //                       of the right subtree) into this node, then
    //                       remove that successor from the right subtree.
    // Return the new subtree root so the parent re-links to it.
    //
    // TODO Floor 9 (Friday). Sketch:
    //     if (n == nullptr) return nullptr;
    //     if (value < n->data) { n->left  = removeFrom(n->left,  value, removed); return n; }
    //     if (n->data < value) { n->right = removeFrom(n->right, value, removed); return n; }
    //     removed = true;
    //     if (n->left  == nullptr) { Node* r = n->right; delete n; return r; }
    //     if (n->right == nullptr) { Node* l = n->left;  delete n; return l; }
    //     Node* succ = n->right;
    //     while (succ->left != nullptr) succ = succ->left;
    //     n->data = succ->data;
    //     bool discard = false;
    //     n->right = removeFrom(n->right, succ->data, discard);
    //     return n;
    static Node* removeFrom(Node* n, const T& value, bool& removed) {
        if (n == nullptr) return nullptr;
        if (value < n->data) {
            n->left = removeFrom(n->left, value, removed);
            return n;
        }
        if (n->data < value) {
            n->right = removeFrom(n->right, value, removed);
            return n;
        }
        removed = true;
        if (n->left == nullptr) {
            Node* r = n->right;
            delete n;
            return r;
        }
        if (n->right == nullptr) {
            Node* l = n->left;
            delete n;
            return l;
        }
        Node* succ = n->right;
        while (succ->left != nullptr) {
            succ = succ->left;
        }
        n->data = succ->data;
        bool discard = false;
        n->right = removeFrom(n->right, succ->data, discard);
        return n;
    }
};
}  // namespace dungeon