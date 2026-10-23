/*
Block Unit Tests

Implements the Block test plan from tests/TODO.md.
*/

#include "block.hpp"
#include "machine.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

constexpr double kEps = 1e-4;
constexpr double kLengthTol = 1e-4;
constexpr double kPi = 3.14159265358979323846;

bool approx_equal(double a, double b, double eps = kEps) {
  return std::fabs(a - b) <= eps;
}

bool require(bool condition, const std::string &msg) {
  if (!condition) {
    std::cerr << "[FAIL] " << msg << '\n';
    return false;
  }
  return true;
}

std::string format_value(double value) {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(6) << value;
  return oss.str();
}

std::string format_time(double time) {
  return ", t=" + format_value(time);
}

bool require_near(double obtained, double expected, const std::string &msg,
                  double tol = kEps) {
  std::ostringstream oss;
  oss << msg << ", expected=" << format_value(expected)
      << ", obtained=" << format_value(obtained)
      << ", tolerance=" << format_value(tol);
  return require(approx_equal(obtained, expected, tol), oss.str());
}

bool require_near_at_time(double obtained, double expected, double time,
                          const std::string &msg, double tol = kEps) {
  std::ostringstream oss;
  oss << msg << ", expected=" << format_value(expected)
      << ", obtained=" << format_value(obtained)
      << ", tolerance=" << format_value(tol) << format_time(time);
  return require(approx_equal(obtained, expected, tol), oss.str());
}

bool require_ge_at_time(double obtained, double expected_min, double time,
                        const std::string &msg, double tol = kEps) {
  std::ostringstream oss;
  oss << msg << ", expected >= " << format_value(expected_min)
      << ", obtained=" << format_value(obtained) << format_time(time)
      << ", tolerance=" << format_value(tol);
  return require(obtained >= expected_min - tol, oss.str());
}

bool require_le_at_time(double obtained, double expected_max, double time,
                        const std::string &msg, double tol = kEps) {
  std::ostringstream oss;
  oss << msg << ", expected <= " << format_value(expected_max)
      << ", obtained=" << format_value(obtained) << format_time(time)
      << ", tolerance=" << format_value(tol);
  return require(obtained <= expected_max + tol, oss.str());
}

bool require_lt_at_time(double obtained, double threshold, double time,
                        const std::string &msg, double tol = kEps) {
  std::ostringstream oss;
  oss << msg << ", expected < " << format_value(threshold)
      << ", obtained=" << format_value(obtained) << format_time(time)
      << ", tolerance=" << format_value(tol);
  return require(obtained < threshold + tol, oss.str());
}

bool require_gt_at_time(double obtained, double threshold, double time,
                        const std::string &msg, double tol = kEps) {
  std::ostringstream oss;
  oss << msg << ", expected > " << format_value(threshold)
      << ", obtained=" << format_value(obtained) << format_time(time)
      << ", tolerance=" << format_value(tol);
  return require(obtained > threshold - tol, oss.str());
}

bool require_lt(double obtained, double threshold, const std::string &msg,
                double tol = kEps) {
  std::ostringstream oss;
  oss << msg << ", expected < " << format_value(threshold)
      << ", obtained=" << format_value(obtained)
      << ", tolerance=" << format_value(tol);
  return require(obtained < threshold + tol, oss.str());
}

bool require_gt(double obtained, double threshold, const std::string &msg,
                double tol = kEps) {
  std::ostringstream oss;
  oss << msg << ", expected > " << format_value(threshold)
      << ", obtained=" << format_value(obtained)
      << ", tolerance=" << format_value(tol);
  return require(obtained > threshold - tol, oss.str());
}

cncpp::Block make_origin_block(cncpp::Machine &m) {
  cncpp::Block b("N1 g00 x0 y0 z0");
  b.parse(&m);
  return b;
}

