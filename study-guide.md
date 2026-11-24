---
title: Study Guide — The Final Descent
permalink: /study-guide/
---

# Study Guide · The Final Descent

*The Architect of Errors does not ask what you memorized. It hands you something that looks right and asks what it does.*

## What the exam looks like

- **25 multiple-choice questions, 4 points each, 100 points.** Four choices, one best answer.
- **Cumulative.** Every floor from 0 to 12 appears.
- **Closed notes, closed laptop.** No code to write. No penalty for guessing, so answer everything.
- **Most questions make you trace or diagnose.** You will follow code by hand, draw a structure as it changes, or find what is wrong with a piece of code that looks fine.

| Part | Covers | Worth |
|------|--------|-------|
| I | Floors 0–3: cost, searching, sorting, templates, exceptions | 20 points |
| II | Floors 4–7: lists, Rule of Three, iterators, stacks, queues | 28 points |
| III | Floors 8–9½: trees, BSTs, balance | 20 points |
| IV | Floors 10–11: sets, maps, hash tables | 16 points |
| V | Floor 12: graphs | 16 points |

**Not on the exam:** heaps and priority-queue internals, treaps, coding a full AVL or red-black insert, Shell sort, radix sort, Dijkstra's steps, Bellman-Ford, minimum spanning trees.

Wrong choices are not random. Each one is what you get by making one specific, common mistake. If your answer matches a choice quickly, check that you did not just make the popular mistake.

## How to study

Rereading the floor pages will feel productive and help very little. Do this instead.

1. **Draw it.** For every container, draw the boxes and arrows before and after each operation. Most questions fall to a careful picture.
2. **Predict, then run.** Change one line in a starter, write down what you expect, then build and run it. Every "break it on purpose" lab item is worth doing again.
3. **Explain the bug.** For each floor's bestiary monster, finish this sentence out loud: *"Nothing crashes, but…"*
4. **Use the practice problems below closed-book.** Write an answer on paper *before* you open the answer. They are short-answer on purpose: if you can produce the answer, you can recognize it.

A five-sitting plan, about an hour each:

| Sitting | Do this |
|---|---|
| 1 | Part I practice, then trace `merge` and `partition` on a list of seven numbers you make up |
| 2 | Part II practice, then redo Lab 4½ items 2 and 3 and Lab 5 items 2 and 3 |
| 3 | Part III practice, then redo Lab 9 items 2–4 and Lab 9½ items 2–4 on paper |
| 4 | Parts IV and V practice, then redo Lab 10 item 5, Lab 11 item 5, and Lab 12 item 4 |
| 5 | Every problem you missed, again, from a blank page |

## The costs you must know cold

| Structure | Find | Insert | Remove | Comes out in sorted order? |
|---|---|---|---|---|
| `Bag` / `std::vector`, unsorted | O(n) | O(1) at the back, O(n) at the front | O(n) from the front or middle | no |
| `Bag`, kept sorted | O(log n) by binary search | O(n), because it shifts | O(n), because it shifts | yes |
| `Chain` (doubly linked) | O(n) | O(1) at either end | O(1) at either end | no |
| `Stack`, `Queue` | not offered | O(1) | O(1) | no |
| General `Tree` | O(n) | O(1) once you have the parent | — | no |
| `BST`, bushy | O(log n) | O(log n) | O(log n) | yes, in-order |
| `BST`, stick | O(n) | O(n) | O(n) | yes, in-order |
| `std::set`, `std::map` | O(log n) guaranteed | O(log n) | O(log n) | yes |
| `HashTable`, `std::unordered_*` | O(1) average, O(n) worst | O(1) average | O(1) average | no |

| Sort | Average | Worst | Stable? | In place? |
|---|---|---|---|---|
| Merge sort | O(n log n) | O(n log n) | yes | no, it needs a scratch buffer |
| Quicksort (ours) | O(n log n) | O(n²) | no | yes |
| `std::sort` | O(n log n) | O(n log n) | not guaranteed | yes |

