// Unit tests for the Block class (src/block.hpp)

#include "block.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <numbers>
#include <vector>

using namespace cncpp;
using Catch::Approx;
using std::numbers::pi;

static void require_point(Point const &p, data_t x, data_t y, data_t z) {
  REQUIRE(p.x() == Approx(x).margin(1e-9));
  REQUIRE(p.y() == Approx(y).margin(1e-9));
  REQUIRE(p.z() == Approx(z).margin(1e-9));
}

static bool is_multiple(data_t v, data_t q) {
  return std::abs(v / q - std::round(v / q)) < 1e-6;
}

//  ____             __ _ _
// |  _ \ _ __ ___  / _(_) | ___
// | |_) | '__/ _ \| |_| | |/ _ \
// |  __/| | | (_) |  _| | |  __/
// |_|   |_|  \___/|_| |_|_|\___|

TEST_CASE("Profile::trapezoid", "[block][profile]") {
  Machine m; // tq = 0.001
  data_t f = 1000, A = 5, f_m = f / 60.0;

  SECTION("zero length gives an all-zero profile") {
    auto p = Block::Profile::trapezoid(0, f, A, m);
    REQUIRE(p.dt == 0);
    REQUIRE(p.f == 0);
    REQUIRE(p.a == 0);
  }

  SECTION("long block is a trapezoid") {
    data_t l = 100;
    auto p = Block::Profile::trapezoid(l, f, A, m);
    REQUIRE(p.dt_m > 0);
    REQUIRE(p.a == Approx(-p.d));
    REQUIRE(is_multiple(p.dt, m.tq()));
    REQUIRE(p.dt >= l / f_m + f_m / A);
    REQUIRE(p.f * (p.dt_1 + p.dt_2 + 2 * p.dt_m) / 2 == Approx(l));
    REQUIRE(p.f <= f_m);
  }

  SECTION("short block is a triangle") {
    data_t l = 10;
    auto p = Block::Profile::trapezoid(l, f, A, m);
    REQUIRE(p.dt_m == 0);
    REQUIRE(is_multiple(p.dt, m.tq()));
    REQUIRE(p.dt >= 2 * std::sqrt(l / A));
    REQUIRE(p.f * (p.dt_1 + p.dt_2 + 2 * p.dt_m) / 2 == Approx(l));
    REQUIRE(p.f <= f_m);
  }
}

TEST_CASE("Profile::lambda", "[block][profile]") {
  Machine m;
  data_t s = -1;

  SECTION("zero-length profile is always at the end") {
    auto p = Block::Profile::trapezoid(0, 1000, 5, m);
    REQUIRE(p.lambda(0.5, s) == 1.0);
    REQUIRE(s == 0);
  }

  auto p = Block::Profile::trapezoid(100, 1000, 5, m);

  SECTION("before start and after end") {
    REQUIRE(p.lambda(-1, s) == 0.0);
    REQUIRE(s == 0);
    REQUIRE(p.lambda(p.dt, s) == 1.0);
    REQUIRE(s == 0);
    REQUIRE(p.lambda(p.dt + 1, s) == 1.0);
  }

  SECTION("monotonic and continuous") {
    data_t prev = 0;
    for (data_t t = 0; t <= p.dt; t += m.tq()) {
      data_t l = p.lambda(t, s);
      REQUIRE(l >= prev);
      prev = l;
    }
    data_t e = 1e-9;
    data_t t1 = p.dt_1, t2 = p.dt_1 + p.dt_m;
    REQUIRE(p.lambda(t1 - e, s) == Approx(p.lambda(t1 + e, s)));
    REQUIRE(p.lambda(t2 - e, s) == Approx(p.lambda(t2 + e, s)));
    REQUIRE(p.lambda(p.dt - e, s) == Approx(1.0));
  }

  SECTION("speed and acceleration in each phase") {
    p.lambda(p.dt_1 / 2, s);
    REQUIRE(p.current_acc == p.a);
    p.lambda(p.dt_1 + p.dt_m / 2, s);
    REQUIRE(p.current_acc == 0);
    REQUIRE(s == Approx(p.f * 60));
    p.lambda(p.dt - p.dt_2 / 2, s);
    REQUIRE(p.current_acc == p.d);
  }
}

