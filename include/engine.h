#pragma once

#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace vowels {

namespace details {

std::tuple<std::string, std::string> removeVowels(std::string word);

bool isValidDirection(int direction, int i, int j, int gridSize);
} // namespace details

struct Word {
  std::string word;
  std::string squeezed;
  std::string wildCard;
  uint64_t appearance = 0;
};

class Engine {
public:
  Engine(int gridSize = 5, std::string filename = "datas/valid_words.txt");

  uint64_t getWordsToFindLength() const { return m_wordsToFind.size(); };

  const Word &getWord(uint64_t index) const { return m_wordsToFind[index]; }

  void generateGrid();
  void generateWordList();
  void reduceWordList();
  const std::vector<char> &getGrid() const { return m_grid; };
  void showGrid() const;

private:
  std::vector<Word> m_wordsList; //!< List of all valid words used

  int m_gridSize;                      //!< Size of the keyboard grid
  std::vector<char> m_grid;            //!< List of letter to form the keyboard grid
  std::vector<Word> m_wordsToFind; //!< List of index of the word to find in the grid
};

} // namespace vowels
