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
  Engine(std::string filename = "datas/valid_word.txt");

  uint64_t getWordListLength() const { return m_wordList.size(); };

  const std::string &getWordComplete(uint64_t index) const { return m_wordList[index]; }
  const std::string &getWordSqueezed(uint64_t index) const { return m_wordListSqueezed[index]; }
  const std::string &getWordWildcard(uint64_t index) const { return m_wordListWithWildcard[index]; }

  void newPuzzle();
  void createGrid();
  const std::vector<std::string> &getGrid() const;

private:
  std::vector<std::string> m_wordList;             //!< List of all valid words used
  std::vector<std::string> m_wordListSqueezed;     //!< List of all words without vowels
  std::vector<std::string> m_wordListWithWildcard; //!< List of all words with vowels replaced by *
  std::vector<std::string> m_grid;                 // List of letter to form the keyboard grid

  std::vector<uint64_t> m_wordsInternalIndex;
};

} // namespace vowels