//  _ _  __                      _
// | (_)/ _| ___  ___ _   _  ___| | ___
// | | | |_ / _ \/ __| | | |/ __| |/ _ \
// | | |  _|  __/ (__| |_| | (__| |  __/
// |_|_|_|  \___|\___|\__, |\___|_|\___|
//                    |___/

TEST_CASE("Block linked constructor", "[block]") {
  Block b1("N10 G01 X10 F1000");
  Block b2("N20 X20", b1);
  REQUIRE(b1.next() == &b2);
  REQUIRE_THROWS(b2.desc(false)); // not parsed
}

TEST_CASE("Block desc", "[block]") {
  Machine m;
  Block b("N10 G01 X10 Y0 Z0 F1000");
  REQUIRE_THROWS(b.desc(false));
  b.parse(&m);
  auto s = b.desc(false);
  REQUIRE(s.find('\x1b') == std::string::npos);
  REQUIRE(s.find("[ 10]") != std::string::npos);
  REQUIRE(s.find("G01") != std::string::npos);
  REQUIRE(s.find("10") != std::string::npos);
}

//  ____
// |  _ \ __ _ _ __ ___  ___
// | |_) / _` | '__/ __|/ _ \
// |  __/ (_| | |  \__ \  __/
// |_|   \__,_|_|  |___/\___|

TEST_CASE("Block parse errors", "[block][parse]") {
  Machine m;

  SECTION("null machine") {
    Block b("N10 G01 X10 F1000");
    REQUIRE_THROWS(b.parse(nullptr));
  }

  SECTION("unparsed previous block") {
    Block b1("N10 G01 X10 F1000");
    Block b2("N20 X20", b1);
    REQUIRE_THROWS(b2.parse(&m));
  }

  SECTION("bad words") {
    for (auto line : {"N10 Q1", "N10 X", "N10 X1.3Y2", "N10 G1.5", "N10.5",
                      "N10 T1.5", "N10 G05", "N0 G00 X1"}) {
      INFO(line);
      Block b(line);
      REQUIRE_THROWS(b.parse(&m));
    }
  }

  SECTION("non-increasing block number") {
    Block b1("N10 G00 X10");
    b1.parse(&m);
    Block b2("N10 X20", b1);
    REQUIRE_THROWS(b2.parse(&m));
  }

  SECTION("feedrate <= 0 on lines and arcs") {
    REQUIRE_THROWS(Block("N10 G01 X10").parse(&m));
    REQUIRE_THROWS(Block("N10 G01 X10 F-5").parse(&m));
    REQUIRE_THROWS(Block("N10 G02 X10 R10").parse(&m));
  }

  Block b1("N10 G01 X10 Y0 Z0 F1000");
  b1.parse(&m);

  SECTION("I/J arc with mismatching endpoints") {
    Block b2("N20 G02 X0 Y10 I-10 J1", b1);
    REQUIRE_THROWS(b2.parse(&m));
  }

  SECTION("R arc with radius shorter than half the chord") {
    Block b2("N20 G02 X0 Y10 R1", b1);
    REQUIRE_THROWS(b2.parse(&m));
  }
}

TEST_CASE("Block parse modal state", "[block][parse]") {
  Machine m;
  m.zero(1, 2, 3);

  SECTION("first block starts at machine zero, omitted coords kept") {
    Block b("N10 G01 X10 F1000");
    b.parse(&m);
    require_point(b.interpolate(0.0), 1, 2, 3);
    require_point(b.interpolate(1.0), 10, 2, 3);
  }

  SECTION("G, F, S, T and start are inherited") {
    Block b1("N10 G01 X10 F1000 S500 T2");
    b1.parse(&m);
    Block b2("N20 Y20", b1);
    b2.parse(&m);
    REQUIRE(b2.type() == Block::BlockType::LINE);
    REQUIRE(b2.desc(false).find("F 1000 S 500 T02") != std::string::npos);
    require_point(b2.interpolate(0.0), 10, 2, 3);
    require_point(b2.interpolate(1.0), 10, 20, 3);
  }

  SECTION("lowercase words and comments") {
    Block b("n10 g01 x10 f1000 ; X20");
    b.parse(&m);
    REQUIRE(b.n() == 10);
    require_point(b.interpolate(1.0), 10, 2, 3);
    Block c("N10 G01 X10 F1000 # X20");
    c.parse(&m);
    require_point(c.interpolate(1.0), 10, 2, 3);
  }
}

