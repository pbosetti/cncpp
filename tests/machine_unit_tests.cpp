/*
Machine Unit Tests

Covers configuration loading (JSON, TOML, files) and the offline behavior of
setpoint/position. MADS connection is not tested, since it needs a broker.
*/

#include "machine.hpp"
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

constexpr double kEps = 1e-9;

bool approx_equal(double a, double b) { return std::fabs(a - b) <= kEps; }

bool require(bool condition, const std::string &msg) {
  if (!condition) {
    std::cerr << "[FAIL] " << msg << '\n';
    return false;
  }
  return true;
}

template <class F> bool throws_runtime_error(F &&f) {
  try {
    f();
  } catch (const std::runtime_error &) {
    return true;
  } catch (...) {
    return false;
  }
  return false;
}

int test_json_load() {
  int failures = 0;
  using cncpp::Machine;
  nlohmann::json cfg = {{"A", 7.0},     {"tq", 0.002},          {"fmax", 3000},
                        {"max_error", 0.01}, {"zero", {1, 2, 3}}, {"offset", {0.5, 0, 0}}};
  Machine m(cfg);
  failures += !require(approx_equal(m.A(), 7.0), "json: A");
  failures += !require(approx_equal(m.tq(), 0.002), "json: tq");
  failures += !require(approx_equal(m.fmax(), 3000.0), "json: integer fmax");
  failures += !require(approx_equal(m.max_error(), 0.01), "json: max_error");
  failures += !require(approx_equal(m.zero().z(), 3.0), "json: zero");
  failures += !require(approx_equal(m.offset().x(), 0.5), "json: offset");

  failures += !require(throws_runtime_error([] {
                         nlohmann::json bad = {{"A", "fast"}};
                         Machine m(bad);
                       }),
                       "json: non-numeric A throws");
  failures += !require(throws_runtime_error([] {
                         nlohmann::json bad = {{"zero", {1, 2}}};
                         Machine m(bad);
                       }),
                       "json: zero with two values throws");
  return failures;
}

int test_toml_load() {
  int failures = 0;
  cncpp::Machine m;
  toml::table t = toml::parse("A = 10\ntq = 0.005\nzero = [0, 0, 800]\n");
  bool ok = true;
  try {
    m.load(t);
  } catch (const std::exception &e) {
    std::cerr << "toml load error: " << e.what() << '\n';
    ok = false;
  }
  failures += !require(ok, "toml: integer values are accepted");
  failures += !require(approx_equal(m.A(), 10.0), "toml: A");
  failures += !require(approx_equal(m.tq(), 0.005), "toml: tq");
  failures += !require(approx_equal(m.zero().z(), 800.0), "toml: zero");
  failures += !require(m.data().contains("A"), "toml: data() holds the table");
  return failures;
}

int test_file_load() {
  int failures = 0;
  for (std::string name : {"machine.json", "machine.toml", "mads.ini"}) {
    std::string path = std::string(CNCPP_SOURCE_DIR) + "/" + name;
    bool ok = true;
    try {
      cncpp::Machine m(path);
    } catch (const std::exception &e) {
      std::cerr << name << ": " << e.what() << '\n';
      ok = false;
    }
    failures += !require(ok, "file: repo " + name + " loads");
  }
  failures += !require(throws_runtime_error([] {
                         std::string path = "no_such_file.toml";
                         cncpp::Machine m(path);
                       }),
                       "file: missing file throws");
  return failures;
}

int test_offline_setpoint() {
  int failures = 0;
  cncpp::Machine m;
  m.set_setpoint(cncpp::Point(1, 2, 3));
  failures += !require(m.position().is_complete(),
                       "offline: position follows setpoint");
  failures += !require(m.error() < m.max_error(),
                       "offline: error is below max_error (go to zero ends)");
  return failures;
}

} // namespace

int main() {
  int failures = 0;
  failures += test_json_load();
  failures += test_toml_load();
  failures += test_file_load();
  failures += test_offline_setpoint();

  if (failures == 0) {
    std::cout << "All Machine tests passed.\n";
  } else {
    std::cout << failures << " Machine test assertion(s) failed.\n";
  }
  return failures;
}
