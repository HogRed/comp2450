// COMP 2450 — Floor 9½ starter
// hero/BST.h — your Floor 9 binary search tree, now with BALANCE. THE WORK
// IS HERE (the new section near the bottom).
//
// Last week you built a BST and proved the catch yourself: insert values in
// sorted order and the tree degenerates into a "stick" — height N-1, lookup
// back to O(n). The rule (left < node < right) was correct; it just wasn't
// *helping*. Grix's tree, leaning all the way right, is a list wearing a hat.
//
// This week you do not rebuild the tree. You learn to MEASURE its balance,
// you learn the one structural move that REPAIRS it — a *rotation* — and then
// you watch a real balanced tree (std::set) shrug off the very input that
// flattened yours. The lesson is "compare, don't reinvent": you touch what a
// self-balancing tree does, then reach for the library that does it for you.
//
// =====================================================================
// What is provided vs. what you write
// =====================================================================
// PROVIDED (complete — this is your finished Floor 9 BST): the Node, the
// Rule of Three, access, insert/find/inOrder/remove and their six helpers,
// size()/height(), and — new this week — signedHeight() plus the public
// balance delegators (balanceFactor / isBalanced / rotateRootRight /
// rotateRootLeft / balanceRoot).
//
// YOU WRITE (the five new helpers, near the bottom):
//   Monday    — balanceFactorOf, isBalancedFrom   (diagnose the imbalance)
//   Wednesday — rotateRight, rotateLeft           (the repair primitive)
//   Friday    — rebalanceAt                        (pick the right rotation)
//
// =====================================================================
// One convention to notice: height() vs signedHeight()
// =====================================================================
// Floor 9's height() counts EDGES, so an empty tree and a single leaf are
// both 0. Balance needs a finer ruler: an empty subtree must count as -1,
// one *less* than a leaf. That is the standard balance-factor convention,
// and it is the only reason a three-node stick reads as unbalanced instead
// of balanced. signedHeight() is provided for exactly that.

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
        Node* left  = nullptr;
        Node* right = nullptr;
        explicit Node(const T& v) : data(v) {}
    };

    // ----- lifetime (your Floor 9 Rule of Three — complete) ----------
    BST() = default;
    ~BST() { destroy(root_); }

    BST(const BST& other) { root_ = clone(other.root_); }
    BST& operator=(const BST& other) {
        if (this != &other) {
            BST tmp(other);
            std::swap(root_, tmp.root_);
        }
        return *this;
    }

    // ----- access (PROVIDED) -----------------------------------------
    const Node* root()  const { return root_; }
    bool        empty() const { return root_ == nullptr; }
    std::size_t size()   const { return countFrom(root_); }
    std::size_t height() const { return heightFrom(root_); }   // EDGES

    // ----- Floor 9 operations (PROVIDED delegators, complete) --------
    void insert(const T& value)            { root_ = insertInto(root_, value); }
    bool contains(const T& value) const    { return findFrom(root_, value) != nullptr; }
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

    // ===== Floor 9½ — balance: diagnostics + rotations ===============
    // PROVIDED delegators (like insert/find): each forwards to one of the
    // helpers you write this week.

    // Balance factor of the root: signedHeight(left) - signedHeight(right).
    // 0 = level; positive = left-heavy; negative = right-heavy.
    int balanceFactor() const { return balanceFactorOf(root_); }

    // True iff EVERY node is height-balanced — no node's two subtrees differ
    // in height by more than 1. This is the AVL invariant. A bushy tree
    // passes; Floor 9's sorted-insert stick fails.
    bool isBalanced() const { return isBalancedFrom(root_); }

    // Apply one rotation at the root (if the needed child exists). Returns
    // true if it rotated. The rotation itself is your work; these just call
    // it at the root and re-link root_.
    bool rotateRootRight() {
        if (root_ == nullptr || root_->left == nullptr) return false;
        root_ = rotateRight(root_);
        return true;
    }
    bool rotateRootLeft() {
        if (root_ == nullptr || root_->right == nullptr) return false;
        root_ = rotateLeft(root_);
        return true;
    }

    // Rebalance the root with ONE rotation if it is out of balance. Returns
    // true if it rotated. This only *touches* what a self-balancing tree
    // does: a real AVL tree calls a rebalance at EVERY node on EVERY insert
    // and remove, so it never becomes a stick at all. You write the decision
    // (rebalanceAt); this applies it at the root.
    bool balanceRoot() {
        const int bf = balanceFactorOf(root_);
        root_ = rebalanceAt(root_);
        return bf < -1 || bf > 1;
    }

