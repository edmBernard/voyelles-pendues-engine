#include "catch.hpp"

#include "engine.h"
#include <filesystem>

namespace fs = std::filesystem;

using namespace vowels;

TEST_CASE("Engine Contructor", "[engine]") {

  Engine engine(5, "test_valid_words.txt");

  CHECK(engine.getWordsToFindLength() == 25);
  CHECK(engine.getWordSqueezed(2) == "bssbls");
  CHECK(engine.getWordComplete(2) == "abaissables");
  CHECK(engine.getWordWildcard(2) == "*b**ss*bl*s");
}
