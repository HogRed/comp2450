// COMP 2450 — Floor 8 starter
// hero/Tree.h — a general (n-ary) tree template. THE WORK IS HERE.
//
// For two weeks you built ADTs by *adapting* a Chain<T>: a Stack was a
// chain you only touched at one end, a Queue was a chain you touched at
// both. The shape underneath was still a line. This week the shape
// changes. A node no longer has *one* next; it has a whole Bag of
// children. The line becomes a branching structure: a tree.
//
//   * A LINE  (Chain): each node has at most one successor.
//   * A TREE  (this):  each node has any number of children, but exactly
//                      one parent. Exactly one node — the ROOT — has no
//                      parent. A node with no children is a LEAF.
//
// =====================================================================
// What is provided vs. what you write
// =====================================================================
// PROVIDED (read it, do not edit): the Node struct, the root accessors,
// makeRoot(), and the Rule of Three (the recursive deep-copy ctor, the
// copy-assign, and the recursive destructor). The copy/destroy machinery
// is recursive in exactly the way your own methods will be — read it as a
// worked example before you write yours.
//
// YOU WRITE (the six TODOs below):
//   Monday    — addChild, size (countFrom), height (heightFrom)
//   Wednesday — preOrder (preOrderInto), levelOrder
//   Friday    — lineage (findFrom)
//
// =====================================================================
// Why a Bag<Node*> for the children, and why a parent pointer
// =====================================================================
// Children live in a Bag<Node*> — your Floor 3 container, holding raw
// node pointers. A tree node owns its children the way a Chain owned its
// nodes: with new in addChild and delete in the (provided) destructor.
//
// Each node also carries a `parent` back-pointer, set when it is added.
// You never needed one in a Stack or Queue. You need it here for the
// same reason Chain<T> kept a `prev`: so you can walk *up* the structure,
// not just down. lineage() (Friday) is the payoff — it walks parents from
// a node back to the root, the tree's answer to Chain's reverse iterator.

#pragma once

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

#include "Bag.h"
#include "Queue.h"   // levelOrder (BFS) reuses your Floor 7 Queue<T>.

namespace dungeon {

template <typename T>
class Tree {
public:
    struct Node {
        T          data;
        Node*      parent;     // nullptr for the root; set by addChild otherwise.
        Bag<Node*> children;   // owned: created with new, freed by destroy().

        explicit Node(const T& v, Node* p = nullptr) : data(v), parent(p) {}
    };

    // ----- lifetime --------------------------------------------------
    // The default tree is empty (no root). makeRoot() plants one.
    Tree() = default;
    ~Tree() { destroy(root_); }

    // Rule of Three, recursive. PROVIDED — and worth reading closely,
    // because countFrom/heightFrom/preOrderInto all recurse over children
    // the same way clone() and destroy() do.
    Tree(const Tree& other) { root_ = clone(other.root_, nullptr); }
    Tree& operator=(const Tree& other) {
        if (this != &other) {
            Tree tmp(other);
            std::swap(root_, tmp.root_);
        }
        return *this;
    }

    // ----- access (PROVIDED) -----------------------------------------
    Node*       root()        { return root_; }
    const Node* root()  const { return root_; }
    bool        empty() const { return root_ == nullptr; }

    // Plant the root if there isn't one yet; return it either way.
    Node* makeRoot(const T& value) {
        if (root_ == nullptr) root_ = new Node(value, nullptr);
        return root_;
    }

    // =================================================================
    // THE SIX THINGS YOU WRITE THIS WEEK
    // =================================================================

    // ----- Monday ----------------------------------------------------

    // Attach a new child holding `value` under `parent`. Allocate the
    // node (set its parent back-pointer to `parent`), push it onto
    // parent->children, and return the new node so the caller can hang
    // grandchildren off it. O(1).
    //
    // TODO Floor 8 (Monday). About three lines:
    //     Node* child = new Node(value, parent);
    //     parent->children.push_back(child);
    //     return child;
    Node* addChild(Node* parent, const T& value) {
		Node* child = new Node(value, parent); //point the new child to its parent
		parent->children.push_back(child); // add the new child to the parent's children bag
		return child; 
    }

    // Total number of nodes in the tree. Recursive: one node, plus the
    // size of every child's subtree.
    //
    // PROVIDED — this delegating body is done. The work is in countFrom()
    // (your Monday TODO, down in the private helpers).
    std::size_t size() const { return countFrom(root_); }

    // Height = number of EDGES on the longest root-to-leaf path. An empty
    // tree and a single-node tree both have height 0; each extra level
    // adds 1. Recursive: 1 + the tallest child subtree (0 at a leaf).
    //
    // PROVIDED — delegating body is done. The work is in heightFrom() below.
    std::size_t height() const { return heightFrom(root_); }

    // ----- Wednesday -------------------------------------------------

    // Depth-first PRE-ORDER: visit a node, THEN recurse into its children
    // left to right. This is the catalogue order — a category appears
    // immediately before everything filed beneath it.
    //
    // PROVIDED — delegating body is done. The work is in preOrderInto()
    // (your Wednesday TODO, down in the private helpers).
    std::vector<T> preOrder() const {
        std::vector<T> out;
        preOrderInto(root_, out);
        return out;
    }