TEST_CASE("Block parse geometry", "[block][parse]") {
  Machine m;
  Block b1("N10 G01 X10 Y0 Z0 F1000");
  b1.parse(&m);

  SECTION("line length") {
    Block b2("N20 X13 Y4", b1);
    b2.parse(&m);
    REQUIRE(b2.profile().l == Approx(5));
  }

  SECTION("feedrate is capped to fmax") {
    m.A(1000); // long enough for a trapezoid
    Block b2("N20 X1000 F20000", b1);
    b2.parse(&m);
    REQUIRE(b2.profile().f <= m.fmax() / 60 + 1e-9);
    REQUIRE(b2.profile().f > 0.9 * m.fmax() / 60);
  }

  SECTION("I/J and R forms give the same arc, G03 is CCW") {
    Block ij("N20 G03 X0 Y10 I-10 J0", b1);
    ij.parse(&m);
    Block r("N20 G03 X0 Y10 R10", b1);
    r.parse(&m);
    require_point(ij.interpolate(0.5), 10 * std::sqrt(0.5),
                  10 * std::sqrt(0.5), 0);
    require_point(r.interpolate(0.5), 10 * std::sqrt(0.5),
                  10 * std::sqrt(0.5), 0);
    REQUIRE(ij.profile().l == Approx(pi / 2 * 10));
    REQUIRE(r.profile().l == Approx(ij.profile().l));
  }

  SECTION("G02 is CW") {
    Block b2("N20 G02 X0 Y10 I-10 J0", b1);
    b2.parse(&m);
    require_point(b2.interpolate(0.5), -10 * std::sqrt(0.5),
                  -10 * std::sqrt(0.5), 0);
    REQUIRE(b2.profile().l == Approx(1.5 * pi * 10));
  }

  SECTION("coinciding endpoints give a full circle") {
    Block b2("N20 G03 X10 Y0 I-10 J0", b1);
    b2.parse(&m);
    REQUIRE(b2.profile().l == Approx(2 * pi * 10));
  }

  SECTION("helical arc length") {
    Block b2("N20 G03 X0 Y10 Z5 I-10 J0", b1);
    b2.parse(&m);
    REQUIRE(b2.profile().l == Approx(std::hypot(pi / 2 * 10, 5)));
  }
}

TEST_CASE("Block re-parse gives the same result as a fresh parse", "[block]") {
  Machine m;
  Block b1("N10 G01 X10 Y0 Z0 F1000");
  b1.parse(&m);

  SECTION("I/J arc longer than 180 degrees") {
    // CW from (10,0) to (0,10) around (0,0): a 270 degree arc
    Block b2("N20 G02 X0 Y10 I-10 J0", b1);
    Point p1 = b2.parse(&m).interpolate(0.5);
    Point p2 = b2.parse(&m).interpolate(0.5);
    // halfway is at -135 degrees
    REQUIRE(p1.x() == Approx(-10 * std::sqrt(0.5)));
    REQUIRE(p1.y() == Approx(-10 * std::sqrt(0.5)));
    REQUIRE(p2.x() == Approx(p1.x()));
    REQUIRE(p2.y() == Approx(p1.y()));
  }

  SECTION("I/J full circle") {
    Block b2("N20 G02 X10 Y0 I-10 J0", b1);
    b2.parse(&m);
    REQUIRE_NOTHROW(b2.parse(&m));
    REQUIRE(b2.interpolate(0.5).x() == Approx(-10));
  }

  SECTION("omitted coordinates follow a changed previous block") {
    Block b2("N20 Y5", b1);
    b2.parse(&m);
    b1.line("N10 G01 X20 Y0 Z0 F1000");
    b1.parse(&m);
    b2.parse(&m);
    require_point(b2.interpolate(1.0), 20, 5, 0);
  }
}

