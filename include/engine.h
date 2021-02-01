#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace vowels {

enum class SearchReturnCode {
  kWordInList,
  kWordExist,
  kWordDontExist
};

namespace details {

std::tuple<std::string, std::string> removeVowels(std::string_view word);

bool isValidDirection(int direction, int i, int j, int gridSize);

} // namespace details

struct Word {
  std::string word;
  std::string squeezed;
  std::string wildCard;
  std::vector<uint64_t> letterPosition= {};
  bool used = 0;
};

// Usage :
// engine.generateGrid();
// engine.generateWordList();
// engine.reduceWordList();
// engine.generateBloomGrid();
class Engine {
public:
  Engine(int gridSize = 5, std::string filename = "datas/valid_words.txt");
  Engine(int gridSize, const std::vector<std::string> &wordList);

  uint64_t getWordsToFindLength() const { return m_wordsToFind.size(); };

  void generateNewPuzzle();

  [[nodiscard]] SearchReturnCode search(std::string_view word);

  const Word &getWord(uint64_t index) const { return m_wordsToFind[index]; }

  //! Generate grid from all word in dictionnary
  [[nodiscard]] bool generateGrid();

  //! List all words we can find with the generate grid
  void generateWordList();

  //! Reduce word list to avoid to have to many word in the grid
  void reduceWordList();

  //! Generate bloom filter with the word to find
  void generateBloomGrid();

  const std::vector<char> &getGrid() const { return m_grid; };
  const std::vector<int> &getBloom() const { return m_bloomGrid; };

  uint64_t getGridSize() const { return m_gridSize; };

  void showGrid() const;
  void showBloomGrid() const;

private:
  void reset();

  //! Remove word in the bloom grid
  void removeFromBloom(const Word &word);

  std::vector<Word> m_wordsList; //!< List of all valid words used

  int m_gridSize;                      //!< Size of the keyboard grid
  std::vector<char> m_grid;            //!< List of letter to form the keyboard grid
  uint64_t m_wordsUsedToBuildGrid = 0; //!< Number of word used to create the grid
  std::vector<int> m_bloomGrid;        //!< Bloom filter like to keep track of used letter in grid
  std::vector<Word> m_wordsToFind;     //!< List of words to find in the grid
};

} // namespace vowels
