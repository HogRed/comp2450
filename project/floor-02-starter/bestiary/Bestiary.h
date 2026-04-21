// COMP 2450 — Floor 1 starter
// bestiary/Bestiary.h — provided by the framework. Do not edit.
//
// Loads and prints monsters. Sorting helper provided so you don't
// have to think about std::sort yet — your job is the search.
#pragma once
#include <string>
#include <vector>
#include "Monster.h"

namespace dungeon {

// Read monsters from a pipe-delimited text file into a vector.
// File format, one per line:  name|hp|attack|weakness
// Blank lines and lines starting with '#' are ignored.
// On read failure returns an empty vector.
std::vector<Monster> loadBestiary(const std::string& path);

// Sort the bestiary by name ascending. Call this BEFORE binarySearch.
void sortBestiary(std::vector<Monster>& bestiary);

// Pretty-print one monster to stdout.
void printMonster(const Monster& m);

}
