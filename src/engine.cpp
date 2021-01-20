#include "engine.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace vowels {

namespace details {

std::tuple<std::string, std::string> removeVowels(std::string word) {
  std::string wordSqueezd;
  std::string wordWildCard;
  for (char c : word) {

    for (char consonnant : "bcdfghjklmpqrstvwxz") {
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

Engine::Engine(std::string filename) {
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    throw std::runtime_error("File Not Found : " + filename);
  }

  std::string line;
  while (infile >> line) {
    m_wordList.push_back(line);
    auto [wordSqueezd, wordWildCard] = details::removeVowels(line);
    m_wordListSqueezed.push_back(wordSqueezd);
    m_wordListWithWildcard.push_back(wordWildCard);
  }

  newPuzzle();
};

void Engine::newPuzzle() {
}

} // namespace vowels