    // Breadth-first LEVEL-ORDER: the root, then every depth-1 node, then
    // every depth-2 node, and so on. Closest-first. This is the walk you
    // were promised on Floor 7: enqueue the root, then loop "dequeue a
    // node, record it, enqueue its children." The Queue<T> you built last
    // week is exactly the bookkeeping that makes closest-first happen.
    //
    // TODO Floor 8 (Wednesday). Iterative, no recursion. Sketch:
    //     if (root_ == nullptr) return out;
    //     Queue<Node*> q;
    //     q.enqueue(root_);
    //     while (!q.empty()) {
    //         Node* cur = q.front();
    //         q.dequeue();
    //         out.push_back(cur->data);
    //         for (std::size_t i = 0; i < cur->children.size(); ++i)
    //             q.enqueue(cur->children[i]);
    //     }
    std::vector<T> levelOrder() const {
        std::vector<T> out;
		return out;  // TODO Wednesday
    }

    // ----- Friday ----------------------------------------------------

    // The lineage of `value`: the path of labels from the root down to
    // the node holding `value`, inclusive. Empty vector if not found.
    //
    // The interesting half — finding the node — is findFrom (your TODO).
    // Once you have the node, the walk UP to the root is the payoff of the
    // parent pointer: follow `parent` until it is nullptr, collecting as
    // you go, then reverse. That part is written for you here.
    //
    // TODO Floor 8 (Friday): write findFrom(root_, value) below.
    std::vector<T> lineage(const T& value) const {
        std::vector<T> path;
        const Node* n = findFrom(root_, value);
        if (n == nullptr) return path;          // not found → empty path
        for (const Node* p = n; p != nullptr; p = p->parent) path.push_back(p->data);
        std::reverse(path.begin(), path.end());  // collected leaf→root; flip it
        return path;
    }

private:
    Node* root_ = nullptr;

    // ===== recursive helpers — PROVIDED ==============================

    // Free an entire subtree: every child first, then the node itself.
    // Post-order by necessity — you cannot delete a node before the
    // children that hang off it, or you lose the pointers to them.
    static void destroy(Node* n) {
        if (n == nullptr) return;
        for (std::size_t i = 0; i < n->children.size(); ++i) destroy(n->children[i]);
        delete n;
    }

    // Deep-copy a subtree under a new parent. Mirrors destroy(): make the
    // node, then recurse to make each child's subtree beneath it.
    static Node* clone(const Node* n, Node* parent) {
        if (n == nullptr) return nullptr;
        Node* copy = new Node(n->data, parent);
        for (std::size_t i = 0; i < n->children.size(); ++i) {
            copy->children.push_back(clone(n->children[i], copy));
        }
        return copy;
    }

    // ===== recursive helpers — YOU WRITE =============================

    // Count this node plus every node beneath it. A null subtree has 0.
    //
    // TODO Floor 8 (Monday). Sketch:
    //     if (n == nullptr) return 0;
    //     std::size_t total = 1;
    //     for (std::size_t i = 0; i < n->children.size(); ++i)
    //         total += countFrom(n->children[i]);
    //     return total;
    static std::size_t countFrom(const Node* n) {
		if (n == nullptr) return 0;
		std::size_t total = 1;
		for (std::size_t i = 0; i < n->children.size(); ++i) {
			total += countFrom(n->children[i]);
		}
		return total;
    }

    // Height (in edges) of the subtree rooted at n. A null subtree and a
    // leaf are both 0; otherwise 1 + the tallest child subtree.
    //
    // TODO Floor 8 (Monday). Sketch:
    //     if (n == nullptr || n->children.size() == 0) return 0;
    //     std::size_t best = 0;
    //     for (std::size_t i = 0; i < n->children.size(); ++i)
    //         best = std::max(best, heightFrom(n->children[i]));
    //     return 1 + best;
    static std::size_t heightFrom(const Node* n) {
		if (n == nullptr || n->children.size() == 0) return 0;
		std::size_t best = 0;
		for (std::size_t i = 0; i < n->children.size(); ++i) {
			best = std::max(best, heightFrom(n->children[i]));
		}
		return 1 + best;
    }

    // Append this node's data, THEN each child's subtree in order. The
    // "node first, then children" ordering is what makes it PRE-order.
    //
    // TODO Floor 8 (Wednesday). Sketch:
    //     if (n == nullptr) return;
    //     out.push_back(n->data);
    //     for (std::size_t i = 0; i < n->children.size(); ++i)
    //         preOrderInto(n->children[i], out);
    static void preOrderInto(const Node* n, std::vector<T>& out) {
		if (n == nullptr) return;
		out.push_back(n->data);
		for (std::size_t i = 0; i < n->children.size(); ++i) {
			preOrderInto(n->children[i], out);
		}
    }

    // Recursive search: return the node whose data == value, or nullptr.
    // Check this node, then ask each child's subtree in turn; return the
    // first hit.
    //
    // TODO Floor 8 (Friday). Sketch:
    //     if (n == nullptr) return nullptr;
    //     if (n->data == value) return n;
    //     for (std::size_t i = 0; i < n->children.size(); ++i)
    //         if (const Node* hit = findFrom(n->children[i], value)) return hit;
    //     return nullptr;
    static const Node* findFrom(const Node* n, const T& value) {
        (void)n; (void)value;
        return nullptr;   // TODO Friday
    }
};

}  // namespace dungeon