//  ___       _                        _       _   _
// |_ _|_ __ | |_ ___ _ __ _ __   ___ | | __ _| |_(_) ___  _ __
//  | || '_ \| __/ _ \ '__| '_ \ / _ \| |/ _` | __| |/ _ \| '_ \
//  | || | | | ||  __/ |  | |_) | (_) | | (_| | |_| | (_) | | | |
// |___|_| |_|\__\___|_|  | .__/ \___/|_|\__,_|\__|_|\___/|_| |_|
//                        |_|

TEST_CASE("Block lambda", "[block]") {
  Machine m;
  Block b("N10 G01 X10 Y0 Z0 F1000");
  data_t s1, s2;
  REQUIRE_THROWS(b.lambda(0.1, s1));
  b.parse(&m);
  auto p = b.profile();
  REQUIRE(b.lambda(0.1, s1) == p.lambda(0.1, s2));
  REQUIRE(s1 == s2);
}

TEST_CASE("Block interpolate(lambda)", "[block]") {
  Machine m;

  SECTION("unparsed, RAPID and NO_MOTION blocks throw") {
    Block u("N10 G01 X10 F1000");
    REQUIRE_THROWS(u.interpolate(0.5));
    Block r("N10 G00 X10");
    r.parse(&m);
    REQUIRE_THROWS(r.interpolate(0.5));
    Block n("N10 M03");
    n.parse(&m);
    REQUIRE_THROWS(n.interpolate(0.5));
  }

  SECTION("line endpoints and midpoint") {
    Block b("N10 G01 X10 Y20 Z30 F1000");
    b.parse(&m);
    require_point(b.interpolate(0.0), 0, 0, 0);
    require_point(b.interpolate(0.5), 5, 10, 15);
    require_point(b.interpolate(1.0), 10, 20, 30);
  }

  SECTION("helical arc stays on the circle, Z is linear") {
    Block b1("N10 G01 X10 Y0 Z0 F1000");
    b1.parse(&m);
    Block b2("N20 G02 X0 Y10 Z6 I-10 J0", b1);
    b2.parse(&m);
    require_point(b2.interpolate(0.0), 10, 0, 0);
    require_point(b2.interpolate(1.0), 0, 10, 6);
    for (data_t l = 0; l <= 1.0; l += 0.1) {
      Point p = b2.interpolate(l);
      REQUIRE(std::hypot(p.x(), p.y()) == Approx(10));
      REQUIRE(p.z() == Approx(6 * l).margin(1e-9));
    }
  }
}

TEST_CASE("Block interpolate(time)", "[block]") {
  Machine m;
  Block b("N10 G01 X10 Y20 Z30 F1000");
  b.parse(&m);
  data_t l, s;
  Point p = b.interpolate(0.3, l, s);
  Point q = b.interpolate(l);
  require_point(p, q.x(), q.y(), q.z());
  require_point(b.interpolate(b.profile().dt, l, s), 10, 20, 30);
}

TEST_CASE("Block walk", "[block]") {
  Machine m;

  SECTION("unparsed block throws") {
    Block b("N10 G01 X10 F1000");
    REQUIRE_THROWS(b.walk([](Block &, data_t, data_t, data_t) {}));
  }

  SECTION("callback times and lambdas") {
    Block b("N10 G01 X10 F1000");
    b.parse(&m);
    std::vector<data_t> ts, ls;
    b.walk([&](Block &, data_t t, data_t l, data_t) {
      ts.push_back(t);
      ls.push_back(l);
    });
    REQUIRE(ts.size() == std::llround(b.profile().dt / m.tq()) + 1);
    REQUIRE(ts.front() == 0);
    REQUIRE(ls.front() == 0);
    REQUIRE(ts.back() == Approx(b.profile().dt));
    REQUIRE(ls.back() == Approx(1.0));
    for (size_t i = 1; i < ts.size(); i++)
      REQUIRE(ts[i] > ts[i - 1]);
  }

  SECTION("RAPID and NO_MOTION blocks call once") {
    for (auto line : {"N10 G00 X10", "N10 M03"}) {
      Block b(line);
      b.parse(&m);
      int calls = 0;
      b.walk([&](Block &, data_t, data_t, data_t) { calls++; });
      REQUIRE(calls == 1);
    }
  }
}
