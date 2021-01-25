#include "catch.hpp"

#include "engine.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

using namespace vowels;

TEST_CASE("Remove vowels", "[utils]") {

  std::string testWord = "azertyuiopqsdfghjklmwxcvbnùéàât";
  std::string expectedSqueezed = "zrtpqsdfghjklmwxcvbnt";
  std::string expectedWildcard = "*z*rt****pqsdfghjklmwxcvbn****t";

  auto [squeezed, wildcard] = vowels::details::removeVowels(testWord);
  CHECK(squeezed == expectedSqueezed);
  CHECK(squeezed == expectedWildcard);

}

TEST_CASE("Grid generation", "[engine]") {

  Engine engine(5, "datas/valid_words.txt");

  engine.showGrid();

}