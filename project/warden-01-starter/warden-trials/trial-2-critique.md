# Trial II — *The Critique*

*Warden of the Foundations · Midterm 1 · 15 %*

This trial **requires** you to use an LLM. You will paste the prompt and the reply, then critique what you got. Reading and judging AI output is part of the job now; we are practicing it on purpose.

---

## The prompt

Open Claude (or your LLM of choice) and paste **this exact prompt**:

> *Write a C++ function template that returns the largest element of a `std::vector<T>`. Handle the case where the vector is empty. Briefly explain your design choices.*

Tell us which tool you used and the date:

- **Tool:** Claude.ai
- **Date of the conversation:** 2026/09/17

---

## The LLM's reply

Paste the **full reply**, code and prose. Use a fenced code block for the code. Do not edit the reply — we want to see what you actually got.

```cpp
#include <algorithm>
#include <optional>
#include <vector>

template <typename T>
std::optional<T> largest(const std::vector<T>& v) {
    if (v.empty()) {
        return std::nullopt;
    }
    return *std::max_element(v.begin(), v.end());
}
```
Usage:
```cpp
std::vector<int> nums{3, 9, 2};
if (auto m = largest(nums)) {
    std::cout << *m << '\n';   // 9
} else {
    std::cout << "empty\n";
}
```

Design choices

std::optional<T> for the empty case. An empty vector has no maximum, so the return type should say so. Returning T{} would be indistinguishable from a real value (0 might be the actual max), and throwing makes a routine condition exceptional. optional makes the caller handle the empty case explicitly.
std::max_element. It's standard, well-tested, and only requires operator< on T. It returns the first of several equal maxima.
const std::vector<T>& parameter. No copy of the vector, and the function doesn't modify it.
Returns a copy of the element. This is simple and safe, but it copies T, which could be costly for large types. If that matters, return std::optional<std::reference_wrapper<const T>> or a const T* (null when empty), but then the caller must not let the vector outlive or change under the reference.

Possible extensions

Add a comparator parameter (template <typename T, typename Compare = std::less<T>>) for custom orderings.
For floating-point types, NaNs make < comparisons unreliable, so filter them out first if your data can contain them.

---

## My critique  *( ≥ 150 words )*

Your critique must read like an engineer reviewing a colleague's pull request — specific, technical, and naming exactly what the LLM did. A vague critique ("looks fine to me, but it could be better") is *not* full credit even if the LLM's code was great.

Address each of these:

- **Compile.** Did you actually try to compile and run the code? On what types? What happened?
- **Empty case.** What does the empty-vector case actually do in this code? Is the LLM's choice reasonable? Is it different from what *you* would have chosen?
- **Requirements on `T`.** What does the code require of `T`? What happens if you instantiate it with a type that doesn't support `<` (e.g., a custom struct)?
- **Const-correctness.** Pass by value or by reference? Return by value or by reference? Would you change either?
- **`std::max_element`?** Did the LLM cheat by using the algorithm header, or did it write the loop? Which would *you* have shipped, and why?
- **One concrete improvement.** Name one specific change you would make before merging this into a real project. Not "make it better" — a concrete code change.
