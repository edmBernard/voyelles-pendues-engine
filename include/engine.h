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
  Engine(std::vector<std::string> &&wordList);

  void newPuzzle();
  void createGrid();
  const std::vector<std::string> &getGrid() const;

  uint64_t getWordListLength() const { return m_wordList.size(); };

  const std::string &getWordWildcard(uint64_t index) const { return m_wordListWithWildcard[index]; }
  const std::string &getWordComplete(uint64_t index) const { return m_wordList[index]; }

private:
  std::vector<std::string> m_wordList;

  std::vector<uint64_t> m_wordsInternalIndex;
  std::vector<std::string> m_grid;
  std::vector<std::string> m_wordListWithoutVowels;
  std::vector<std::string> m_wordListWithWildcard;
};

} // namespace vowels