Breadth-first and depth-first crossings cost **O(V + E)** on adjacency lists and **O(V²)** on an adjacency matrix.

---

## Part I · Floors 0–3

**Be able to**

- Say which **ADT** a problem needs and which **data structure** would implement it, and why those are two different questions.
- Count work, not lines: nested loops, loops that double, a loop that hides a search inside it. Add up the *total* cost of N operations, not just the cost of one.
- Trace binary search exactly: every midpoint, with the half-open range `[low, high)`.
- Say what a search does when its input is *not* sorted, and why it never crashes.
- Trace `merge` and `partition`. Say what stability means, and which line of `merge` makes it stable.
- Predict what is printed when an exception is thrown several calls deep, caught, and possibly re-thrown.
- Say *when* a template mistake is found, and what the compiler stamps out and when.

**Traps**

- "It ran without errors" says nothing about a binary search on unsorted data.
- A pivot always taken from the first or the last slot is at its worst on data that is already sorted.
- `at()` checks and throws. `operator[]` checks nothing and is allowed to do anything.
- A handler for a base class catches every derived exception. After a handler finishes normally, control continues *after* the `try`/`catch`, never back where the throw was.

### Practice

**1.** Give the Big-O of each, in terms of `N`.

```cpp
// (a)
for (int i = 0; i < N; ++i)
    for (int j = i; j < N; ++j)
        ++count;

// (b)
for (int i = 1; i < N; i *= 2)
    linearSearch(bestiary, name);      // the bestiary holds N monsters
```

<details markdown="1">
<summary>Answer</summary>

(a) **O(N²).** The inner loop runs N, then N−1, then N−2, … times. That sum is about N²/2, and the ½ is a constant. (b) **O(N log N).** The outer loop doubles `i`, so it runs about log N times, and each pass hides an O(N) search.

</details>

**2.** `v = {3, 8, 12, 17, 21, 30, 44, 59}`. Using the half-open binary search from Floor 1, list every value compared with the target, in order, for `30` and then for `10`.

<details markdown="1">
<summary>Answer</summary>

For 30: **21, 44, 30.** The range `[0,8)` probes index 4 (21), so `low = 5`. The range `[5,8)` probes index 6 (44), so `high = 6`. The range `[5,6)` probes index 5 (30).

For 10: **21, 12, 8**, then the range is empty. Index 4 (21) sends it left to `[0,4)`; index 2 (12) sends it left to `[0,2)`; index 1 (8) sends it right to `[2,2)`.

</details>

**3.** Trace the course's `partition` (middle pivot, swapped to the end, then one left-to-right scan) on `8 1 5 9 3`. What is in the vector afterward, and what index is returned?

<details markdown="1">
<summary>Answer</summary>

**`1 3 5 9 8`, and it returns 2.** The pivot is the 5 at index 2. Swap it to the end: `8 1 3 9 5`. Scan: 8 is not less; 1 is, so swap it into slot 0 (`1 8 3 9 5`); 3 is, so swap it into slot 1 (`1 3 8 9 5`); 9 is not. Finally swap the pivot into slot 2. The left side is smaller and the right side is larger, but neither side has been sorted yet.

</details>

**4.** Items arrive in this order: `Axe 3`, `Bow 1`, `Cap 3`, `Dirk 1`. A **stable** sort by weight runs. What order comes out? Name one output that is also correctly ordered by weight but proves the sort was *not* stable.

<details markdown="1">
<summary>Answer</summary>

Stable: **Bow 1, Dirk 1, Axe 3, Cap 3.** Anything that swaps a tied pair, such as `Dirk 1, Bow 1, Axe 3, Cap 3`, is still ordered by weight but shows the sort was not stable.

</details>

**5.** `Bag<int> b` is empty. What does this print?

```cpp
void deep(Bag<int>& b)   { std::cout << "1"; b.at(5); std::cout << "2"; }
void middle(Bag<int>& b) { std::cout << "3"; deep(b); std::cout << "4"; }

try {
    middle(b);
    std::cout << "5";
} catch (const std::exception& e) {
    std::cout << "6";
}
std::cout << "7";
```

