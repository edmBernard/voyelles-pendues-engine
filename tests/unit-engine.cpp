#include "catch.hpp"

#include "engine.h"
#include <filesystem>
#include <iostream>

#include "utf8cpp/utf8.h"

namespace fs = std::filesystem;

using namespace vowels;


TEST_CASE("Remove vowels", "[utils]") {

  std::string testWord = "azertyuiopqsdfghjklmwxcvbnùéàât";
  std::string expectedSqueezed = "zrtpqsdfghjklmwxcvbnt";
  std::string expectedWildcard = "*z*rt****pqsdfghjklmwxcvbn****t";

  uint64_t size = utf8::distance(testWord.begin(), testWord.end());
  CHECK(size == expectedWildcard.size());

  auto [squeezed, wildcard] = details::removeVowels(testWord);
  CHECK(squeezed == expectedSqueezed);
  CHECK(wildcard == expectedWildcard);

}

TEST_CASE("Grid generation", "[engine]") {

  Engine engine(5, "datas/valid_words.txt");

  engine.showGrid();

}

TEST_CASE("Search", "[engine]") {

  Engine engine(5, "datas/valid_words.txt");

  Word firstWord = engine.getWord(0);

  CHECK(engine.search(firstWord.word) == vowels::SearchReturnCode::kWordInList);
  CHECK(engine.search("WordNotInList") == vowels::SearchReturnCode::kWordDontExist);
}