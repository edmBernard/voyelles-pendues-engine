
#include "engine.h"

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"

#include <iostream>

int main(int argc, char *argv[]) try {

  spdlog::cfg::load_env_levels();

  vowels::Engine engine(5);

  spdlog::info("List of word to find : ");
  for (int i = 0; i < engine.getWordsToFindLength(); ++i) {
    spdlog::info(" - {}", engine.getWord(i).word);
  }

  engine.showGrid();
  engine.showBloomGrid();

  // uint64_t puzzleLength = engine.getWordsToFindLength();
  // for (uint64_t n = 0; n < puzzleLength; ++n) {
  //   std::cout << std::endl;
  //   vowels::Word word = engine.getWord(0);

  //   std::cout << "word.word: " << word.word << "  ";
  //   for (auto pos : word.letterPosition) {
  //     std::cout << pos << " ";
  //   }
  //   std::cout << std::endl;

  //   if (engine.search(word.word) == vowels::SearchReturnCode::kWordInList) {
  //     engine.showBloomGrid();
  //   }
  // }

  engine.generateNewPuzzle();
  engine.showGrid();
  engine.showBloomGrid();

  return EXIT_SUCCESS;

} catch (const std::exception &e) {
  spdlog::error("{}", e.what());
}