<details markdown="1">
<summary>Answer</summary>

**`3167`.** `at(5)` throws. `deep` and `middle` have no handler, so 2, 4, and 5 are all skipped. The base-class handler prints 6, and execution continues after the `try`/`catch` with 7.

</details>

**6.** `findByName` is a function template. A program calls it on the bestiary in three places and on the inventory in two. How many versions of `findByName` does the compiler stamp out? Why must its body live in a header?

<details markdown="1">
<summary>Answer</summary>

**Two**: one for `Monster` and one for `Item`. The compiler stamps one per type, not one per call. The body must be in a header because the compiler can stamp a version only where it can see the whole body.

</details>

---

## Part II · Floors 4–7

**Be able to**

- Draw every pointer change for `push_front`, `push_back`, `pop_front`, `pop_back`, an insert in the middle, and `eraseFirst`, including the empty and one-node cases. Say which order of statements loses a pointer.
- State the Rule of Three. Count how many nodes a piece of code allocates, including pass-by-value, copy construction, and assignment.
- Explain shallow versus deep copy, and exactly when the double delete happens.
- Explain copy-and-swap: what is built first, what is swapped, what is destroyed, and why that order is safe in every case.
- Name the five iterator categories, what each adds, and which one `std::find`, `std::reverse`, and `std::sort` require.
- Use `end()` correctly: it is one past the last element, and it is what `std::find` returns on a miss.
- Build LIFO and FIFO from the same `Chain` by choosing ends. Trace bracket balancing, undo, hot potato, and a queue made from two stacks.

**Traps**

- A walk that looks right *forward* can still be broken *backward*. Check both directions.
- Reading anything through a node you already deleted is undefined behavior, even if it seems to work.
- A reference to the top or front is only good while that node exists.
- `chain[i]` in a loop is the Quadratic Mire. `++it` is not.
- Our `pop()` and `dequeue()` return nothing. Read with `top()` or `front()` first, and only after checking `empty()`.

### Practice

**7.** `p` points at a *middle* node of a doubly linked chain. Write the statements that unlink it and free it. Which statement must come last, and why?

<details markdown="1">
<summary>Answer</summary>

`p->prev->next = p->next;`, then `p->next->prev = p->prev;`, then `delete p;`. The first two may trade places. The `delete` must be last because both of the others read pointers stored inside `*p`.

</details>

**8.** Someone deletes the copy constructor you wrote, so the compiler generates one. What goes wrong here, and at which line does it happen?

```cpp
{
    Chain<int> a;
    a.push_back(1);
    a.push_back(2);
    Chain<int> b = a;
    b.push_back(3);
}   // end of scope
```

<details markdown="1">
<summary>Answer</summary>

The generated copy constructor copies the pointers, so `a` and `b` share nodes 1 and 2. `b.push_back(3)` links node 3 onto the shared node 2, so a forward walk of `a` now reaches a third element while `a.size()` still says 2 and `a.tail()` still points at node 2. The disaster is **at the closing brace**: `b` is destroyed first and deletes all three nodes, then `a`'s destructor walks and deletes memory that is already gone.

</details>

**9.** For each call, say whether a `Chain` iterator (bidirectional) is strong enough: `std::find`, `std::reverse`, `std::sort`, `std::distance`, `it += 3`.

<details markdown="1">
<summary>Answer</summary>

`std::find`, `std::reverse`, and `std::distance` work. (`std::distance` has to step one node at a time, so it is O(n) on a chain.) `std::sort` and `it += 3` need random access and do not compile.

</details>

**10.** A `Chain<int>` holds `5 1 4`. What does this print?

```cpp
for (auto it = c.rbegin(); it != c.rend(); ++it)
    std::cout << *it << ' ';
auto hit = std::find(c.begin(), c.end(), 2);
std::cout << (hit == c.end());
```

<details markdown="1">
<summary>Answer</summary>

