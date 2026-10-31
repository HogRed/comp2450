// COMP 2450 — Floor 9½ starter
// hero/BalanceTests.cpp — the `selftest balance` harness. Provided. Do not edit.
//
// Five phases over BST<int>. The lesson is in the order: measure balance
// (phase 1), confirm a bushy tree is balanced (phase 2), watch last week's
// sorted stick FAIL the balance check (phase 3 — the degeneracy, now
// diagnosed), repair structure with a rotation that leaves the in-order order
// untouched (phase 4), and let balanceRoot pick the correct rotation on its
// own (phase 5).
//
// MWF: Monday's balanceFactorOf + isBalancedFrom green phases 1–3; Wednesday's
// rotateRight/rotateLeft green phase 4; Friday's rebalanceAt greens phase 5.

#include "BalanceTests.h"

#include <iostream>
#include <vector>

#include "BST.h"

namespace dungeon {

namespace {

void result(const char* phase, bool ok, const char* diag = nullptr) {
    std::cout << "  " << phase << ": " << (ok ? "OK" : "FAIL");
    if (!ok && diag) std::cout << " — " << diag;
    std::cout << "\n";
}

// {5,3,8,2,4,7,9} — the balanced sample from Floor 9 (height 2).
BST<int> buildBushy() {
    BST<int> t;
    for (int v : {5, 3, 8, 2, 4, 7, 9}) t.insert(v);
    return t;
}

}  // anonymous namespace

bool runBalanceSelfTest() {
    bool all = true;

    // --- Phase 1: balance factor = signedHeight(left) - signedHeight(right) ---
    {
        BST<int> empty;
        BST<int> bushy = buildBushy();
        BST<int> leftLeaning;                  // {2,1}: root has only a left child
        leftLeaning.insert(2);
        leftLeaning.insert(1);
        const bool ok = (empty.balanceFactor() == 0
                      && bushy.balanceFactor() == 0
                      && leftLeaning.balanceFactor() == 1);
        result("balanceFactor: empty==0, bushy==0, left-leaning==1", ok,
               "balanceFactorOf must be signedHeight(left) - signedHeight(right); an empty subtree counts as -1");
        all = all && ok;
    }

    // --- Phase 2: a bushy tree is balanced ---
    {
        BST<int> bushy = buildBushy();
        const bool ok = bushy.isBalanced();
        result("isBalanced: bushy {5,3,8,2,4,7,9} balanced (true)", ok,
               "isBalancedFrom must verify every node's |balance factor| <= 1");
        all = all && ok;
    }

    // --- Phase 3: a sorted-insert stick is NOT balanced (Floor 9's degeneracy) ---
    {
        BST<int> stick;
        for (int v : {1, 2, 3, 4, 5, 6}) stick.insert(v);   // already sorted
        const bool ok = (!stick.isBalanced() && stick.height() == 5);
        result("degenerate: sorted stick {1..6} is NOT balanced (height 5)", ok,
               "isBalancedFrom must return false when ANY node leans by more than 1 EITHER way (bf < -1 || bf > 1); the stick leans right, so its factors are negative");
        all = all && ok;
    }

    // --- Phase 4: a rotation repairs structure AND preserves in-order ---
    {
        BST<int> t;
        for (int v : {1, 2, 3}) t.insert(v);                // right-leaning stick
        const std::vector<int> before = t.inOrder();        // {1,2,3}
        t.rotateRootLeft();                                  // lift 2 to the root
        const bool structure = (t.root() != nullptr && t.root()->data == 2
                                && t.height() == 1 && t.isBalanced());
        const bool order = (t.inOrder() == before);          // order must be unchanged
        const bool ok = structure && order;
        result("rotate: rotateRootLeft on {1,2,3} -> root 2, height 1, order kept", ok,
               "rotateLeft must lift n->right to the root; a rotation must NOT change the in-order order");
        all = all && ok;
    }

    // --- Phase 5: balanceRoot picks the correct single rotation, both ways ---
    {
        BST<int> rightHeavy;            // {1,2,3} -> right-heavy -> needs a left rotation
        for (int v : {1, 2, 3}) rightHeavy.insert(v);
        BST<int> leftHeavy;             // {3,2,1} -> left-heavy  -> needs a right rotation
        for (int v : {3, 2, 1}) leftHeavy.insert(v);
        const std::vector<int> expected = {1, 2, 3};
        bool ok = rightHeavy.balanceRoot() && rightHeavy.isBalanced()
                  && rightHeavy.inOrder() == expected;
        ok = ok && leftHeavy.balanceRoot() && leftHeavy.isBalanced()
                  && leftHeavy.inOrder() == expected;
        result("balanceRoot: right- and left-heavy sticks both rebalance (order kept)", ok,
               "rebalanceAt must rotate left when bf<-1 and right when bf>1");
        all = all && ok;
    }

    std::cout << "  " << (all ? "all phases OK" : "(see FAILs above)") << "\n";
    return all;
}

}  // namespace dungeon
