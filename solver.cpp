#include <vector>
#include <memory>
#include <unordered_set>
#include <utility>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

class SolverTrie {

  struct LetterObj {
    char c; // An alphabetical character.
    size_t idx; // An index, from 0 to 15 for a 4x4 board.

    LetterObj(char c_, size_t idx_) : c{c_}, idx{idx_} {}
  };

  struct TrieNode {
    LetterObj letter;
    std::vector<std::shared_ptr<TrieNode>> children;

    TrieNode(char c, size_t idx): letter(c, idx) {}
  };

  std::shared_ptr<TrieNode> root;
  size_t rows;
  size_t cols;
  std::unordered_set<std::string> words;


  void recurseBuild(const std::vector<std::vector<char>>& board, size_t row, size_t col, std::shared_ptr<TrieNode> curNode, std::unordered_set<size_t>& seen) {
    // Ensure we haven't seen this cell already.
    std::cout << "evaluating " << row << " " << col << std::endl;
    size_t idx = row * rows + col;
    if (seen.contains(idx)) return;
    seen.insert(idx);

    // Create a new node in the trie for this cell.
    curNode->children.push_back(std::make_shared<TrieNode>(board[row][col], idx));
  
    // Adjust the curNode.
    curNode = *curNode->children.rbegin();

    // Recursively check neighbors.
    for (int i = static_cast<int>(row) - 1; i <= row + 1; i++) {
      for (int j = static_cast<int>(col) - 1; j <= col + 1; j++) {
        // Ensure the neighbour is in range.
        if (i < 0 || i >= rows || j < 0 || j >= cols) continue;

        recurseBuild(board, i, j, curNode, seen);
      }
    }
  }

  public:

  SolverTrie(const std::vector<std::vector<char>> board) : root{std::make_shared<TrieNode>(0, 0)}, rows{board.size()}, cols{board[0].size()} {

    // Parse the words into a hashset.
    std::ifstream file("words_alpha.txt"); // Replace "example.txt" with your file's name
    if (!file) throw std::runtime_error("Error opening the file!");

    std::string line;
    while (std::getline(file, line)) {
      words.insert(line);
      std::cout << line << std::endl; // Process the line (in this case, print it)
    }

    file.close();

    // Construct the trie.
    for (size_t startRow = 0; startRow < rows; startRow++) {
      for (size_t startCol = 0; startCol < cols; startCol++) {
        // board[startRow][startCol] is where we're starting the current exploration from.

        std::unordered_set<size_t> seen;
        recurseBuild(board, startRow, startCol, root, seen);

      }
    }
  }

  // Finds all the words from the trie and the dictionary.
  // std::vector<std::pair<char, size_t>> findWords() {}



};



int main() {
  
  // A 4x4 input board, as a 2d char arr.
  // Must contain lowercase alphabetical chars.
  std::vector<std::vector<char>> board {
    {'f', 'q', 't', 'a'},
    {'a', 'f', 'y', 'e'},
    {'m', 'l', 's', 'o'},
    {'d', 'g', 'x', 'y'}
  };

  // We want to convert this grid into a trie.
  SolverTrie trie(board);

  // Now that we have a trie, lookup all possible strings to see if we have valid words.


  


}

