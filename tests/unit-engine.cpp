#include "catch.hpp"

#include "engine.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

using namespace vowels;

TEST_CASE("Engine Contructor", "[engine]") {

  Engine engine(5, "tests/datas/test_valid_words.txt");

  CHECK(engine.getWordsToFindLength() == 25);
  CHECK(engine.getWord(2).word == "abaissables");
  CHECK(engine.getWord(2).squeezed == "bssbls");
  CHECK(engine.getWord(2).wildCard == "*b**ss*bl*s");
}

TEST_CASE("Grid generation", "[engine]") {

  Engine engine(5, "datas/valid_words.txt");

  engine.generateGrid();

  engine.showGrid();

}