#include "engine.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace vowels {

namespace details {

std::tuple<std::string, std::string> removeVowels(std::string word) {
  std::string wordVowels;
  std::string wordWildCard;
  for (char c : word) {

    for (char consonnant : "bcdfghjklmpqrstvwxz") {
      if (c == consonnant) {
        wordVowels += consonnant;
        wordWildCard += consonnant;
        break;
      }
      if (consonnant == 'z') {
        wordWildCard += "*";
      }
    }
  }
  return {wordVowels, wordWildCard};
}
} // namespace details

Engine::Engine(std::string filename) {
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    throw std::runtime_error("File Not Found : " + filename);
  }

  std::string line;
  while (infile >> line) {
    m_wordList.push_back(line);
    auto [wordVowels, wordWildCard] = details::removeVowels(line);
    m_wordListWithoutVowels.push_back(wordVowels);
    m_wordListWithWildcard.push_back(wordWildCard);
  }

  newPuzzle();
};

Engine::Engine(std::vector<std::string> &&wordList)
    : m_wordList(wordList) {
  newPuzzle();
};

void Engine::newPuzzle() {
}

} // namespace vowels