
#include "engine.h"

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"

#include <iostream>

int main(int argc, char *argv[]) try {

  spdlog::cfg::load_env_levels();

  vowels::Engine engine(5);
  spdlog::info("List of word to find of length {}", engine.getWordsToFindLength());
  engine.generateWordList();
  spdlog::info("List of word to find of length {}", engine.getWordsToFindLength());
  engine.reduceWordList();
  spdlog::info("List of word to find of length {}", engine.getWordsToFindLength());

  spdlog::info("List of word to find");
  for (int i = 0; i < engine.getWordsToFindLength(); ++i) {
    spdlog::info("- {}", engine.getWord(i).word);
  }



  engine.showGrid();
  return EXIT_SUCCESS;

} catch (const std::exception &e) {
  spdlog::error("{}", e.what());
}
