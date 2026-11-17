// Unit tests for the Machine class (src/machine.hpp)

#include "machine.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <filesystem>
#include <fstream>

using namespace cncpp;
using Catch::Approx;
using json = nlohmann::json;
namespace fs = std::filesystem;

static fs::path write_file(std::string const &name, std::string const &text) {
  fs::path p = fs::temp_directory_path() / name;
  std::ofstream(p) << text;
  return p;
}

static void require_point(Point const &p, data_t x, data_t y, data_t z) {
  REQUIRE(p.x() == Approx(x));
  REQUIRE(p.y() == Approx(y));
  REQUIRE(p.z() == Approx(z));
}

static void require_defaults(Machine const &m) {
  REQUIRE(m.A() == Approx(5.0));
  REQUIRE(m.fmax() == Approx(10000.0));
  REQUIRE(m.max_error() == Approx(0.005));
  REQUIRE(m.tq() == Approx(0.001));
  require_point(m.zero(), 0, 0, 0);
  require_point(m.offset(), 0, 0, 0);
}

TEST_CASE("Machine default constructor", "[machine]") {
  Machine m;
  require_defaults(m);
  REQUIRE(m.data().is_null());
}

TEST_CASE("Machine load(json)", "[machine]") {
  Machine m;

  SECTION("valid values are stored") {
    json j = {{"A", 40.0},           {"f_max", 5000},
              {"max_error", 0.01},   {"tq", 0.005},
              {"zero", {10, 15, 0}}, {"offset", {1.5, 2.5, 3.5}}};
    m.load(j);
    REQUIRE(m.A() == Approx(40.0));
    REQUIRE(m.fmax() == Approx(5000.0));
    REQUIRE(m.max_error() == Approx(0.01));
    REQUIRE(m.tq() == Approx(0.005));
    require_point(m.zero(), 10, 15, 0);
    require_point(m.offset(), 1.5, 2.5, 3.5);
    REQUIRE(m.data() == j);
  }

  SECTION("missing values keep defaults") {
    m.load(json::object());
    require_defaults(m);
  }

  SECTION("partial JSON sets only given fields") {
    m.load(json{{"A", 12.0}});
    REQUIRE(m.A() == Approx(12.0));
    REQUIRE(m.tq() == Approx(0.001));
    require_point(m.zero(), 0, 0, 0);
  }

  SECTION("invalid types throw") {
    REQUIRE_THROWS(m.load(json{{"A", "fast"}}));
    REQUIRE_THROWS(m.load(json{{"f_max", true}}));
    REQUIRE_THROWS(m.load(json{{"max_error", json::array()}}));
    REQUIRE_THROWS(m.load(json{{"tq", nullptr}}));
    REQUIRE_THROWS(m.load(json{{"zero", 1.0}}));
    REQUIRE_THROWS(m.load(json{{"zero", {1, 2}}}));
    REQUIRE_THROWS(m.load(json{{"offset", {1, 2, 3, 4}}}));
    REQUIRE_THROWS(m.load(json{{"offset", {1, "a", 3}}}));
  }

  SECTION("JSON constructor is equivalent to load") {
    Machine m2(json{{"A", 7.0}, {"zero", {1, 2, 3}}});
    REQUIRE(m2.A() == Approx(7.0));
    require_point(m2.zero(), 1, 2, 3);
  }
}

TEST_CASE("Machine load(toml)", "[machine]") {
  Machine m;
  toml::table t = toml::parse("A = 40.0\ntq = 0.005\nzero = [10, 15, 0]\n");
  m.load(t);
  REQUIRE(m.A() == Approx(40.0));
  REQUIRE(m.tq() == Approx(0.005));
  require_point(m.zero(), 10, 15, 0);
  REQUIRE(m.fmax() == Approx(10000.0));
  REQUIRE(m.data()["A"] == 40.0);
}

TEST_CASE("Machine load(filename)", "[machine]") {
  Machine m;

  SECTION("valid TOML file with [cnc] table") {
    auto f = write_file("cncpp_machine_ok.toml",
                        "[cnc]\nA = 40.0\ntq = 0.005\nzero = [10, 15, 0]\n");
    m.load(f.string());
    REQUIRE(m.A() == Approx(40.0));
    REQUIRE(m.tq() == Approx(0.005));
    require_point(m.zero(), 10, 15, 0);
  }

  SECTION("valid JSON file") {
    auto f = write_file("cncpp_machine_ok.json",
                        R"({"A": 20, "f_max": 3000, "offset": [1, 2, 3]})");
    m.load(f.string());
    REQUIRE(m.A() == Approx(20.0));
    REQUIRE(m.fmax() == Approx(3000.0));
    require_point(m.offset(), 1, 2, 3);
  }

  SECTION("filename constructor is equivalent to load") {
    auto f = write_file("cncpp_machine_ctor.json", R"({"tq": 0.002})");
    Machine m2(f.string());
    REQUIRE(m2.tq() == Approx(0.002));
  }

  SECTION("TOML without [cnc] section throws") {
    auto f = write_file("cncpp_machine_nocnc.toml", "[other]\nA = 1.0\n");
    REQUIRE_THROWS(m.load(f.string()));
  }

  SECTION("TOML with non-table [cnc] throws") {
    auto f = write_file("cncpp_machine_cncval.toml", "cnc = 3\n");
    REQUIRE_THROWS(m.load(f.string()));
  }

  SECTION("malformed files throw") {
    auto fj = write_file("cncpp_machine_bad.json", "{ \"A\": ");
    auto ft = write_file("cncpp_machine_bad.toml", "[cnc\nA = \n");
    REQUIRE_THROWS(m.load(fj.string()));
    REQUIRE_THROWS(m.load(ft.string()));
  }

  SECTION("missing file, directory, or wrong extension throw") {
    REQUIRE_THROWS(m.load(std::string("/nonexistent/cncpp_machine.json")));
    REQUIRE_THROWS(m.load(fs::temp_directory_path().string()));
    auto f = write_file("cncpp_machine.txt", "{}");
    REQUIRE_THROWS(m.load(f.string()));
  }
}

TEST_CASE("Machine quantize", "[machine]") {
  Machine m;
  m.tq(0.01);
  data_t dq = -1;

  data_t q = m.quantize(0.123, dq);
  REQUIRE(q == Approx(0.13));
  REQUIRE(dq == Approx(0.007));
  REQUIRE(q - dq == Approx(0.123));
  // result is always a multiple of tq and not smaller than t
  REQUIRE(q >= 0.123);
  REQUIRE(std::fmod(q, m.tq()) == Approx(0).margin(1e-9));
}

TEST_CASE("Machine setters", "[machine]") {
  Machine m;
  REQUIRE(m.A(10) == Approx(10));
  REQUIRE(m.fmax(2000) == Approx(2000));
  REQUIRE(m.max_error(0.1) == Approx(0.1));
  REQUIRE(m.tq(0.002) == Approx(0.002));
  require_point(m.offset(Point{1, 2, 3}), 1, 2, 3);
  require_point(m.zero(Point{4, 5, 6}), 4, 5, 6);
  require_point(m.zero(7, 8, 9), 7, 8, 9);
  REQUIRE(m.A() == Approx(10));
  require_point(m.zero(), 7, 8, 9);
}

TEST_CASE("Machine desc", "[machine]") {
  Machine m;
  m.A(42);
  auto s = m.desc(false);
  REQUIRE(s.find("A = 42") != std::string::npos);
  REQUIRE(s.find("tq = 0.001") != std::string::npos);
}
