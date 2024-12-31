#include "solver.h"

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

EnglishWordTrie::TrieNode::TrieNode(char c) : letter{c}, end{false} {};

EnglishWordTrie::EnglishWordTrie(std::string inputFile)
    : root(std::make_shared<TrieNode>(0)) {
  // First, create the trie based on the dictionary words.
  std::ifstream file(inputFile);
  if (!file) throw std::runtime_error("Error opening the file!");

  std::string word;
  while (std::getline(file, word)) {
    std::cout << word << std::endl;

    word.pop_back();  // Remove the carriage return.

    // Add the word into the trie, letter by letter.
    auto curNode = root;

    for (char c : word) {
      if (!curNode->children[c - 'a'])
        curNode->children[c - 'a'] = std::make_shared<TrieNode>(c);
      curNode = curNode->children[c - 'a'];
    }
    curNode->end = true;
  }

  file.close();
}

void Solution::recurseFind(
    const std::vector<std::vector<char>>& board,
    const std::shared_ptr<EnglishWordTrie::TrieNode> curNode, const size_t row,
    const size_t col) {
  // Base case- this node doesn't exist.
  if (!curNode) return;

  // Base case- we've already visited this cell.
  size_t idx = row * numRows + col;
  if (seen.contains(idx)) return;

  // Check if we can proceed in our search with our current board char.
  auto nextNode = curNode->children[board[row][col] - 'a'];
  if (!nextNode) return;

  // If we've got this far, we can continue.
  seen.insert(idx);
  curWord.emplace_back(board[row][col], idx);

  // See if our current spot has a word.
  if (nextNode->end) foundWords.push_back(curWord);

  // Recursively check neighbors.
  for (int i = static_cast<int>(row) - 1; i <= static_cast<int>(row) + 1; i++) {
    for (int j = static_cast<int>(col) - 1; j <= static_cast<int>(col) + 1;
         j++) {
      // Ensure the neighbour is in range.
      if (i < 0 || i >= numRows || j < 0 || j >= numCols) continue;

      // Don't access the current cell again.
      if (i == row && j == col) continue;

      recurseFind(board, nextNode, i, j);
    }
  }

  seen.erase(idx);
  curWord.pop_back();
}

void Solution::iterativeFind(
    const std::vector<std::vector<char>>& board,
    const std::shared_ptr<EnglishWordTrie::TrieNode> rootNode,
    const size_t startRow, const size_t startCol) {
  struct DFSFrame {
    size_t row;
    size_t col;
    std::shared_ptr<EnglishWordTrie::TrieNode> curNode;

    std::vector<std::pair<char, size_t>> curWord;
    std::unordered_set<size_t> seen;
  };

  std::stack<DFSFrame> stack;

  // Push the initial position onto the stack.
  stack.push({startRow, startCol, rootNode, {}, {}});

  while (!stack.empty()) {
    // Pop the topmost frame.
    auto [row, col, curNode, curWord, seen] = stack.top();
    stack.pop();

    // If no node, continue.
    if (!curNode) continue;

    // Check if we've seen this node already.
    size_t idx = row * numRows + col;
    if (seen.contains(idx)) continue;

    // See if we have a match.
    char c = board[row][col];
    auto nextNode = curNode->children[c - 'a'];
    if (!nextNode) continue;

    seen.insert(idx);
    curWord.emplace_back(c, idx);

    // Found a word.
    if (nextNode->end) {
      std::lock_guard<std::mutex> lock(foundWordsMutex);
      foundWords.push_back(curWord);
    }

    // Add neighbors.
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        // Skip the cell itself.
        if (i == 0 && j == 0) continue;

        int nextRow = static_cast<int>(row) + i;
        int nextCol = static_cast<int>(col) + j;

        // Ensure neighbor is valid.
        if (nextRow < 0 || nextRow >= static_cast<int>(numRows) ||
            nextCol < 0 || nextCol >= static_cast<int>(numCols))
          continue;

        stack.push({static_cast<size_t>(nextRow), static_cast<size_t>(nextCol),
                    nextNode, curWord, seen});
      }
    }
  }
}

Solution::Solution(const std::vector<std::vector<char>>& board,
                   const EnglishWordTrie& trie)
    : numRows{board.size()}, numCols{board[0].size()} {
  std::vector<std::thread> threads;
  threads.reserve(numRows * numCols);

  for (size_t row = 0; row < numRows; row++) {
    for (size_t col = 0; col < numCols; col++) {
      threads.emplace_back(
          [&, row, col] { iterativeFind(board, trie.root, row, col); });
    }
  }

  for (auto& t : threads) {
    t.join();
  }
}

std::vector<std::vector<std::pair<char, size_t>>> Solution::getFoundWords()
    const {
  return foundWords;
}

}  // namespace Solver
