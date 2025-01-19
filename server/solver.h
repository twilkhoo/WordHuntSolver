#ifndef __SOLVER_H__
#define __SOLVER_H__

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

namespace Solver {

struct EnglishWordTrie {
  struct TrieNode {
    char letter;
    std::array<std::shared_ptr<TrieNode>, 26> children;
    bool end;

    TrieNode(char c);
  };

  std::shared_ptr<TrieNode> root;
  EnglishWordTrie(std::string inputFile);
};

class WordFinder {
  // Board dimensions.
  size_t numRows;
  size_t numCols;

  // Mutex for writing to the result vector.
  std::mutex foundWordsMutex;

  // The result vector.
  std::vector<std::vector<std::pair<char, size_t>>> foundWords;

  // A set to store what we've seen so far.
  std::unordered_set<std::string> foundWordsSet;


  // Iterative algorithm to search for words in the board (given the word trie).
  void iterativeFind(const std::vector<std::vector<char>>& board,
                     const std::shared_ptr<EnglishWordTrie::TrieNode> rootNode,
                     const size_t startRow, const size_t startCol);

 public:
  WordFinder(const std::vector<std::vector<char>>& board,
         const EnglishWordTrie& trie);

  std::vector<std::vector<std::pair<char, size_t>>> getFoundWords() const;
};

}  // namespace Solver

#endif  // __SOLVER_H__