private:
    Node* root_ = nullptr;

    // ===== size/height — PROVIDED (Floor 8/9 muscle memory) ==========
    static std::size_t countFrom(const Node* n) {
        if (n == nullptr) return 0;
        return 1 + countFrom(n->left) + countFrom(n->right);
    }
    static std::size_t heightFrom(const Node* n) {
        if (n == nullptr || (n->left == nullptr && n->right == nullptr)) return 0;
        return 1 + std::max(heightFrom(n->left), heightFrom(n->right));
    }

    // signedHeight — like heightFrom() but an EMPTY subtree is -1, not 0
    // (a leaf is 0, a missing child is -1). PROVIDED — this is the ruler
    // your balance factor measures with. See the header note.
    static int signedHeight(const Node* n) {
        if (n == nullptr) return -1;
        return 1 + std::max(signedHeight(n->left), signedHeight(n->right));
    }

    // ===== Floor 9 helpers — PROVIDED (your finished work) ===========

    static Node* insertInto(Node* n, const T& value) {
        if (n == nullptr) return new Node(value);
        if (value < n->data)      n->left  = insertInto(n->left,  value);
        else if (n->data < value) n->right = insertInto(n->right, value);
        return n;
    }
    static const Node* findFrom(const Node* n, const T& value) {
        if (n == nullptr)    return nullptr;
        if (value < n->data) return findFrom(n->left,  value);
        if (n->data < value) return findFrom(n->right, value);
        return n;
    }
    static void inOrderInto(const Node* n, std::vector<T>& out) {
        if (n == nullptr) return;
        inOrderInto(n->left, out);
        out.push_back(n->data);
        inOrderInto(n->right, out);
    }
    static void destroy(Node* n) {
        if (n == nullptr) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }
    static Node* clone(const Node* n) {
        if (n == nullptr) return nullptr;
        Node* copy = new Node(n->data);
        copy->left  = clone(n->left);
        copy->right = clone(n->right);
        return copy;
    }
    static Node* removeFrom(Node* n, const T& value, bool& removed) {
        if (n == nullptr) return nullptr;
        if (value < n->data) { n->left  = removeFrom(n->left,  value, removed); return n; }
        if (n->data < value) { n->right = removeFrom(n->right, value, removed); return n; }
        removed = true;
        if (n->left == nullptr)  { Node* r = n->right; delete n; return r; }
        if (n->right == nullptr) { Node* l = n->left;  delete n; return l; }
        Node* succ = n->right;
        while (succ->left != nullptr) succ = succ->left;
        n->data = succ->data;
        bool discard = false;
        n->right = removeFrom(n->right, succ->data, discard);
        return n;
    }

    // ===== Floor 9½ helpers — YOU WRITE ==============================

    // ----- Monday: diagnose imbalance --------------------------------

    // Balance factor of n: signedHeight(left) - signedHeight(right). A null
    // node is 0. Positive = left-heavy, negative = right-heavy.
    //
    // TODO Floor 9½ (Monday). Body is two lines:
    //     if (n == nullptr) return 0;
    //     return signedHeight(n->left) - signedHeight(n->right);
    static int balanceFactorOf(const Node* n) {
        // TODO Monday — stub reports every tree as perfectly level
        if (n == nullptr) return 0;
        return signedHeight(n->left) - signedHeight(n->right);
    }

    // True iff the subtree at n is height-balanced at EVERY node. Check THIS
    // node's balance factor is within [-1, 1], then require both children to
    // be balanced too.
    //
    // TODO Floor 9½ (Monday). Sketch:
    //     if (n == nullptr) return true;
    //     const int bf = balanceFactorOf(n);
    //     if (bf < -1 || bf > 1) return false;
    //     return isBalancedFrom(n->left) && isBalancedFrom(n->right);
    static bool isBalancedFrom(const Node* n) {
        // TODO Monday — stub claims every tree is balanced
        if (n == nullptr) return true;
        const int bf = balanceFactorOf(n);
        if (bf < -1 || bf > 1) return false;
        return isBalancedFrom(n->left) && isBalancedFrom(n - right);
    }

    // ----- Wednesday: the rotation primitive -------------------------

    // Right rotation around n. n's LEFT child (pivot) becomes the new subtree
    // root; n becomes pivot's right child; pivot's old right subtree becomes
    // n's new left. Return the new subtree root. Precondition: n and n->left
    // are non-null. A rotation does NOT change the in-order order — that is
    // exactly what makes it a safe repair.
    //
    /*
               n                 pivot
              / \                /   \
          pivot  c    =>        a     n
           / \                        / \
          a   b                      b   c
    */
    // TODO Floor 9½ (Wednesday). Sketch:
    //     Node* pivot  = n->left;
    //     n->left      = pivot->right;
    //     pivot->right = n;
    //     return pivot;
    static Node* rotateRight(Node* n) {
        // TODO Wednesday — stub returns the tree unrotated
        Node* pivot = n->left;
        n->left = pivot->right;
        pivot->right = n;
        return pivot;
    }

    // Left rotation around n — the mirror of rotateRight. n's RIGHT child
    // becomes the new subtree root. Precondition: n and n->right non-null.
    // Also in-order-preserving.
    //
    // TODO Floor 9½ (Wednesday). Sketch:
    //     Node* pivot = n->right;
    //     n->right    = pivot->left;
    //     pivot->left = n;
    //     return pivot;
    static Node* rotateLeft(Node* n) {
        // TODO Wednesday — stub returns the tree unrotated
        Node* pivot = n->right;
        n->right = pivot->left;
        pivot->left = n;
        return pivot;
    }

    // ----- Friday: pick the rotation ---------------------------------

    // If n is out of balance, apply the one rotation that fixes a simple
    // imbalance: rotate right if left-heavy, left if right-heavy. Return the
    // (possibly new) subtree root. A real AVL tree calls this at every node
    // on the way back up an insert/remove (and adds a second rotation for the
    // zig-zag cases); here we touch the single-rotation case at one node.
    //
    // TODO Floor 9½ (Friday). Sketch:
    //     if (n == nullptr) return n;
    //     const int bf = balanceFactorOf(n);
    //     if (bf > 1)  return rotateRight(n);   // left-heavy
    //     if (bf < -1) return rotateLeft(n);    // right-heavy
    //     return n;
    static Node* rebalanceAt(Node* n) {
        // TODO Friday — stub never rebalances
        if (n == nullptr) return n;
        const int bf = balanceFactorOf(n);
        if (bf > 1) return rotateRight(n); //left-heavy
        if (bf < -1) return rotateLeft(n); // right-heavy
        return n;
    }
};

}  // namespace dungeon