**`4 1 5 1`.** The reverse iterators walk from tail to head. 2 is not in the chain, so `find` returns `end()` and the comparison prints 1 (true).

</details>

**11.** Run the bracket balancer on `{[()()]}`. What is the most characters the stack ever holds? Then run it on `({)}`: at which character does it answer *not balanced*, and what is on top of the stack at that moment?

<details markdown="1">
<summary>Answer</summary>

**Three** (`{`, `[`, and `(`). For `({)}` it stops at the third character, the `)`, because the top of the stack is `{`.

</details>

**12.** Hot potato with `A B C D E F` and `k = 2` (rotate once, then eliminate the front). Give the elimination order and the survivor.

<details markdown="1">
<summary>Answer</summary>

Eliminated: **B, D, F, C, A**. Survivor: **E**. After F is out, the queue reads `A C E`: A rotates and C is out, leaving `E A`; E rotates and A is out.

</details>

**13.** What does this print?

```cpp
Stack<int> s;
Queue<int> q;
for (int i = 1; i <= 5; ++i) {
    if (i % 2 == 1) s.push(i);
    else            q.enqueue(i);
}
while (!s.empty()) { q.enqueue(s.top()); s.pop(); }
while (!q.empty()) { s.push(q.front()); q.dequeue(); }
while (!s.empty()) { std::cout << s.top() << ' '; s.pop(); }
```

<details markdown="1">
<summary>Answer</summary>

**`1 3 5 4 2`.** The odds sit on the stack with 5 on top, and the queue holds `2 4`. Draining the stack makes the queue `2 4 5 3 1`. Moving the queue back onto the stack leaves 1 on top, so it prints in reverse: `1 3 5 4 2`.

</details>

---

## Part III · Floors 8–9½

**Be able to**

- Use the vocabulary exactly: depth and height count **edges**, and a one-node tree has height 0.
- Produce pre-order, post-order, in-order, and level-order from a drawing. Say which container or recursion produces which.
- Insert and remove in a BST by hand, including a two-child removal where the successor has a right child.
- Check the BST invariant *for the whole path*, not just parent against child.
- Compute balance factors with the empty subtree at **−1**. Decide whether a tree is AVL-balanced by checking **every** node.
- Perform a left or right rotation, including where the middle subtree goes, and re-check the balance afterward.

**Traps**

- A root balance factor of 0 does not make a tree balanced.
- A rotation can change height. It never changes the in-order order. It does not always fix the lean.
- `height()` calls an empty tree 0. `signedHeight` calls it −1. Balance factors use `signedHeight`.
- In-order is sorted no matter how badly the tree is shaped. Correct and fast are different claims.

### Practice

**14.** A general tree: `R` has children `X, Y`. `X` has children `P, Q`. `Q` has child `Z`. `Y` has child `W`. Give the pre-order, post-order, and level-order, the height, and the depth of `Q`.

<details markdown="1">
<summary>Answer</summary>

Pre-order **R X P Q Z Y W**. Post-order **P Z Q X W Y R**. Level-order **R X Y P Q W Z**. Height **3** (R, X, Q, Z is three edges). Depth of Q is **2**.

</details>

**15.** Insert `40 20 60 10 30 50 70 25 35` into an empty BST. (a) Which nodes does `find(35)` visit? (b) Remove 20 the course's way. What is the pre-order now? (c) What is the in-order?

<details markdown="1">
<summary>Answer</summary>

(a) **40, 20, 30, 35.** (b) 20 has two children; its successor is 25, the smallest value on its right. 25 moves up and the old 25 leaf is removed: **40 25 10 30 35 60 50 70**. (c) **10 25 30 35 40 50 60 70**, sorted, as it must be.

</details>

**16.** A classmate draws this and says it is a valid BST because every child is on the correct side of its parent. Is it?

```text
        50
       /  \
     30    70
    /  \
  20    60
```

<details markdown="1">
<summary>Answer</summary>