int test_unparsed_throws() {
  int failures = 0;
  using cncpp::Block;
  using ::data_t;

  Block b("N2 g01 x100 y100 z0 f1000");

  {
    bool threw = false;
    try {
      (void)b.desc(false);
    } catch (const std::runtime_error &) {
      threw = true;
    }
    failures += !require(threw, "desc throws when block is not parsed");
  }

  {
    bool threw = false;
    data_t s = 0;
    try {
      (void)b.lambda(0.0, s);
    } catch (const std::runtime_error &) {
      threw = true;
    }
    failures += !require(threw, "lambda throws when block is not parsed");
  }

  {
    bool threw = false;
    try {
      (void)b.interpolate(0.5);
    } catch (const std::runtime_error &) {
      threw = true;
    }
    failures += !require(threw,
                         "interpolate(lambda) throws when block is not parsed");
  }

  {
    bool threw = false;
    data_t l = 0;
    data_t s = 0;
    try {
      (void)b.interpolate(0.0, l, s);
    } catch (const std::runtime_error &) {
      threw = true;
    }
    failures += !require(threw,
                         "interpolate(time, lambda, speed) throws when block is not parsed");
  }

  {
    bool threw = false;
    try {
      b.walk([](Block &, data_t, data_t, data_t) {});
    } catch (const std::runtime_error &) {
      threw = true;
    }
    failures += !require(threw, "walk throws when block is not parsed");
  }

  return failures;
}

int test_line_interpolation() {
  int failures = 0;
  using cncpp::Block;
  using cncpp::Machine;
  using ::data_t;

  Machine m;
  Block b0 = make_origin_block(m);
  Block b1("N2 g01 x100 y100 z0 f1000", b0);
  b1.parse(&m);

  failures += !require(b1.parsed(), "line block is parsed");
  failures += !require(b1.type() == Block::BlockType::LINE, "line block type is G01");
  failures += !require(approx_equal(b1.target().x(), 100.0), "line target X = 100");
  failures += !require(approx_equal(b1.target().y(), 100.0), "line target Y = 100");
  failures += !require(approx_equal(b1.target().z(), 0.0), "line target Z = 0");
  failures += !require(approx_equal(b1.feedrate(), 1000.0), "line feedrate = 1000");

  const data_t dt = b1.profile().dt;
  failures += !require(dt > 0.0, "line profile duration is positive");

  {
    data_t l = -1.0, s = -1.0;
    (void)b1.interpolate(0.0, l, s);
    failures += !require_near(l, 0.0, "interpolate at t=0 gives lambda=0");
    failures += !require_near(s, 0.0, "interpolate at t=0 gives speed=0");
  }

  {
    data_t l = -1.0, s = -1.0;
    (void)b1.interpolate(dt, l, s);
    failures += !require_near(l, 1.0, "interpolate at t=dt gives lambda=1");
    failures += !require_near(s, 0.0, "interpolate at t=dt gives speed=0");
  }

  {
    data_t l = -1.0, s = -1.0;
    (void)b1.interpolate(dt / 2.0, l, s);
    failures += !require_near(l, 0.5,
                              "interpolate at t=dt/2 gives lambda=0.5");
  }

  {
    data_t l = -1.0, s = -1.0;
    (void)b1.interpolate(-1.0, l, s);
    failures += !require_near(l, 0.0,
                              "interpolate with t<0 clamps lambda=0");
  }

  {
    data_t l = -1.0, s = -1.0;
    (void)b1.interpolate(dt + 1.0, l, s);
    failures += !require_near(l, 1.0,
                              "interpolate with t>dt clamps lambda=1");
  }

  {
    const data_t expected_length = std::hypot(100.0, 100.0);
    failures += !require_near(b1.length(), expected_length,
                              "line length is hypot(100,100)",
                              kLengthTol);
  }

  {
    data_t max_speed = -std::numeric_limits<data_t>::infinity();
    b1.walk([&](Block &, data_t, data_t, data_t s) {
      max_speed = std::max(max_speed, s);
    });
    failures += !require_le_at_time(max_speed, 1000.0, dt,
                                    "walk max speed <= 1000");
    failures += !require_ge_at_time(max_speed, 990.0, dt,
                                    "walk max speed >= 990");
  }

  return failures;
}

