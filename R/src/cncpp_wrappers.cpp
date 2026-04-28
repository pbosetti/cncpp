#include <Rcpp.h>

#include "cncpp.hpp"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace {

struct RBlock {
  explicit RBlock(std::string line)
      : block(std::move(line)), machine(std::make_shared<cncpp::Machine>()) {}

  RBlock(std::string line, std::shared_ptr<RBlock> previous_block)
      : block(std::move(line), previous_block->block),
        machine(previous_block->machine), previous(std::move(previous_block)) {}

  cncpp::Block block;
  std::shared_ptr<cncpp::Machine> machine;
  std::shared_ptr<RBlock> previous;
};

using RBlockHandle = std::shared_ptr<RBlock>;

opt_data_t optional_scalar(SEXP value, const char *name) {
  if (Rf_isNull(value)) {
    return std::nullopt;
  }

  Rcpp::NumericVector vector(value);
  if (vector.size() != 1 || Rcpp::NumericVector::is_na(vector[0])) {
    Rcpp::stop("%s must be NULL or a non-missing numeric scalar", name);
  }
  return static_cast<data_t>(vector[0]);
}

cncpp::Point &point_ref(SEXP point) {
  if (!Rf_inherits(point, "cncpp_point")) {
    Rcpp::stop("Expected a cncpp_point external pointer");
  }

  Rcpp::XPtr<cncpp::Point> ptr(point);
  if (ptr.get() == nullptr) {
    Rcpp::stop("Point external pointer is null");
  }
  return *ptr;
}

RBlockHandle block_ref(SEXP block) {
  if (!Rf_inherits(block, "cncpp_block")) {
    Rcpp::stop("Expected a cncpp_block external pointer");
  }

  Rcpp::XPtr<RBlockHandle> ptr(block);
  if (ptr.get() == nullptr || !*ptr) {
    Rcpp::stop("Block external pointer is null");
  }
  return *ptr;
}

SEXP make_point(cncpp::Point point) {
  Rcpp::XPtr<cncpp::Point> ptr(new cncpp::Point(std::move(point)), true);
  ptr.attr("class") =
      Rcpp::CharacterVector::create("cncpp_point", "externalptr");
  return ptr;
}

SEXP make_block(RBlockHandle block) {
  Rcpp::XPtr<RBlockHandle> ptr(new RBlockHandle(std::move(block)), true);
  ptr.attr("class") =
      Rcpp::CharacterVector::create("cncpp_block", "externalptr");
  return ptr;
}

Rcpp::NumericVector point_vector(cncpp::Point const &point) {
  auto values = point.vec();
  Rcpp::NumericVector output =
      Rcpp::NumericVector::create(values[0], values[1], values[2]);
  output.names() = Rcpp::CharacterVector::create("x", "y", "z");
  return output;
}

std::string block_type_name(cncpp::Block::BlockType type) {
  switch (type) {
  case cncpp::Block::BlockType::RAPID:
    return "RAPID";
  case cncpp::Block::BlockType::LINE:
    return "LINE";
  case cncpp::Block::BlockType::CWA:
    return "CWA";
  case cncpp::Block::BlockType::CCWA:
    return "CCWA";
  case cncpp::Block::BlockType::NO_MOTION:
    return "NO_MOTION";
  }
  return "UNKNOWN";
}

bool is_motion_type(cncpp::Block::BlockType type) {
  return type == cncpp::Block::BlockType::LINE ||
         type == cncpp::Block::BlockType::CWA ||
         type == cncpp::Block::BlockType::CCWA;
}

void require_interpolatable(RBlock const &block) {
  if (!block.block.parsed()) {
    Rcpp::stop("Block must be parsed before interpolation");
  }

  if (!is_motion_type(block.block.type())) {
    Rcpp::stop("Only LINE, CWA and CCWA blocks can be interpolated");
  }

  if (block.block.prev == nullptr) {
    Rcpp::stop("Interpolation requires a block linked to a parsed previous "
               "block");
  }
}

} // namespace

// [[Rcpp::export]]
SEXP cncpp_point_create(SEXP x, SEXP y, SEXP z) {
  return make_point(cncpp::Point(optional_scalar(x, "x"),
                                 optional_scalar(y, "y"),
                                 optional_scalar(z, "z")));
}