**No.** 60 is correctly to the right of 30, but it is in 50's *left* subtree and it is larger than 50. A call to `find(60)` goes right at 50 and never sees it. The invariant covers the whole subtree, not just parent and child.

</details>

**17.** Insert `10 20 30 25 40` into an empty BST. (a) What is the root's balance factor? (b) Which single rotation does `rebalanceAt` do at the root? (c) Draw the result. Is it balanced now?

<details markdown="1">
<summary>Answer</summary>

(a) The left side is empty (−1) and the right side has height 2, so the factor is **−3**. (b) **Rotate left.** (c) 20 becomes the root, with 10 on its left and 30 on its right; 30 keeps 25 and 40. The root's factor is 0 − 1 = **−1**, and every other node's is 0, so **yes**, it is balanced.

</details>

**18.** You rotate right at node `n`, whose left child is `p`. Where does `p`'s old right subtree end up, and why is that the only legal place?

<details markdown="1">
<summary>Answer</summary>

It becomes **`n`'s left subtree**. Everything in it is larger than `p` and smaller than `n`. After the rotation, `n` is `p`'s right child, so "right of `p`, left of `n`" is exactly that spot.

</details>

---

## Part IV · Floors 10–11

**Be able to**

- Choose among `std::set`, `std::map`, `std::unordered_set`, `std::unordered_map`, a sorted `Bag`, and your `BST` when a problem gives you *several* demands at once.
- Say precisely what `[]`, `at`, `find`, `count`, `insert`, and `erase` do to a map when the key is missing.
- Predict the order a map or set prints in, and say why a hash container's order is not something you can rely on.
- Trace a separate-chaining table: hash code, then modulo, then chain. Compute the load factor, decide when it grows, and re-file every item.
- Explain how a bad hash, or a rehash that files items in the wrong place, breaks the table *without* crashing.

**Traps**

- `m[key]` on a missing key inserts it, even inside an `if`.
- "Greater than the threshold" and "at least the threshold" give different answers. Read which one is asked.
- Compute the load factor *after* the insert, with the bucket count at that moment.
- More buckets cannot rescue a hash that gives two keys the same hash *code*.
- Erasing from one index does not touch any other index.

### Practice

**19.** What does this print?

```cpp
std::map<std::string, int> m;
m["lich"] = 2;
if (m.find("imp") == m.end()) m["lich"] += 1;
std::cout << m["wisp"];
std::cout << m.size() << m["lich"] << m.count("imp");
```

<details markdown="1">
<summary>Answer</summary>

**`0230`.** `find` never inserts, so `imp` stays out and `lich` becomes 3. Reading `m["wisp"]` inserts it with value 0 and prints 0. The size is now 2, `lich` is 3, and the count of `imp` is 0.

</details>

**20.** Pick one container for each. (a) "Has this room been visited?", millions of times, order never matters. (b) How many times each monster was fought, printed alphabetically. (c) The distinct weaknesses, printed alphabetically. (d) The last ten commands, undone newest first.

<details markdown="1">
<summary>Answer</summary>

(a) `std::unordered_set`. (b) `std::map` from name to count. (c) `std::set`. (d) a stack.

</details>

**21.** A separate-chaining table has 8 buckets, and a key's hash code is the key itself. Insert `9, 17, 4, 25, 12` with no resizing. Draw the buckets and give the load factor. Now rehash to 16 buckets and draw it again.

<details markdown="1">
<summary>Answer</summary>

With 8 buckets: bucket 1 holds 9, 17, 25; bucket 4 holds 4, 12. The load factor is 5/8 = **0.625**. With 16: bucket 1 holds 17; bucket 4 holds 4; bucket 9 holds 9, 25; bucket 12 holds 12. The load factor is 0.3125. Every item's home had to be recomputed, but not every home changed.

</details>

**22.** Two names, *Goblin* and *Wraith*, share a bucket. A student's `find` computes the bucket and returns the first item in it. Which lookups are wrong, and which are right by luck?

<details markdown="1">
<summary>Answer</summary>