int test_arc_cw_ij() {
  int failures = 0;
  using cncpp::Block;
  using cncpp::Machine;
  using cncpp::Point;
  using ::data_t;

  Machine m;
  Block b0 = make_origin_block(m);
  Block b1("N2 g02 x100 y0 z0 i50 j50 f1000", b0);
  b1.parse(&m);

  const data_t dt = b1.profile().dt;
  const data_t r = b1.r();
  const data_t expected = 0.75 * (2.0 * kPi * r);
  failures += !require_ge_at_time(r, 0.0, dt,
                                  "G02 I,J derived radius r is non-negative");
  failures += !require_near(b1.length(), expected,
                            "G02 I,J arc length is 3/4 perimeter of r",
                            kLengthTol);

  b1.walk([&](Block &b, data_t t, data_t l, data_t) {
    Point p = b.interpolate(l);
    failures += !require_ge_at_time(p.y(), 0.0, t,
                                    "G02 I,J arc has Y never negative");
    if (t < dt / 2.0) {
      failures += !require_lt_at_time(p.x(), 50.0, t,
                                      "G02 I,J arc has X<50 for t<dt/2");
    }
    if (t > dt / 2.0) {
      failures += !require_gt_at_time(p.x(), 50.0, t,
                                      "G02 I,J arc has X>50 for t>dt/2");
    }
  });

  return failures;
}

int test_arc_ccw_ij() {
  int failures = 0;
  using cncpp::Block;
  using cncpp::Machine;
  using cncpp::Point;
  using ::data_t;

  Machine m;
  Block b0 = make_origin_block(m);
  Block b1("N2 g03 x100 y0 z0 i50 j50 f1000", b0);
  b1.parse(&m);

  const data_t dt = b1.profile().dt;
  const data_t r = b1.r();
  const data_t expected = 0.25 * (2.0 * kPi * r);
  failures += !require_ge_at_time(r, 0.0, dt,
                                  "G03 I,J derived radius r is non-negative");
  failures += !require_near(b1.length(), expected,
                            "G03 I,J arc length is 1/4 perimeter of r",
                            kLengthTol);

  b1.walk([&](Block &b, data_t t, data_t l, data_t) {
    Point p = b.interpolate(l);
    failures += !require_le_at_time(p.y(), 0.0, t,
                                    "G03 I,J arc has Y never positive");
    if (t < dt / 2.0) {
      failures += !require_lt_at_time(p.x(), 50.0, t,
                                      "G03 I,J arc has X<50 for t<dt/2");
    }
    if (t > dt / 2.0) {
      failures += !require_gt_at_time(p.x(), 50.0, t,
                                      "G03 I,J arc has X>50 for t>dt/2");
    }
  });

  return failures;
}

int test_arc_cw_r_positive() {
  int failures = 0;
  using cncpp::Block;
  using cncpp::Machine;
  using cncpp::Point;
  using ::data_t;

  Machine m;
  Block b0 = make_origin_block(m);
  Block b1("N2 g02 x100 y0 z0 R60 f1000", b0);
  b1.parse(&m);

  const data_t dt = b1.profile().dt;
  const data_t r = b1.r();
  const data_t half_perimeter = 0.5 * (2.0 * kPi * r);
  const data_t y_limit = 30.0;
  failures += !require_lt(b1.length(), half_perimeter,
                          "G02 R60 arc length is less than 1/2 perimeter of r",
                          kLengthTol);

  b1.walk([&](Block &b, data_t t, data_t l, data_t) {
    Point p = b.interpolate(l);
    failures += !require_ge_at_time(p.y(), 0.0, t,
                                    "G02 R60 arc has Y never negative");
    failures += !require_le_at_time(p.y(), y_limit, t,
                                    "G02 R60 arc has Y <= 30");
    if (t < dt / 2.0) {
      failures += !require_lt_at_time(p.x(), 50.0, t,
                                      "G02 R60 arc has X<50 for t<dt/2");
    }
    if (t > dt / 2.0) {
      failures += !require_gt_at_time(p.x(), 50.0, t,
                                      "G02 R60 arc has X>50 for t>dt/2");
    }
  });

  return failures;
}

