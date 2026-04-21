# Floor 0 Starter — *The Antechamber*

This is the starter code for your semester-long dungeon-crawler project. By the end of the semester, this folder will be a full game. This week, it does almost nothing. That's the point.

## What this program does

Prompts for your hero's name, then prints one line of dungeon flavor text. That's it. You are not implementing a data structure yet. You are confirming that your **compiler, editor, and terminal** all work together on your machine.

## What you need to do

1. **Install a C++ compiler** if you haven't already.
   - Windows: install Visual Studio Build Tools (which ships MSVC), MSYS2/MinGW-w64, or WSL.
   - macOS: `xcode-select --install` gets you `clang++`.
   - Linux: `sudo apt install g++` or equivalent.
2. **Install CMake** (version 3.15 or newer) if you haven't already.
   - Windows: `winget install Kitware.CMake`, or get it from [cmake.org](https://cmake.org/download/).
   - macOS: `brew install cmake`.
   - Linux: `sudo apt install cmake` or equivalent.
3. **Configure and build:**
   ```
   cmake -B build
   cmake --build build
   ```
   The first command creates a `build/` folder and picks a sensible build system for your platform (Visual Studio on Windows-with-VS, Unix Makefiles on macOS/Linux, Ninja if installed). The second command does the actual compile.
4. **Run it:**
   ```
   ./build/antechamber           # macOS / Linux
   build\antechamber.exe         # Windows (cmd.exe)
   ```
4. **Edit `main.cpp`.** Find the line marked `// TODO Floor 0:` and replace the placeholder `"Change me, adventurer."` string with your own flavor line.
5. **Commit and push** your change to your project repo. Your commit message should start with `Floor 0: `.

## Deliverable for Lab 0

A terminal screenshot (or pasted output) showing your program running with *your* hero's name and *your* flavor text, plus one commit visible in your project repo.

## If you get stuck

The **Compile Demon** is waiting. Common fixes:

- `cmake: command not found` — CMake isn't installed or isn't on `PATH`. Re-check your install.
- `No CMAKE_CXX_COMPILER could be found` — you have CMake but no C++ compiler. See step 1.
- `'cout' was not declared` — you're missing `#include <iostream>` or `using namespace std;`.
- Mysterious linker errors or stale build — wipe the build dir and reconfigure: `rm -rf build && cmake -B build && cmake --build build`.

Bring any error you can't defeat to Monday's class. We'll fight it together.