Looking up whichever name is first in the chain works by luck. Looking up the other returns the wrong monster. An *absent* name that hashes to any non-empty bucket "finds" a monster that is not it. `find` must compare the **name**, not just the bucket.

</details>

---

## Part V · Floor 12

**Be able to**

- Write out the adjacency lists from a series of `connect` calls, in order, with both directions.
- Trace `bfsOrder` and `dfsOrder` exactly as we wrote them, showing the queue or stack *and* the visited set at each step.
- Say when a room is marked: at enqueue in our BFS (which is also when `hops` sets its distance), at pop in our stack DFS. Say what would change if that moved, or if the set were missing.
- Explain why BFS gives the fewest doors, and why it would not give the cheapest route if doors had weights.
- Compare the adjacency list and the matrix for space, for finding a room's neighbors, and for a whole crossing.

**Traps**

- A stack hands back the neighbor you pushed **last**, so our DFS and a recursive DFS usually visit in different orders.
- A room can sit on the DFS stack more than once. It is visited once.
- A room is 0 doors from itself. Count doors, not rooms.
- Without a visited set, a single two-way door is already enough to loop forever.

### Practice

**23.** These calls build an undirected graph: `connect(S,A)`, `connect(S,B)`, `connect(A,C)`, `connect(B,C)`, `connect(C,D)`, `connect(B,E)`. Give the adjacency lists, `bfsOrder(S)`, `dfsOrder(S)`, and the number of hops from `S` to `D`.

<details markdown="1">
<summary>Answer</summary>

Lists: S: A B. A: S C. B: S C E. C: A B D. D: C. E: B.

BFS: **S A B C E D**. DFS with our stack: **S B E C D A**. Hops from S to D: **3** (S, A, C, D).

DFS in detail: pop S, push A then B. Pop B, push C then E. Pop E. Pop C, push A then D. Pop D. Pop A. The copy of A left at the bottom is skipped.

</details>

**24.** A student's `connect(a, b)` adds `b` to `a`'s list but forgets the other direction. After `connect(A,B)` and `connect(B,C)`, which of `reachable(A,C)`, `reachable(C,A)`, and `reachable(B,B)` return true?

<details markdown="1">
<summary>Answer</summary>

`reachable(A,C)` is **true** and `reachable(B,B)` is **true** (the start is the target). `reachable(C,A)` is **false**, because no list leads back.

(This assumes `connect` still makes sure both rooms exist. If it didn't, C would not be on the map at all.)

</details>

**25.** Where does the V come from, and where does the E come from, in O(V + E) for a BFS on adjacency lists? Why is the same crossing O(V²) on a matrix, even when there are very few doors?

<details markdown="1">
<summary>Answer</summary>

**V**: each room is enqueued and dequeued once. **E**: each room's list is scanned once, and all the lists together are proportional to the number of doors. On a matrix, finding one room's neighbors means scanning a whole row of V cells, most of them empty, and that happens for each of the V rooms.

</details>

**26.** Every door now has a cost. Going straight from `S` to `T` costs 10; going `S` to `M` to `T` costs 3 + 3. What does BFS report as the shortest way from `S` to `T`, and is it the cheapest?

<details markdown="1">
<summary>Answer</summary>

BFS counts doors, so it reports the direct route: **1 door, cost 10**. The cheapest is the two-door route at cost 6. Weighted graphs need a different algorithm (Dijkstra's).

</details>

---

## The night before

You are ready if you can do these on a blank page, without looking anything up:

- Trace `partition` and `merge` on seven numbers.
- Draw all four pointer changes for a doubly linked insert in the middle, in a safe order.
- Write the Rule of Three for `Chain` from memory and say what each piece prevents.
- Remove a two-child node from a BST, then give the pre-order.
- Find every balance factor in a ten-node tree.
- Walk a hash table through two resizes.
- Trace BFS and our stack DFS on the same six-room map and get two different, correct orders.

*Sleep. The Bard has been singing your ending since the first floor.*
