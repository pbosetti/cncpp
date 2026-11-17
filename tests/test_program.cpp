// Unit tests for the Program class (implementation in src/_program.cpp)

#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <filesystem>
#include <vector>
#include <fstream>

// Compile the AI implementation into this test so it is the one under test,
// not src/program.cpp 
#include "../src/program.cpp"

namespace fs = std::filesystem;

static fs::path write_gcode(std::string const &name, std::string const &text) {
  fs::path p = fs::temp_directory_path() / name;
  std::ofstream(p) << text;
  return p;
}

static size_t count_lines(std::string const &s) {
  return std::count(s.begin(), s.end(), '\n');
}

static std::vector<size_t> numbers(Program &p) {
  std::vector<size_t> v;
  for (Block &b : p)
    v.push_back(b.n());
  return v;
}

static const std::string GCODE = "N10 G00 X100 Y100 Z100 T01\n"
                                 "\n"
                                 "N20 G01 Z50 F1000 S5000 M03\n"
                                 "N30 G01 X150\n";

TEST_CASE("Program constructor", "[program]") {
  Machine m;
  auto file = write_gcode("cncpp_prog_ok.g", GCODE);

  SECTION("empty, then loaded from file") {
    Program p(&m);
    REQUIRE(p.size() == 0);
    p.load(file.string());
    REQUIRE(p.size() == 3);
  }

  SECTION("empty, then built line by line") {
    Program p(&m);
    p << "N10 G00 X10" << "N20 G01 Y10 F500";
    REQUIRE(p.size() == 2);
  }

  SECTION("error in the middle throws, description stops before it") {
    auto bad = write_gcode("cncpp_prog_bad.g", "N10 G00 X10\n"
                                               "N20 G01 X20 F100\n"
                                               "N30 G99 X30\n"
                                               "N40 G01 X40\n");
    Program p(&m);
    REQUIRE_THROWS(p.load(bad.string()));
    REQUIRE(p.size() == 2);
    REQUIRE(count_lines(p.desc(false)) == 2);
  }
}

TEST_CASE("Program::desc", "[program]") {
  Machine m;
  Program p(&m);

  SECTION("empty program") { REQUIRE(p.desc() == ""); }

  p << "N10 G00 X10" << "N20 G01 Y10 F500";

  SECTION("one line per block") {
    std::string d = p.desc(false);
    REQUIRE(count_lines(d) == 2);
    auto it = p.begin();
    REQUIRE(d == it->desc(false) + "\n" + std::next(it)->desc(false) + "\n");
  }

  SECTION("colored or not") {
    REQUIRE(p.desc(true).find("\x1b[") != std::string::npos);
    REQUIRE(p.desc(false).find("\x1b[") == std::string::npos);
    REQUIRE(count_lines(p.desc(true)) == 2);
  }
}

TEST_CASE("Program::load", "[program]") {
  Machine m;
  Program p(&m);
  auto file = write_gcode("cncpp_prog_ok.g", GCODE);

  SECTION("blocks in file order") {
    p.load(file.string());
    REQUIRE(numbers(p) == std::vector<size_t>{10, 20, 30});
  }

  SECTION("append a second program with increasing numbers") {
    auto more = write_gcode("cncpp_prog_more.g", "N40 G01 Y150\nN50 G00 Z100\n");
    p.load(file.string());
    p.load(more.string(), true);
    REQUIRE(numbers(p) == std::vector<size_t>{10, 20, 30, 40, 50});
  }

  SECTION("reload without append replaces the program") {
    p.load(file.string());
    p.load(file.string());
    REQUIRE(p.size() == 3);
  }

  SECTION("missing file throws") {
    REQUIRE_THROWS_AS(p.load("/nonexistent/cncpp.g"), std::runtime_error);
  }
}

TEST_CASE("Program::operator<<", "[program]") {
  Machine m;
  Program p(&m);

  SECTION("add to an empty program") {
    p << "N10 G00 X10";
    REQUIRE(p.size() == 1);
    REQUIRE_NOTHROW(p.desc());
  }

  SECTION("add to a non-empty program") {
    p << "N10 G00 X10";
    p << "N20 G01 Y10 F500";
    REQUIRE(p.size() == 2);
    REQUIRE(count_lines(p.desc(false)) == 2);
    REQUIRE(std::next(p.begin())->desc(false).find("[ 20]") == 0);
  }
}

TEST_CASE("Program::insert", "[program]") {
  Machine m;
  Program p(&m);
  p << "N10 G00 X10" << "N20 G01 Y10 F500" << "N30 G01 X20";

  SECTION("at the beginning") {
    Block &b = p.insert(0, "N5 G00 Z10");
    REQUIRE(b.n() == 5);
    REQUIRE(numbers(p) == std::vector<size_t>{5, 10, 20, 30});
    REQUIRE(count_lines(p.desc(false)) == 4);
  }

  SECTION("in the middle") {
    Block &b = p.insert(1, "N15 G01 Z10 F200");
    REQUIRE(b.n() == 15);
    REQUIRE(numbers(p) == std::vector<size_t>{10, 15, 20, 30});
    REQUIRE(count_lines(p.desc(false)) == 4);
  }

  SECTION("a wrong block at the end is discarded and the error thrown") {
    REQUIRE_THROWS_AS(p.insert(3, "N25 G01 X30"), std::runtime_error);
    REQUIRE(numbers(p) == std::vector<size_t>{10, 20, 30});
    REQUIRE(count_lines(p.desc(false)) == 3);
  }

  SECTION("a block breaking the next one is discarded, the rest restored") {
    REQUIRE_THROWS_AS(p.insert(1, "N25 G01 X30"), std::runtime_error);
    REQUIRE(numbers(p) == std::vector<size_t>{10, 20, 30});
    REQUIRE(count_lines(p.desc(false)) == 3);
  }

  SECTION("out of range index") {
    REQUIRE_THROWS_AS(p.insert(4, "N40 G00 X0"), std::out_of_range);
  }
}

TEST_CASE("Program::current", "[program]") {
  Machine m;
  Program p(&m);

  SECTION("empty program") {
    REQUIRE(p.current() == nullptr);
    REQUIRE_THROWS_AS(p.load_next(), std::out_of_range);
    REQUIRE(p.current() == nullptr);
  }

  p << "N10 G00 X10" << "N20 G01 Y10 F500";

  SECTION("not started, running, done, rewound") {
    REQUIRE(p.current() == nullptr);
    REQUIRE(p.load_next().n() == 10);
    REQUIRE(p.current() == &*p.begin());
    REQUIRE(p.load_next().n() == 20);
    REQUIRE_THROWS_AS(p.load_next(), std::out_of_range);
    REQUIRE(p.current() == nullptr);
    REQUIRE(p.done());
    p.rewind();
    REQUIRE(p.current() == nullptr);
    REQUIRE_FALSE(p.done());
  }
}
