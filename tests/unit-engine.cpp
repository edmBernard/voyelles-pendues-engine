#include "catch.hpp"

#include "engine.h"
#include <filesystem>

namespace fs = std::filesystem;

using namespace vowels;

TEST_CASE("Engine Contructor", "[engine]") {

  Engine engine("test_valid_words.txt");

  CHECK(engine.getWordListLength() == 25);
  CHECK(engine.getWordSqueezed(2) == "bssbls");
  CHECK(engine.getWordComplete(2) == "abaissables");
  CHECK(engine.getWordWildcard(2) == "*b**ss*bl*s");
}
