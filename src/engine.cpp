#include "engine.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <assert.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

namespace vowels {

constexpr char kWildcard = '*';

namespace details {

std::tuple<std::string, std::string> removeVowels(std::string word) {
  std::string wordSqueezed;
  std::string wordWildCard;
  for (char c : word) {

    for (auto consonnant : "bcdfghjklmnpqrstvwxz") {
      if (c == consonnant) {
        wordSqueezed += consonnant;
        wordWildCard += consonnant;
        break;
      }
      if (consonnant == 'z') {
        wordWildCard += kWildcard;
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

int getMaxWordSize(int direction, int i, int j, int gridSize) {
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

Engine::Engine(int gridSize, std::string filename)
    : m_gridSize(gridSize), m_grid(m_gridSize * m_gridSize, kWildcard) {

  std::ifstream infile(filename);
  if (!infile.is_open()) {
    throw std::runtime_error("File Not Found : " + filename);
  }

  std::string line;
  while (infile >> line) {
    auto [wordSqueezed, wordWildCard] = details::removeVowels(line);
    m_wordsList.push_back({line, wordSqueezed, wordWildCard});
  }

  generateGrid();
}

void Engine::generateGrid() {
  assert(m_wordsList.size() != 0);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<int> directionSpace = {0, 1, 2, 3}; // +i -i +j -j
  std::uniform_int_distribution<uint64_t> distrib(0, m_wordsList.size() - 1);

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
    const int wordMaxSize = details::getMaxWordSize(direction, i, j, m_gridSize);
    std::vector<char> wordConstraint;
    for (int t = 0, pos = n; t < wordMaxSize; ++t, pos += directionOffset) {
      wordConstraint.push_back(m_grid[pos]);
    }

    auto constraint = [&](const Word &word) {
      if (word.squeezed.size() > wordMaxSize) {
        return false;
      }

      for (int u = 0; u < word.squeezed.size(); ++u) {
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
      this->showGrid();
      throw std::runtime_error("No valid word. I need to implement retry of the process");
    }

    // fill the grid with the word selected
    for (int t = 0, pos = n; t < foundWord->squeezed.size(); ++t, pos += directionOffset) {
      m_grid[pos] = foundWord->squeezed[t];
    }
    m_wordsToFind.push_back(*foundWord);
  }
}

void Engine::showGrid() const {

  for (int i = 0; i < m_gridSize; ++i) {
    for (int j = 0; j < m_gridSize; ++j) {
      std::cout << m_grid[i + j * m_gridSize] << " ";
    }
    std::cout << std::endl;
  }
}

} // namespace vowels