#include "engine.h"
#include "catch.hpp"
#include <filesystem>

namespace fs = std::filesystem;

using namespace vp;

TEST_CASE("Engine Contructor", "[engine]") {

    Engine engine("../tests/datas/test_valid_word.txt");

    CHECK(engine.getWordListLength() == 18);
    CHECK(engine.getWordComplete(3) == "abaissables");
    CHECK(engine.getWordWildcard(3) == "*b**ss*bl*s");

}
