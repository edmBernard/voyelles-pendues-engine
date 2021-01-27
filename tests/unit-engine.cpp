#include "catch.hpp"

#include "engine.h"
#include <filesystem>
#include <iostream>

#include "utf8cpp/utf8.h"

namespace fs = std::filesystem;

using namespace vowels;


TEST_CASE("Remove vowels", "[utils]") {

  std::string testWord = "azertyuiopqsdfghjklmwxcvbnt";
  std::string expectedSqueezed = "zrtpqsdfghjklmwxcvbnt";
  std::string expectedWildcard = "a*e**yuio******************";

  uint64_t size = utf8::distance(testWord.begin(), testWord.end());
  CHECK(size == expectedWildcard.size());

  auto [squeezed, wildcard] = details::removeVowels(testWord);
  CHECK(squeezed == expectedSqueezed);
  CHECK(wildcard == expectedWildcard);

}

TEST_CASE("Grid generation", "[engine]") {

  Engine engine(5, "datas/valid_words.txt");

  engine.generateWordList();
  engine.reduceWordList();
  engine.generateBloomGrid();

  engine.showGrid();
  engine.showBloomGrid();

}

TEST_CASE("Search", "[engine]") {

  Engine engine(5, "datas/valid_words.txt");
  engine.generateWordList();
  engine.reduceWordList();
  engine.generateBloomGrid();

  Word firstWord = engine.getWord(0);
  CHECK(firstWord.squeezed.size() == firstWord.letterPosition.size());

  uint64_t n = engine.getWordsToFindLength();
  CHECK(engine.search(firstWord.word) == vowels::SearchReturnCode::kWordInList);
  CHECK(engine.getWordsToFindLength() == n-1);
  CHECK(engine.search("WordNotInList") == vowels::SearchReturnCode::kWordDontExist);
  CHECK(engine.getWordsToFindLength() == n-1);
}