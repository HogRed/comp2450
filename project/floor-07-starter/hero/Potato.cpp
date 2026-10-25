// COMP 2450 — Floor 7 starter
// hero/Potato.cpp — body of hotPotato.  THE WORK IS HERE (Wednesday).

#include "Potato.h"

#include "Queue.h"

namespace dungeon {

// TODO Floor 7 (Wednesday).
//
// The classic FIFO elimination algorithm. Load names into a Queue,
// then rotate (front -> back) k-1 times and dequeue the unlucky one.
// Repeat until one remains.
//
//   * Handle the empty-input case up front — return "" if names empty.
//   * The single-element case shakes out naturally — the loop never
//     enters, and front() returns the survivor.
//   * The rotation is exactly k-1 "move-front-to-back" operations.
//     For k = 1, that's zero rotations: each round just drops the front.
//
// Reference body (do NOT paste — type it from memory in class):
//
//     if (names.empty()) return "";
//     Queue<std::string> q;
//     for (const std::string& n : names) q.enqueue(n);
//     while (q.size() > 1) {
//         for (std::size_t i = 0; i < k - 1; ++i) {
//             q.enqueue(q.front());
//             q.dequeue();
//         }
//         q.dequeue();
//     }
//     return q.front();
//
// The structural lesson: enqueue, dequeue, and front are the ENTIRE
// surface area of this algorithm. No indexing, no random access. That
// is what "FIFO is enough for some problems" means in code.

std::string hotPotato(const std::vector<std::string>& names, std::size_t k) {
	if (names.empty()) return "";
	Queue<std::string> q;
	for (const std::string& n : names) {
		q.enqueue(n);
	}
	while (q.size() > 1) {
		for (std::size_t i = 0; i < k - 1; ++i) {
			q.enqueue(q.front());
			q.dequeue();
		}
		q.dequeue();
	}
	return q.front();
}

}  // namespace dungeon
