#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_set>

struct EnglishWordTrie {
  struct TrieNode {
    char letter;
    std::array<std::shared_ptr<TrieNode>, 26> children;
    bool end;

    TrieNode(char c) : letter{c}, end{false} {};
  };

  std::shared_ptr<TrieNode> root;

  EnglishWordTrie(std::string inputFile) : root(std::make_shared<TrieNode>(0)) {
    // First, create the trie based on the dictionary words.
    std::ifstream file(inputFile); // Replace "example.txt" with your file's name
    if (!file) throw std::runtime_error("Error opening the file!");

    std::string word;
    while (std::getline(file, word)) {
      std::cout << word << std::endl;

      word.pop_back(); // Remove the carriage return.

      // Add the word into the trie, letter by letter.
      auto curNode = root;

      for (char c : word) {
        if (!curNode->children[c - 'a']) curNode->children[c - 'a'] = std::make_shared<TrieNode>(c);
        curNode = curNode->children[c - 'a'];
      }
      curNode->end = true;
    }

    file.close();
  }
};


class Solver {

  size_t numRows;
  size_t numCols;

  std::vector<std::vector<std::pair<char, size_t>>> foundWords;
  std::vector<std::pair<char, size_t>> curWord;
  std::unordered_set<size_t> seen;
  std::string curWordStr;

  // Maintain invariant of board[row][col] always being valid.
  void recurseFind(const std::vector<std::vector<char>>& board, std::shared_ptr<EnglishWordTrie::TrieNode> curNode, size_t row, size_t col) {

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
    curWordStr.push_back(board[row][col]);
    curWord.emplace_back(board[row][col], idx);

    // See if our current spot has a word.
    if (nextNode->end) {
      foundWords.push_back(curWord);
      std::cout << "Found a word: " << curWordStr << std::endl;
    }

    // Recursively check neighbors.
    for (int i = static_cast<int>(row) - 1; i <= static_cast<int>(row) + 1; i++) {
      for (int j = static_cast<int>(col) - 1; j <= static_cast<int>(col) + 1; j++) {
        // Ensure the neighbour is in range.
        if (i < 0 || i >= numRows || j < 0 || j >= numCols) continue;
        
        // Don't access the current cell again.
        if (i == row && j == col) continue;

        recurseFind(board, nextNode, i, j);
      }
    }
    
    seen.erase(idx);
    curWord.pop_back();
    curWordStr.pop_back();
  }

public:
  Solver(const std::vector<std::vector<char>>& board, const EnglishWordTrie& trie) : numRows{board.size()}, numCols{board[0].size()} {
    for (size_t row = 0; row < numRows; row++) {
      for (size_t col = 0; col < numCols; col++) {
        recurseFind(board, trie.root, row, col);
      }
    }
  }

  std::vector<std::vector<std::pair<char, size_t>>> getFoundWords() {
    return foundWords;
  }
};


int main() {
  
  // A 4x4 input board, as a 2d char arr.
  // Must contain lowercase alphabetical chars.
  std::vector<std::vector<char>> board {
    {'a', 'b', 'c', 'd'},
    {'e', 'f', 'g', 'h'},
    {'i', 'j', 'k', 'l'},
    {'m', 'n', 'o', 'p'}
  };

  // We want to convert this grid into a trie.
  EnglishWordTrie trie("words_alpha.txt");

  // Now that we have a trie, lookup all possible strings to see if we have valid words.
  Solver s(board, trie);

  std::cout << s.getFoundWords().size() << std::endl;

}