// [[Rcpp::export]]
std::string cncpp_point_desc(SEXP point, bool colored) {
  return point_ref(point).desc(colored);
}

// [[Rcpp::export]]
bool cncpp_point_is_complete(SEXP point) {
  return point_ref(point).is_complete();
}

// [[Rcpp::export]]
void cncpp_point_reset(SEXP point) { point_ref(point).reset(); }

// [[Rcpp::export]]
double cncpp_point_x(SEXP point) { return point_ref(point).x(); }

// [[Rcpp::export]]
double cncpp_point_y(SEXP point) { return point_ref(point).y(); }

// [[Rcpp::export]]
double cncpp_point_z(SEXP point) { return point_ref(point).z(); }

// [[Rcpp::export]]
void cncpp_point_set_x(SEXP point, double value) { point_ref(point).x(value); }

// [[Rcpp::export]]
void cncpp_point_set_y(SEXP point, double value) { point_ref(point).y(value); }

// [[Rcpp::export]]
void cncpp_point_set_z(SEXP point, double value) { point_ref(point).z(value); }

// [[Rcpp::export]]
Rcpp::NumericVector cncpp_point_vec(SEXP point) {
  return point_vector(point_ref(point));
}

// [[Rcpp::export]]
double cncpp_point_length(SEXP point) { return point_ref(point).length(); }

// [[Rcpp::export]]
SEXP cncpp_point_delta(SEXP point, SEXP other) {
  return make_point(point_ref(point).delta(point_ref(other)));
}

// [[Rcpp::export]]
SEXP cncpp_point_add(SEXP point, SEXP other) {
  return make_point(point_ref(point) + point_ref(other));
}

// [[Rcpp::export]]
void cncpp_point_modal(SEXP point, SEXP other) {
  point_ref(point).modal(point_ref(other));
}

// [[Rcpp::export]]
SEXP cncpp_block_create(std::string line, SEXP previous) {
  if (Rf_isNull(previous)) {
    return make_block(std::make_shared<RBlock>(std::move(line)));
  }

  return make_block(
      std::make_shared<RBlock>(std::move(line), block_ref(previous)));
}

// [[Rcpp::export]]
SEXP cncpp_block_parse(SEXP block) {
  auto handle = block_ref(block);
  handle->block.parse(handle->machine.get());
  return block;
}

// [[Rcpp::export]]
std::string cncpp_block_desc(SEXP block, bool colored) {
  return block_ref(block)->block.desc(colored);
}

// [[Rcpp::export]]
std::string cncpp_block_line(SEXP block) {
  return block_ref(block)->block.line();
}

// [[Rcpp::export]]
R_xlen_t cncpp_block_n(SEXP block) {
  return static_cast<R_xlen_t>(block_ref(block)->block.n());
}

// [[Rcpp::export]]
double cncpp_block_dt(SEXP block) { return block_ref(block)->block.dt(); }

// [[Rcpp::export]]
Rcpp::List cncpp_block_type(SEXP block) {
  auto type = block_ref(block)->block.type();
  return Rcpp::List::create(Rcpp::_["code"] = static_cast<int>(type),
                            Rcpp::_["name"] = block_type_name(type));
}

// [[Rcpp::export]]
R_xlen_t cncpp_block_tool(SEXP block) {
  return static_cast<R_xlen_t>(block_ref(block)->block.tool());
}

// [[Rcpp::export]]
double cncpp_block_feedrate(SEXP block) {
  return block_ref(block)->block.feedrate();
}

// [[Rcpp::export]]
double cncpp_block_arc_feedrate(SEXP block) {
  return block_ref(block)->block.arc_feedrate();
}

// [[Rcpp::export]]
double cncpp_block_spindle(SEXP block) {
  return block_ref(block)->block.spindle();
}

// [[Rcpp::export]]
double cncpp_block_length(SEXP block) {
  return block_ref(block)->block.length();
}

// [[Rcpp::export]]
SEXP cncpp_block_target(SEXP block) {
  return make_point(block_ref(block)->block.target());
}

// [[Rcpp::export]]
SEXP cncpp_block_center(SEXP block) {
  return make_point(block_ref(block)->block.center());
}

// [[Rcpp::export]]
SEXP cncpp_block_delta(SEXP block) {
  return make_point(block_ref(block)->block.delta());
}

