#include "engine.h"

// On windows mvcc don't define correctly __cplusplus
#define UTF_CPP_CPLUSPLUS 201103L

#include <spdlog/spdlog.h>
#include <utf8cpp/utf8.h>

#include <algorithm>
#include <assert.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

namespace vowels {

constexpr char kWildcard = '*';
constexpr uint8_t kMinWordLength = 3;
constexpr uint8_t kMaxWordLength = 5;

namespace details {

std::tuple<std::string, std::string> removeVowels(std::string_view word) {
  std::string wordSqueezed;
  std::string wordWildCard;

  for (auto it_i = word.begin(); it_i != word.end(); utf8::next(it_i, word.end())) {
    char32_t cp = utf8::peek_next(it_i, word.end());
    for (char consonnant : "bcdfghjklmnpqrstvwxz") {
      if (*it_i == consonnant) {
        wordSqueezed += consonnant;
        wordWildCard += kWildcard;
        break;
      }
      if (consonnant == 'z') {
        utf8::append(cp, wordWildCard);
      }
    }
  }
  return {wordSqueezed, wordWildCard};
}

bool isValidDirection(int direction, int i, int j, int gridSize) {
  switch (direction) {
  case 0: // +i
    return i < gridSize - 2;
  case 1: // -i
    return i > 1;
  case 2: // +j
    return j < gridSize - 2;
  case 3: // -j
    return j > 1;
  default:
    throw std::runtime_error("Unknown direction");
  }
}

int getDirectionOffset(int direction, int gridSize) {
  switch (direction) {
  case 0: // +i
    return 1;
  case 1: // -i
    return -1;
  case 2: // +j
    return gridSize;
  case 3: // -j
    return -gridSize;
  default:
    throw std::runtime_error("Unknown direction");
  }
}

uint64_t getMaxWordSize(int direction, int i, int j, int gridSize) {
  switch (direction) {
  case 0: // +i
    return gridSize - i;
  case 1: // -i
    return i + 1;
  case 2: // +j
    return gridSize - j;
  case 3: // -j
    return j + 1;
  default:
    throw std::runtime_error("Unknown direction");
  }
}

} // namespace details

Engine::Engine(int gridSize, int wordsPerPuzzle, std::string filename)
    : m_gridSize(gridSize), m_wordsPerPuzzle(wordsPerPuzzle), m_grid(gridSize * gridSize, kWildcard), m_bloomGrid(gridSize * gridSize, 0) {

  std::ifstream infile(filename);
  if (!infile.is_open()) {
    throw std::runtime_error("File Not Found : " + filename);
  }

  std::string line;
  while (infile >> line) {
    auto [wordSqueezed, wordWildCard] = details::removeVowels(line);
    if (wordSqueezed.size() >= kMinWordLength && wordSqueezed.size() <= kMaxWordLength) {
      m_wordsList.push_back({line, wordSqueezed, wordWildCard});
    }
  }

  generateNewPuzzle();
}

Engine::Engine(int gridSize, int wordsPerPuzzle, const std::vector<std::string> &wordList)
    : m_gridSize(gridSize), m_wordsPerPuzzle(wordsPerPuzzle), m_grid(gridSize * gridSize, kWildcard), m_bloomGrid(gridSize * gridSize, 0) {

  for (const std::string &line : wordList) {
    auto [wordSqueezed, wordWildCard] = details::removeVowels(line);
    auto end_it = utf8::find_invalid(wordWildCard.begin(), wordWildCard.end());
    if (end_it != wordWildCard.end()) {
      throw std::runtime_error("Invalid UTF8 character in wordList");
    }
    if (wordSqueezed.size() >= kMinWordLength && wordSqueezed.size() <= kMaxWordLength) {
      m_wordsList.push_back({line, wordSqueezed, wordWildCard});
    }
  }

  generateNewPuzzle();
}

void Engine::generateNewPuzzle() {
  reset();
  while (!generateGrid()) {
  }
  generateWordList();
  reduceWordList();
  generateBloomGrid();
}

bool Engine::generateGrid() {
  spdlog::info("Generate Grid");
  assert(m_wordsList.size() != 0);

  reset();

  std::random_device rd;
  std::mt19937 gen(rd());

  std::shuffle(m_wordsList.begin(), m_wordsList.end(), gen);

  std::vector<int> directionSpace = {0, 1, 2, 3}; // +i -i +j -j

  for (int n = 0; n < m_gridSize * m_gridSize; ++n) {
    const int i = n % m_gridSize;
    const int j = n / m_gridSize;

    // draw direction
    std::shuffle(directionSpace.begin(), directionSpace.end(), gen);
    int direction = -1;
    for (int d : directionSpace) {
      if (details::isValidDirection(d, i, j, m_gridSize)) {
        direction = d;
        break;
      }
    }
    if (direction == -1) {
      spdlog::debug("No valid direction found should not happen on 5x5 grid");
      continue;
    }

    // filter word list with constraint (letter already filled, size available)
    const int directionOffset = details::getDirectionOffset(direction, m_gridSize);
    const uint64_t wordMaxSize = details::getMaxWordSize(direction, i, j, m_gridSize);
    std::vector<char> wordConstraint;
    for (uint64_t t = 0, pos = n; t < wordMaxSize; ++t, pos += directionOffset) {
      wordConstraint.push_back(m_grid[pos]);
    }

    auto constraint = [&](const Word &word) {
      if (word.squeezed.size() > wordMaxSize) {
        return false;
      }
      if (word.used) {
        return false;
      }

      for (uint64_t u = 0; u < word.squeezed.size(); ++u) {
        if (wordConstraint[u] != kWildcard && wordConstraint[u] != word.squeezed[u]) {
          return false;
        }
      }
      return true;
    };

    auto foundWord = std::find_if(m_wordsList.begin(), m_wordsList.end(), constraint);
    if (foundWord == m_wordsList.end()) {
      if (m_grid[n] != kWildcard) {
        continue;
      }
      spdlog::info("No valid word found.");
      return false;
    }
    foundWord->used = true;

    // fill the grid with the word selected
    std::vector<uint64_t> letterPosition;
    for (uint64_t t = 0, pos = n; t < foundWord->squeezed.size(); ++t, pos += directionOffset) {
      m_grid[pos] = foundWord->squeezed[t];
      letterPosition.push_back(pos);
    }
    foundWord->letterPosition = letterPosition;
    m_wordsToFind.push_back(*foundWord);
  }
  m_wordsUsedToBuildGrid = m_wordsToFind.size();
  spdlog::debug("{} : word to find {}", __FUNCTION__, m_wordsToFind.size());
  return true;
}

void Engine::generateWordList() {
  spdlog::info("Generate words list");
  assert(m_wordsList.size() != 0);
  assert(m_grid.size() != 0);

  // Loop on grid position
  for (int n = 0; n < m_gridSize * m_gridSize; ++n) {
    const int i = n % m_gridSize;
    const int j = n / m_gridSize;

    std::vector<int> directionSpace = {0, 1, 2, 3}; // +i -i +j -j
    // Loop on direction
    for (int direction : directionSpace) {
      if (!details::isValidDirection(direction, i, j, m_gridSize)) {
        continue;
      }

      // Loop on wordList : find_if update iterator
      auto wordListBegin = m_wordsList.begin();
      while (true) {
        const int directionOffset = details::getDirectionOffset(direction, m_gridSize);
        const uint64_t wordMaxSize = details::getMaxWordSize(direction, i, j, m_gridSize);

        // Create word consonnant from grid
        std::vector<char> wordConstraint;
        std::vector<uint64_t> letterPosition;
        for (uint64_t t = 0, pos = n; t < wordMaxSize; ++t, pos += directionOffset) {
          wordConstraint.push_back(m_grid[pos]);
          letterPosition.push_back(pos);
        }

        auto constraint = [&](const Word &word) {
          if (word.squeezed.size() > wordMaxSize) {
            return false;
          }
          if (word.used) {
            return false;
          }
          for (uint64_t u = 0; u < word.squeezed.size(); ++u) {
            if (wordConstraint[u] != word.squeezed[u]) {
              return false;
            }
          }
          return true;
        };

        auto foundWord = std::find_if(wordListBegin, m_wordsList.end(), constraint);
        if (foundWord == m_wordsList.end()) {
          break;
        }

        foundWord->used = true;
        letterPosition.resize(foundWord->squeezed.size()); // letter position can be bigger than the find word
        foundWord->letterPosition = letterPosition;
        m_wordsToFind.push_back(*foundWord);
        wordListBegin = ++foundWord;
      }
    }
  }
}

void Engine::generateBloomGrid() {
  spdlog::info("Generate Bloom grid");
  assert(m_wordsToFind.size() != 0);
  std::fill(m_bloomGrid.begin(), m_bloomGrid.end(), 0);

  for (auto &w : m_wordsToFind) {
    for (uint64_t pos : w.letterPosition) {
      m_bloomGrid[pos] += 1;
    }
  }
}

void Engine::reduceWordList() {
  spdlog::info("Reduce words list");
  assert(m_wordsToFind.size() != 0);

  std::random_device rd;
  std::mt19937 gen(rd());

  if (m_wordsToFind.size() < m_wordsPerPuzzle) {
    return;
  }

  // We keep the head of the list unchanged they are words used to create the grid
  std::shuffle(m_wordsToFind.begin() + m_wordsUsedToBuildGrid, m_wordsToFind.end(), gen);
  m_wordsToFind.resize(m_wordsPerPuzzle);
  std::shuffle(m_wordsToFind.begin(), m_wordsToFind.end(), gen);
}

SearchReturnCode Engine::search(std::string_view queryWord) {
  {
    // search in word to find
    auto result = std::find_if(m_wordsToFind.begin(), m_wordsToFind.end(), [&](Word word) {
      return word.word == queryWord;
    });
    if (result != m_wordsToFind.end()) {
      removeFromBloom(*result);
      m_wordsToFind.erase(result);
      return SearchReturnCode::kWordInList;
    }
  }

  {
    // search in word list
    auto result = std::find_if(m_wordsList.begin(), m_wordsList.end(), [&](Word word) {
      return word.word == queryWord;
    });
    if (result != m_wordsList.end()) {
      return SearchReturnCode::kWordExist;
    }
  }

  return SearchReturnCode::kWordDontExist;
}

void Engine::showGrid() const {

  for (int j = 0; j < m_gridSize; ++j) {
    for (int i = 0; i < m_gridSize; ++i) {
      std::cout << m_grid[i + j * m_gridSize] << " ";
    }
    std::cout << std::endl;
  }
}

void Engine::showBloomGrid() const {

  for (int j = 0; j < m_gridSize; ++j) {
    for (int i = 0; i < m_gridSize; ++i) {
      std::cout << m_bloomGrid[i + j * m_gridSize] << " ";
    }
    std::cout << std::endl;
  }
}

void Engine::reset() {

  for (auto &word : m_wordsList) {
    word.used = false;
    word.letterPosition.clear();
  }

  std::fill(m_grid.begin(), m_grid.end(), kWildcard);
  std::fill(m_bloomGrid.begin(), m_bloomGrid.end(), 0);
  m_wordsToFind.clear();
}

void Engine::removeFromBloom(const Word &word) {
  for (uint64_t i : word.letterPosition) {
    m_bloomGrid[i] -= 1;
    if (m_bloomGrid[i] < 0) {
      throw std::runtime_error("Negative value in bloom grid. something goes wrong.");
    }
  }
}

} // namespace vowels
