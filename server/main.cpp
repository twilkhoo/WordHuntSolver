#include <chrono>
#include <iostream>
#include <vector>

#include "solver.h"

int main() {
  // A 4x4 input board, as a 2d char arr.
  // Must contain lowercase alphabetical chars.
  // std::vector<std::vector<char>> board{{'a', 'b', 'c', 'd'},
  //                                      {'e', 'f', 'g', 'h'},
  //                                      {'i', 'j', 'k', 'l'},
  //                                      {'m', 'n', 'o', 'p'}};

  std::vector<std::vector<char>> board{{'h','e','l','l','o'}};
  // We want to convert this grid into a trie.
  Solver::EnglishWordTrie trie("../assets/words_alpha.txt");

  // Now that we have a trie, lookup all possible strings to see if we have
  // valid words.
  auto start = std::chrono::high_resolution_clock::now();
  Solver::WordFinder s(board, trie);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedTime = end - start;

  std::cout << s.getFoundWords().size() << "\n";

  std::cout << "Time taken: " << elapsedTime.count() << "\n";
}