int test_arc_ccw_r_negative() {
  int failures = 0;
  using cncpp::Block;
  using cncpp::Machine;
  using cncpp::Point;
  using ::data_t;

  Machine m;
  Block b0 = make_origin_block(m);
  Block b1("N2 g03 x100 y0 z0 R-60 f1000", b0);
  b1.parse(&m);

  const data_t dt = b1.profile().dt;
  const data_t r = b1.r();
  const data_t half_perimeter = 0.5 * (2.0 * kPi * r);
  const data_t y_threshold = -30.0;
  bool found_y_gt_neg30 = false;
  data_t max_y = -std::numeric_limits<data_t>::infinity();

  failures += !require_gt(b1.length(), half_perimeter,
                          "G03 R-60 arc length is more than 1/2 perimeter of r",
                          kLengthTol);

  b1.walk([&](Block &b, data_t t, data_t l, data_t) {
    Point p = b.interpolate(l);
    max_y = std::max(max_y, p.y());
    failures += !require_le_at_time(p.y(), 0.0, t,
                                    "G03 R-60 arc has Y never positive");
    if (p.y() > y_threshold) {
      found_y_gt_neg30 = true;
    }
    if (t < dt / 2.0) {
      failures += !require_lt_at_time(p.x(), 50.0, t,
                                      "G03 R-60 arc has X<50 for t<dt/2");
    }
    if (t > dt / 2.0) {
      failures += !require_gt_at_time(p.x(), 50.0, t,
                                      "G03 R-60 arc has X>50 for t>dt/2");
    }
  });

  {
    std::ostringstream oss;
    oss << "G03 R-60 arc has some Y values larger than -30, expected > "
        << format_value(y_threshold) << ", obtained max=" << format_value(max_y);
    failures += !require(found_y_gt_neg30, oss.str());
  }
  return failures;
}

// Returns true if parsing `line` (after an origin block) throws a
// std::runtime_error whose message mentions the offending line
bool parse_throws_runtime_error(cncpp::Machine &m, const std::string &line) {
  cncpp::Block b0 = make_origin_block(m);
  cncpp::Block b1(line, b0);
  try {
    b1.parse(&m);
  } catch (const std::runtime_error &e) {
    return std::string(e.what()).find(line) != std::string::npos;
  } catch (const std::exception &) {
    return false;
  }
  return false;
}

int test_parsing_errors() {
  int failures = 0;
  cncpp::Machine m;
  failures += !require(parse_throws_runtime_error(m, "N2 G17 X10"),
                       "unsupported G code throws runtime_error");
  failures += !require(parse_throws_runtime_error(m, "N2 G00 X1a"),
                       "trailing garbage in number throws runtime_error");
  failures += !require(parse_throws_runtime_error(m, "N2 G00 Xabc"),
                       "non-numeric argument throws runtime_error");
  failures += !require(parse_throws_runtime_error(m, "N2 G1.5 X10"),
                       "non-integer G code throws runtime_error");
  failures += !require(parse_throws_runtime_error(m, "N2 G01 X10"),
                       "interpolated block without feedrate throws");
  return failures;
}

int test_modal_motion_type() {
  int failures = 0;
  using cncpp::Block;
  cncpp::Machine m;
  Block b0 = make_origin_block(m);
  Block b1("N2 G01 X10 F1000", b0);
  b1.parse(&m);
  Block b2("N3 X20", b1);
  b2.parse(&m);
  failures += !require(b2.type() == Block::BlockType::LINE,
                       "G01 is modal: block without G word inherits LINE");
  failures += !require_near(b2.feedrate(), 1000.0, "feedrate is modal");
  return failures;
}