// [[Rcpp::export]]
R_xlen_t cncpp_block_m(SEXP block) {
  return static_cast<R_xlen_t>(block_ref(block)->block.m());
}

// [[Rcpp::export]]
Rcpp::List cncpp_block_profile(SEXP block) {
  auto const &profile = block_ref(block)->block.profile();
  return Rcpp::List::create(
      Rcpp::_["a"] = profile.a, Rcpp::_["d"] = profile.d,
      Rcpp::_["f"] = profile.f, Rcpp::_["l"] = profile.l,
      Rcpp::_["fs"] = profile.fs, Rcpp::_["fe"] = profile.fe,
      Rcpp::_["dt_1"] = profile.dt_1, Rcpp::_["dt_m"] = profile.dt_m,
      Rcpp::_["dt_2"] = profile.dt_2, Rcpp::_["dt"] = profile.dt);
}

// [[Rcpp::export]]
bool cncpp_block_parsed(SEXP block) { return block_ref(block)->block.parsed(); }

// [[Rcpp::export]]
double cncpp_block_r(SEXP block) { return block_ref(block)->block.r(); }

// [[Rcpp::export]]
Rcpp::List cncpp_block_lambda(SEXP block, double time) {
  auto handle = block_ref(block);
  data_t speed = 0.0;
  data_t lambda = handle->block.lambda(time, speed);
  return Rcpp::List::create(
      Rcpp::_["lambda"] = lambda, Rcpp::_["speed"] = speed,
      Rcpp::_["acceleration"] = handle->block.profile().current_acc);
}

// [[Rcpp::export]]
SEXP cncpp_block_interpolate_lambda(SEXP block, double lambda) {
  auto handle = block_ref(block);
  require_interpolatable(*handle);
  return make_point(handle->block.interpolate(lambda));
}

// [[Rcpp::export]]
Rcpp::List cncpp_block_interpolate_time(SEXP block, double time) {
  auto handle = block_ref(block);
  require_interpolatable(*handle);
  data_t lambda = 0.0;
  data_t speed = 0.0;
  auto point = handle->block.interpolate(time, lambda, speed);
  return Rcpp::List::create(Rcpp::_["point"] = make_point(point),
                            Rcpp::_["lambda"] = lambda,
                            Rcpp::_["speed"] = speed);
}

// [[Rcpp::export]]
Rcpp::DataFrame cncpp_block_walk(SEXP block) {
  auto handle = block_ref(block);
  require_interpolatable(*handle);

  std::vector<double> time;
  std::vector<double> lambda;
  std::vector<double> speed;
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> z;

  handle->block.walk([&](cncpp::Block &block, data_t t, data_t l, data_t s) {
    cncpp::Point point = block.interpolate(l);
    time.push_back(t);
    lambda.push_back(l);
    speed.push_back(s);
    x.push_back(point.x());
    y.push_back(point.y());
    z.push_back(point.z());
  });

  return Rcpp::DataFrame::create(
      Rcpp::_["time"] = time, Rcpp::_["lambda"] = lambda,
      Rcpp::_["speed"] = speed, Rcpp::_["x"] = x, Rcpp::_["y"] = y,
      Rcpp::_["z"] = z, Rcpp::_["stringsAsFactors"] = false);
}

// [[Rcpp::export]]
Rcpp::List cncpp_block_summary(SEXP block) {
  auto handle = block_ref(block);
  auto type = handle->block.type();
  return Rcpp::List::create(
      Rcpp::_["line"] = handle->block.line(),
      Rcpp::_["n"] = static_cast<R_xlen_t>(handle->block.n()),
      Rcpp::_["parsed"] = handle->block.parsed(),
      Rcpp::_["type_code"] = static_cast<int>(type),
      Rcpp::_["type"] = block_type_name(type),
      Rcpp::_["tool"] = static_cast<R_xlen_t>(handle->block.tool()),
      Rcpp::_["feedrate"] = handle->block.feedrate(),
      Rcpp::_["arc_feedrate"] = handle->block.arc_feedrate(),
      Rcpp::_["spindle"] = handle->block.spindle(),
      Rcpp::_["length"] = handle->block.length(),
      Rcpp::_["dt"] = handle->block.dt(),
      Rcpp::_["m"] = static_cast<R_xlen_t>(handle->block.m()),
      Rcpp::_["r"] = handle->block.r());
}
