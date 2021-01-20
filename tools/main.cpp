
#include "engine.h"
#include <iostream>

int main(int argc, char *argv[]) try {

  using namespace vowels;

  Engine engine;

  for (int i = 0; i < engine.getWordsToFindLength(); ++i) {
    std::cout << "words: " << engine.getWordComplete(i) << " " << engine.getWordSqueezed(i) << " " << engine.getWordWildcard(i) << std::endl;
  }
  return EXIT_SUCCESS;

} catch (const std::exception &e) {
  std::cerr << "Exception: " << e.what() << std::endl;
}
