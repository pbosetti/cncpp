/*
Point Unit Tests

Generatd by Copilot after prompt (Claude-sonnet 4.6):
> Make a unit test for the `Point` class. Integrate it in CTest and make sure it runs as part of the test suite. The test should cover all public methods and operators of the `Point` class, including edge cases like incomplete points and operations that should throw exceptions. Use assertions to verify expected behavior and output informative messages for any failed tests.
*/

#include "point.hpp"
#include <cmath>
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

} // namespace

int main() {
  int failures = 0;
  using cncpp::Point;

  // CONSTRUCTOR AND is_complete
  {
    Point p;
    failures += !require(!p.is_complete(), "default ctor: not complete");
  }
  {
    Point p(1.0, 2.0, 3.0);
    failures += !require(p.is_complete(), "full ctor: is_complete");
    failures += !require(approx_equal(p.x(), 1.0), "full ctor: x");
    failures += !require(approx_equal(p.y(), 2.0), "full ctor: y");
    failures += !require(approx_equal(p.z(), 3.0), "full ctor: z");
  }
  {
    Point p(1.0, std::nullopt, 3.0);
    failures += !require(!p.is_complete(), "partial ctor: not complete");
  }

  // RESET
  {
    Point p(1.0, 2.0, 3.0);
    p.reset();
    failures +=
        !require(!p.is_complete(), "reset: not complete after reset");
  }

  // SETTERS AND GETTERS
  {
    Point p;
    p.x(1.0);
    p.y(2.0);
    p.z(3.0);
    failures += !require(p.is_complete(), "setters: complete after all set");
    failures += !require(approx_equal(p.x(), 1.0), "setters: x");
    failures += !require(approx_equal(p.y(), 2.0), "setters: y");
    failures += !require(approx_equal(p.z(), 3.0), "setters: z");
  }
  {
    // getter on unset coordinate throws std::bad_optional_access
    Point p;
    bool threw = false;
    try {
      (void)p.x();
    } catch (std::bad_optional_access const &) {
      threw = true;
    }
    failures +=
        !require(threw, "getter x on empty point: throws bad_optional_access");
  }

  // DESC
  {
    Point p(1.0, 2.0, 3.0);
    failures += !require(!p.desc().empty(), "desc: non-empty on complete point");
    failures +=
        !require(!p.desc(false).empty(), "desc(false): non-empty on complete point");
  }
  {
    Point p; // incomplete
    failures +=
        !require(!p.desc().empty(), "desc: non-empty on incomplete point");
  }

  // OPERATOR =
  {
    Point p1(1.0, 2.0, 3.0);
    Point p2;
    p2 = p1;
    failures += !require(approx_equal(p2.x(), 1.0), "operator=: x");
    failures += !require(approx_equal(p2.y(), 2.0), "operator=: y");
    failures += !require(approx_equal(p2.z(), 3.0), "operator=: z");
  }

  // OPERATOR +
  {
    Point p1(1.0, 2.0, 3.0);
    Point p2(4.0, 5.0, 6.0);
    Point p3 = p1 + p2;
    failures += !require(approx_equal(p3.x(), 5.0), "operator+: x");
    failures += !require(approx_equal(p3.y(), 7.0), "operator+: y");
    failures += !require(approx_equal(p3.z(), 9.0), "operator+: z");
  }
  {
    // operator+ with incomplete point throws
    Point p1(1.0, 2.0, 3.0);
    Point p2;
    bool threw = false;
    try {
      (void)(p1 + p2);
    } catch (std::runtime_error const &) {
      threw = true;
    }
    failures +=
        !require(threw, "operator+ with incomplete: throws runtime_error");
  }

  // DELTA
  {
    Point p1(5.0, 7.0, 9.0);
    Point p2(1.0, 2.0, 3.0);
    Point d = p1.delta(p2);
    failures += !require(approx_equal(d.x(), 4.0), "delta: x");
    failures += !require(approx_equal(d.y(), 5.0), "delta: y");
    failures += !require(approx_equal(d.z(), 6.0), "delta: z");
  }
  {
    // delta with incomplete point throws
    Point p1(1.0, 2.0, 3.0);
    Point p2;
    bool threw = false;
    try {
      p1.delta(p2);
    } catch (std::runtime_error const &) {
      threw = true;
    }
    failures +=
        !require(threw, "delta with incomplete: throws runtime_error");
  }

  // LENGTH
  {
    // 3-4-0 right triangle: hypotenuse = 5
    Point p(3.0, 4.0, 0.0);
    failures += !require(approx_equal(p.length(), 5.0), "length: 3-4-0");
  }
  {
    // Pythagorean quadruple: 1^2 + 2^2 + 2^2 = 9 -> length = 3
    Point p(1.0, 2.0, 2.0);
    failures += !require(approx_equal(p.length(), 3.0), "length: 1-2-2");
  }
  {
    // length on incomplete point throws
    Point p(1.0, std::nullopt, 3.0);
    bool threw = false;
    try {
      p.length();
    } catch (std::runtime_error const &) {
      threw = true;
    }
    failures +=
        !require(threw, "length on incomplete: throws runtime_error");
  }

  // MODAL
  {
    // modal fills in missing coords, leaves existing ones intact
    Point p1(1.0, std::nullopt, std::nullopt);
    Point p2(9.0, 2.0, 3.0);
    p1.modal(p2);
    failures += !require(approx_equal(p1.x(), 1.0), "modal: x not overwritten");
    failures += !require(approx_equal(p1.y(), 2.0), "modal: y filled from other");
    failures += !require(approx_equal(p1.z(), 3.0), "modal: z filled from other");
  }
  {
    // modal does not overwrite already-set coords
    Point p1(1.0, 2.0, 3.0);
    Point p2(9.0, 8.0, 7.0);
    p1.modal(p2);
    failures += !require(approx_equal(p1.x(), 1.0), "modal: x not overwritten when set");
    failures += !require(approx_equal(p1.y(), 2.0), "modal: y not overwritten when set");
    failures += !require(approx_equal(p1.z(), 3.0), "modal: z not overwritten when set");
  }
  {
    // modal does not fill from an unset source coord
    Point p1;
    Point p2(std::nullopt, std::nullopt, std::nullopt);
    p1.modal(p2);
    failures +=
        !require(!p1.is_complete(), "modal: empty source leaves target incomplete");
  }

  if (failures == 0) {
    std::cout << "All tests passed.\n";
  } else {
    std::cout << failures << " test(s) failed.\n";
  }
  return failures;
}

