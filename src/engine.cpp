#include "engine.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <assert.h>

namespace vowels {

namespace details {

std::tuple<std::string, std::string> removeVowels(std::string word) {
  std::string wordSqueezd;
  std::string wordWildCard;
  for (char c : word) {

    for (auto consonnant : u8"bcdfghjklmpqrstvwxz") {
      if (c == consonnant) {
        wordSqueezd += consonnant;
        wordWildCard += consonnant;
        break;
      }
      if (consonnant == 'z') {
        wordWildCard += "*";
      }
    }
  }
  return {wordSqueezd, wordWildCard};
}
} // namespace details

Engine::Engine(int gridSize, std::string filename) : m_gridSize(gridSize) {
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    throw std::runtime_error("File Not Found : " + filename);
  }

  std::string line;
  while (infile >> line) {
    m_wordsList.push_back(line);
    auto [wordSqueezd, wordWildCard] = details::removeVowels(line);
    m_wordsListSqueezed.push_back(wordSqueezd);
    m_wordsListWithWildcard.push_back(wordWildCard);
  }

  generateGrid();
}

void Engine::generateGrid() {
  assert(m_wordsList.size() != 0);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint64_t> distrib(0, m_wordsListSqueezed.size() - 1);

  for (int n = 0; n < m_gridSize * m_gridSize; ++n) {
    const uint64_t index = distrib(gen);
    m_grid.push_back(m_wordsListSqueezed[index][0]);
    m_wordsToFind.push_back(index);
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