// COMP 2450 — Floor 6 starter
// hero/Lint.cpp — body of isBalanced.  THE WORK IS HERE (Wednesday).

#include "Lint.h"

#include "Stack.h"

namespace dungeon {

// TODO Floor 6 (Wednesday).
//
// The classic stack-balancing algorithm. Walk `s` left-to-right:
//
//   * If c is '(', '[', or '{':   push it on the stack.
//   * If c is ')', ']', or '}':
//       - if the stack is empty, the string is unbalanced — return false
//         (we hit a closer with nothing waiting to be closed).
//       - otherwise peek at the top:
//           if top is the matching opener, pop and continue;
//           if it is the WRONG opener, return false
//             (a `)` cannot close a `{`).
//   * Any other character is ignored — text, spaces, digits, letters.
//
// At the end of the loop, the string is balanced iff the stack is empty.
// (If anything is left on the stack, those openers were never closed.)
//
// Reference body (do NOT paste — type it from memory in class):
//
//     Stack<char> s;
//     for (char c : input) {
//         switch (c) {
//             case '(': case '[': case '{':
//                 s.push(c); break;
//             case ')':
//                 if (s.empty() || s.top() != '(') return false;
//                 s.pop(); break;
//             case ']':
//                 if (s.empty() || s.top() != '[') return false;
//                 s.pop(); break;
//             case '}':
//                 if (s.empty() || s.top() != '{') return false;
//                 s.pop(); break;
//             default:
//                 break;
//         }
//     }
//     return s.empty();

bool isBalanced(const std::string& input) {
    Stack<char> s;
	for (char c : input) {
		switch (c) {
		case '(': case '[': case '{':
			s.push(c); break;
		case ')':
			if (s.empty() || s.top() != '(') return false;
			s.pop(); break;
		case ']':
			if (s.empty() || s.top() != '[') return false;
			s.pop(); break;
		case '}':
			if (s.empty() || s.top() != '{') return false;
			s.pop(); break;
		default: // ignore any other character
			break;
		}
	}
	return s.empty(); // if anything is left on the stack, those openers were never closed
}

}  // namespace dungeon