int test_zero_length_block() {
  int failures = 0;
  using cncpp::Block;
  using ::data_t;
  cncpp::Machine m;
  Block b0 = make_origin_block(m);
  Block b1("N2 G01 X0 Y0 Z0 F1000", b0);
  b1.parse(&m);
  data_t s = 0;
  data_t l = b1.lambda(0.0, s);
  failures += !require(std::isfinite(b1.dt()), "zero-length block: finite dt");
  failures += !require(std::isfinite(l) && std::isfinite(s),
                       "zero-length block: finite lambda and speed");
  return failures;
}

int test_walk_no_nan() {
  int failures = 0;
  using cncpp::Block;
  using ::data_t;
  cncpp::Machine m;
  Block b0 = make_origin_block(m); // G00 block: no profile
  bool finite = true;
  b0.walk([&](Block &, data_t, data_t l, data_t s) {
    finite = finite && std::isfinite(l) && std::isfinite(s);
  });
  failures += !require(finite, "walk on rapid block yields no NaN");

  Block b1("N2 G01 X100 Y100 Z0 F1000", b0);
  b1.parse(&m);
  data_t last_t = -1, last_l = -1;
  b1.walk([&](Block &, data_t t, data_t l, data_t) {
    last_t = t;
    last_l = l;
  });
  failures += !require_near(last_t, b1.dt(), "walk last sample is at t=dt",
                            m.tq() / 10.0);
  failures += !require_near(last_l, 1.0, "walk last sample has lambda=1");
  return failures;
}

int test_fmax_clamp() {
  int failures = 0;
  using cncpp::Block;
  using ::data_t;
  nlohmann::json cfg = {{"fmax", 500.0}};
  cncpp::Machine m(cfg);
  Block b0 = make_origin_block(m);
  Block b1("N2 G01 X100 F1000", b0);
  b1.parse(&m);
  data_t max_s = 0;
  b1.walk([&](Block &, data_t, data_t, data_t s) { max_s = std::max(max_s, s); });
  failures += !require_lt(max_s, 500.0 + kEps, "feedrate clamped to fmax");
  return failures;
}

int test_full_and_helical_arcs() {
  int failures = 0;
  using cncpp::Block;
  cncpp::Machine m;
  Block b0 = make_origin_block(m);
  Block full("N2 G03 X0 Y0 I10 J0 F1000", b0);
  full.parse(&m);
  failures += !require_near(full.length(), 2 * kPi * 10, "full CCW circle length",
                            kLengthTol);

  Block helix("N2 G03 X20 Y0 Z10 I10 J0 F1000", b0);
  helix.parse(&m);
  failures += !require_near(helix.length(), std::hypot(kPi * 10, 10.0),
                            "helical arc length includes Z", kLengthTol);
  return failures;
}

int test_first_block_interpolation() {
  int failures = 0;
  using cncpp::Block;
  cncpp::Machine m;
  Block b("N1 G01 X10 Y0 Z5 F1000"); // no previous block: starts at zero
  b.parse(&m);
  cncpp::Point p = b.interpolate(0.5);
  failures += !require_near(p.x(), 5.0, "first block: x at lambda=0.5");
  failures += !require_near(p.z(), 2.5, "first block: z at lambda=0.5");
  return failures;
}

} // namespace

int main() {
  int failures = 0;
  failures += test_unparsed_throws();
  failures += test_line_interpolation();
  failures += test_arc_cw_ij();
  failures += test_arc_ccw_ij();
  failures += test_arc_cw_r_positive();
  failures += test_arc_ccw_r_negative();
  failures += test_parsing_errors();
  failures += test_modal_motion_type();
  failures += test_zero_length_block();
  failures += test_walk_no_nan();
  failures += test_fmax_clamp();
  failures += test_full_and_helical_arcs();
  failures += test_first_block_interpolation();

  if (failures == 0) {
    std::cout << "All Block tests passed.\n";
  } else {
    std::cout << failures << " Block test assertion(s) failed.\n";
  }
  return failures;
}