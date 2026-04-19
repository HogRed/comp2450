# Floor 0 Starter — *The Antechamber*

This is the starter code for your semester-long dungeon-crawler project. By the end of the semester, this folder will be a full game. This week, it does almost nothing. That's the point.

## What this program does

Prompts for your hero's name, then prints one line of dungeon flavor text. That's it. You are not implementing a data structure yet. You are confirming that your **compiler, editor, and terminal** all work together on your machine.

## What you need to do

1. **Install a C++ compiler** if you haven't already.
   - Windows: install MSYS2/MinGW-w64 (so `g++` is on your `PATH`), or use WSL.
   - macOS: `xcode-select --install` gets you `clang++`.
   - Linux: `sudo apt install g++` or equivalent.
2. **Build the program:**
   ```
   g++ -std=c++17 -Wall -o antechamber main.cpp
   ```
   Or, if you have `make`:
   ```
   make
   ```
3. **Run it:**
   ```
   ./antechamber
   ```
   (On Windows: `antechamber.exe`.)
4. **Edit `main.cpp`.** Find the line marked `// TODO Floor 0:` and replace the placeholder `"Change me, adventurer."` string with your own flavor line.
5. **Commit and push** your change to your project repo. Your commit message should start with `Floor 0: `.

## Deliverable for Lab 0

A terminal screenshot (or pasted output) showing your program running with *your* hero's name and *your* flavor text, plus one commit visible in your project repo.

## If you get stuck

The **Compile Demon** is waiting. Common fixes:

- `g++: command not found` — your compiler isn't on `PATH`. Re-check your install.
- `'cout' was not declared` — you're missing `#include <iostream>` or `using namespace std;`.
- Mysterious linker errors — delete any old `.o` files and rebuild from scratch (`make clean && make`).

Bring any error you can't defeat to Monday's class. We'll fight it together.
