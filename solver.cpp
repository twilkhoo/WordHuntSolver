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


  void recurseBuild(const std::vector<std::vector<char>>& board, size_t row, size_t col, std::shared_ptr<TrieNode> curNode, std::unordered_set<size_t>& seen, std::string& testStr) {
    // Ensure we haven't seen this cell already.
    // std::cout << "evaluating " << row << " " << col << std::endl;
    size_t idx = row * rows + col;
    if (seen.contains(idx)) return;
    seen.insert(idx);

    // Determine the testStr.
    testStr += board[row][col];
    std::cout << "string is now: " << testStr << std::endl;

    // Create a new node in the trie for this cell.
    curNode->children.push_back(std::make_shared<TrieNode>(board[row][col], idx));
  
    // Adjust the curNode.
    curNode = *curNode->children.rbegin();

    // Recursively check neighbors.
    for (int i = static_cast<int>(row) - 1; i <= static_cast<int>(row) + 1; i++) {
      for (int j = static_cast<int>(col) - 1; j <= static_cast<int>(col) + 1; j++) {
        // std::cout << "next accessing i: " << i << ", j: " << j << " from row: " << row << ", col: " << col << std::endl;
        // Ensure the neighbour is in range.
        if (i < 0 || i >= rows || j < 0 || j >= cols) continue;
        
        // Don't access the current cell again.
        if (i == row && j == col) continue;


        recurseBuild(board, i, j, curNode, seen, testStr);
      }
    }

    testStr.pop_back();
    seen.erase(idx);
  }

  public:

  SolverTrie(const std::vector<std::vector<char>> board) : root{std::make_shared<TrieNode>(0, 0)}, rows{board.size()}, cols{board[0].size()} {

    // Parse the words into a hashset.
    // std::ifstream file("words_alpha.txt"); // Replace "example.txt" with your file's name
    // if (!file) throw std::runtime_error("Error opening the file!");

    // std::string line;
    // while (std::getline(file, line)) {
    //   words.insert(line);
    //   std::cout << line << std::endl; // Process the line (in this case, print it)
    // }

    // file.close();

    // Construct the trie.
    for (size_t startRow = 0; startRow < rows; startRow++) {
      for (size_t startCol = 0; startCol < cols; startCol++) {
        // board[startRow][startCol] is where we're starting the current exploration from.

        std::unordered_set<size_t> seen;
        std::string testStr;
        recurseBuild(board, startRow, startCol, root, seen, testStr);

      }
    }
  }


  void recurseFind(std::vector<std::vector<std::pair<char, size_t>>>& validWords, std::vector<std::pair<char, size_t>>& curWord, std::string& curWordStr, std::shared_ptr<TrieNode> curNode) {
    
    // std::cout << "in recurseFind" << std::endl;


    // If we have the root, just recurse on children.
    if (curNode->letter.c == 0) {

      std::cout << "in root" << std::endl;

      for (auto next : curNode->children) {
        recurseFind(validWords, curWord, curWordStr, next);
      }
      return;
    } 
    
    // Add the current node's letter onto the word.
    auto curChar = curNode->letter.c;
    auto curIdx = curNode->letter.idx;

    curWordStr += curChar;
    curWord.emplace_back(curChar, curIdx);

    std::cout << "curWord: " << curWordStr << std::endl;
    
    // See if the current word is in the dictionary.
    if (words.contains(curWordStr)) {
      std::cout << "Found a word: " << curWordStr << std::endl;
      validWords.push_back(curWord);
    }

    // Recurse on children.
    for (auto next : curNode->children) {
      recurseFind(validWords, curWord, curWordStr, next);
    }

    // Undo the letter addition on the current word.
    curWordStr.pop_back();
    curWord.pop_back();
  }

  // Finds all the words from the trie and the dictionary.
  std::vector<std::vector<std::pair<char, size_t>>> findWords() {
    std::vector<std::vector<std::pair<char, size_t>>> words;
    std::vector<std::pair<char, size_t>> curWord;
    std::shared_ptr<TrieNode> curNode = root;
    std::string curWordStr;

    std::cout << "finding words" << std::endl;

    recurseFind(words, curWord, curWordStr, curNode);
    return words;
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
  SolverTrie trie(board);

  // Now that we have a trie, lookup all possible strings to see if we have valid words.
  // auto foundWords = trie.findWords();

}

