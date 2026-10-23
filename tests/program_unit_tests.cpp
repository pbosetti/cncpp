/*
Program Unit Tests

Covers program construction, file loading, block insertion and traversal.
*/

#include "program.hpp"
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace {

bool require(bool condition, const std::string &msg) {
  if (!condition) {
    std::cerr << "[FAIL] " << msg << '\n';
    return false;
  }
  return true;
}

int test_append_and_traverse() {
  int failures = 0;
  cncpp::Machine m;
  cncpp::Program p(&m);
  p << "N10 G00 X0 Y0 Z0" << "N20 G01 X10 F1000" << "X20";
  failures += !require(p.size() == 3, "operator<< appends three blocks");

  size_t count = 0, last_n = 0;
  while (true) {
    p.load_next();
    if (p.done())
      break;
    last_n = (*p.current()).n();
    count++;
  }
  failures += !require(count == 3, "load_next visits every block once");
  failures += !require(last_n == 21, "block number auto-increments");

  p.load_next();
  failures += !require(p.done(), "load_next after the end stays done");

  p.rewind();
  p.load_next();
  failures += !require(!p.done() && (*p.current()).n() == 10,
                       "rewind restarts from the first block");
  return failures;
}

int test_load_skips_blank_and_comment_lines() {
  int failures = 0;
  auto path = std::filesystem::temp_directory_path() / "cncpp_program_test.g";
  {
    std::ofstream f(path);
    f << "N10 G00 X0 Y0 Z0\n\n   \n; a comment\n  # another\r\n"
      << "N20 G01 X10 F1000 ; trailing comment\n";
  }
  cncpp::Machine m;
  cncpp::Program p(path.string(), &m);
  failures += !require(p.size() == 2, "blank and comment lines are skipped");
  std::filesystem::remove(path);
  return failures;
}

// blocks point at each other, so a copied program would point into the
// original one
static_assert(!std::is_copy_constructible_v<cncpp::Program>,
              "Program must not be copyable");

bool near(double a, double b) { return std::fabs(a - b) < 1e-9; }

int test_insert() {
  int failures = 0;
  cncpp::Machine m;
  cncpp::Program p(&m);
  p << "N10 G00 X0 Y0 Z0" << "N20 G01 X10 F1000" << "N30 X20";

  // insert before N20: N20 now starts from the inserted target
  cncpp::Block &b = p.insert(1, "N15 G01 Y10 F500");
  failures += !require(p.size() == 4, "insert adds one block");
  failures += !require(b.prev() && b.prev()->n() == 10 && b.next() &&
                           b.next()->n() == 20,
                       "inserted block is linked to its neighbors");
  cncpp::Block &n20 = *b.next();
  failures += !require(n20.prev() == &b, "next block points back to it");
  failures += !require(near(n20.start().y(), 10.0) && near(n20.target().y(), 10.0),
                       "next block starts from the inserted target (modal Y)");
  failures += !require(near(n20.interpolate(0.0).y(), 10.0),
                       "next block interpolation starts from the inserted target");

  // insert before N30 a block that changes the modal feedrate
  p.insert(3, "N25 F300");
  cncpp::Block &n30 = *std::prev(p.end());
  failures += !require(near(n30.feedrate(), 300.0),
                       "modal feedrate propagates to the following blocks");

  // traversal follows the links, in program order
  std::string order;
  for (p.rewind(); p.load_next();)
    order += std::to_string(p.current()->n()) + " ";
  failures += !require(order == "10 15 20 25 30 ",
                       "traversal visits blocks in order, got: " + order);
  return failures;
}

int test_insert_rollback() {
  int failures = 0;
  cncpp::Machine m;
  cncpp::Program p(&m);
  p << "N10 G00 X0 Y0 Z0" << "N20 G01 X10 F1000";

  bool threw = false;
  try {
    p.insert(1, "N25 G01 Y10"); // N20 would no longer be increasing
  } catch (const std::runtime_error &) {
    threw = true;
  }
  failures += !require(threw, "insert with non-increasing N throws");
  failures += !require(p.size() == 2, "failed insert leaves the size unchanged");
  cncpp::Block &first = *p.begin();
  failures += !require(first.next() && first.next()->n() == 20 &&
                           first.next()->prev() == &first,
                       "failed insert restores the links");
  failures += !require(near(first.next()->start().y(), 0.0),
                       "failed insert restores the next block geometry");

  threw = false;
  try {
    p.insert(5, "N30 X20");
  } catch (const std::out_of_range &) {
    threw = true;
  }
  failures += !require(threw, "insert past the end throws out_of_range");
  return failures;
}

} // namespace

int main() {
  int failures = 0;
  failures += test_append_and_traverse();
  failures += test_load_skips_blank_and_comment_lines();
  failures += test_insert();
  failures += test_insert_rollback();

  if (failures == 0) {
    std::cout << "All Program tests passed.\n";
  } else {
    std::cout << failures << " Program test assertion(s) failed.\n";
  }
  return failures;
}
