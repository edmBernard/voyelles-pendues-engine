#pragma once

#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace vowels {

namespace details {

std::tuple<std::string, std::string> removeVowels(std::string word);

}

class Engine {
public:
  Engine(int gridSize = 5, std::string filename = "datas/valid_word.txt");

  uint64_t getWordsToFindLength() const { return m_wordsToFind.size(); };

  const std::string &getWordComplete(uint64_t index) const { return m_wordsList[m_wordsToFind[index]]; }
  const std::string &getWordSqueezed(uint64_t index) const { return m_wordsListSqueezed[m_wordsToFind[index]]; }
  const std::string &getWordWildcard(uint64_t index) const { return m_wordsListWithWildcard[m_wordsToFind[index]]; }

  void newPuzzle();
  void createGrid();
  const std::vector<std::string> &getGrid() const { return m_grid; };

private:
  std::vector<std::string> m_wordsList;             //!< List of all valid words used
  std::vector<std::string> m_wordsListSqueezed;     //!< List of all words without vowels
  std::vector<std::string> m_wordsListWithWildcard; //!< List of all words with vowels replaced by *

  std::vector<std::string> m_grid;     //!< List of letter to form the keyboard grid
  int m_gridSize;                      //!< Size of the keyboard grid
  std::vector<uint64_t> m_wordsToFind; //!< List of index of the word to find in the grid
};

} // namespace vowels
