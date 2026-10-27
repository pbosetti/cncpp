/*
  ____  _            _           _
 | __ )| | ___   ___| | __   ___| | __ _ ___ ___
 |  _ \| |/ _ \ / __| |/ /  / __| |/ _` / __/ __|
 | |_) | | (_) | (__|   <  | (__| | (_| \__ \__ \
 |____/|_|\___/ \___|_|\_\  \___|_|\__,_|___/___/

A Block loads a single line of G-Code, does the parsing, calculates the
trapezoidal velocity profile, and eventually performs the axes interpolation
*/
#pragma once
#include "defines.hpp"
#include "machine.hpp"
#include "point.hpp"
#include <functional>

namespace cncpp {

/**
 * @brief Represents a single parsed G-code block.
 *
 * A block stores one line of G-code together with its decoded motion,
 * geometric parameters, and feed profile. Blocks are linked with previous and
 * next blocks to form a full program path: on parse(), a block inherits the
 * modal state (motion type, feedrate, spindle, tool, block number) and the
 * start point from its previous block, so after inserting a block in the
 * middle of a sequence, re-parsing the following blocks updates them.
 *
 * Links are raw pointers: a copied block still points at the neighbors of the
 * original. Blocks owned by a Program are linked by the Program itself.
 */
class Block : public Object {
public:
  /**
   * @brief Supported motion type for the block.
   */
  enum class BlockType { RAPID = 0, LINE, CWA, CCWA, NO_MOTION };

  /**
   * @brief Kinematic profile used to execute this block.
   *
   * Units: lengths in mm, times in s, feeds in mm/s, accelerations in mm/s^2.
   */
  struct Profile {
    data_t a = 0, d = 0;
    data_t f = 0, l = 0;
    data_t dt_1 = 0, dt_m = 0, dt_2 = 0;
    data_t dr = 0;

    /**
     * @brief Computes a trapezoidal (or triangular) velocity profile.
     * @param l Path length (mm).
     * @param f Nominal feedrate (mm/min).
     * @param A Maximum acceleration (mm/s^2).
     * @param m Machine used to quantize the total duration.
     * @return The profile; all zeros for a zero-length path.
     */
    static Profile trapezoid(data_t l, data_t f, data_t A, Machine const &m);

    /**
     * @brief Computes normalized path position over profile time.
     * @param t Elapsed time from the beginning of the block.
     * @param s Output instantaneous speed (mm/min).
     * @return Normalized path coordinate in [0, 1].
     */
    data_t lambda(data_t t, data_t &s); // s is the current speed
  };

  // LIFECYCLE =================================================================
  /**
   * @brief Creates a block from a raw G-code line.
   * @param line Raw block text.
   */
  Block(std::string &line);

  /**
   * @brief Creates a block linked after a previous one.
   *
   * Sets `prev` as previous block of this one, and this one as next block of
   * `prev`. Modal state is inherited when the block is parsed.
   * @param line Raw block text.
   * @param prev Previous block in the sequence.
   */
  Block(std::string &line, Block &prev);

  /**
   * @brief Returns a human-readable description of the block.
   * @param colored Enables ANSI color formatting in the output.
   * @return Text description of block state and parameters.
   */
  std::string desc(bool colored = true) const override;

  /**
   * @brief Parses the block using the provided machine context.
   *
   * Parsing starts from a clean state, so a block can be parsed again after
   * its previous block has changed. The previous block must be parsed.
   * @param machine Machine settings and limits used for interpretation.
   * @return Reference to this block.
   */
  Block &parse(const Machine *machine);

  // INTERFACE =================================================================

  /**
   * @brief Computes normalized path position for the given time.
   * @param time Elapsed time from block start.
   * @param speed Output speed at the given time.
   * @return Normalized path coordinate in [0, 1].
   */
  data_t lambda(data_t time, data_t &speed) const;

  /**
   * @brief Interpolates a point from normalized path coordinate.
   * @param lambda Normalized path coordinate in [0, 1].
   * @return Interpolated point on this block trajectory.
   */
  Point interpolate(data_t lambda) const;

  /**
   * @brief Interpolates trajectory point and kinematics at a given time.
   * @param time Elapsed time from block start.
   * @param lambda Output normalized path coordinate.
   * @param speed Output speed value.
   * @return Interpolated point on this block trajectory.
   */
  Point interpolate(data_t time, data_t &lambda, data_t &speed) const;

  /**
   * @brief Iterates along the block profile and invokes a callback.
   *
   * The callback is invoked at t = 0, tq, 2 tq, ..., dt.
   * @param func Callback receiving block, time, lambda and speed values.
   */
  void walk(std::function<void(Block &b, data_t t, data_t l, data_t s)> func) const;

  // ACCESSORS =================================================================

  /**
   * @brief Sets the arc center coordinates.
   * @param value The arc center.
   * @return A constant reference to the assigned center.
   */
  Point const &center(Point const &value) { return (_center = value); }

  /**
   * @brief Sets the angular displacement of the arc.
   * @param value The angular displacement.
   * @return The assigned displacement.
   */
  data_t dtheta(data_t value) { return (_dtheta = value); }

  /**
   * @brief Sets the axis projections of the block movement.
   * @param value The axis projections.
   * @return A constant reference to the assigned projections.
   */
  Point const &delta(Point const &value) { return (_delta = value); }

  /**
   * @brief Sets the feed rate.
   * @param value The feed rate.
   * @return The assigned feed rate.
   */
  data_t feedrate(data_t value) { return (_feedrate = value); }

  /**
   * @brief Sets the arc's I parameter.
   * @param value The I parameter.
   * @return The assigned value.
   */
  data_t i(data_t value) { return (_i = value); }

  /**
   * @brief Sets the arc's J parameter.
   * @param value The J parameter.
   * @return The assigned value.
   */
  data_t j(data_t value) { return (_j = value); }

  /**
   * @brief Sets the movement length.
   * @param value The length.
   * @return The assigned length.
   */
  data_t length(data_t value) { return (_length = value); }

  /**
   * @brief Sets the original G-code line.
   * @param value The source line.
   * @return A constant reference to the assigned line.
   */
  std::string const &line(std::string const &value) { return (_line = value); }

  /**
   * @brief Sets the M-code value.
   * @param value The M-code value.
   * @return The assigned value.
   */
  size_t m(size_t value) { return (_m = value); }

  /**
   * @brief Sets the block number.
   * @param value The block number.
   * @return The assigned block number.
   */
  size_t n(size_t value) { return (_n = value); }

  /**
   * @brief Sets the next block in the program.
   * @param value The following block, or `nullptr` if there is none.
   * @return The assigned next-block pointer.
   */
  Block *next(Block *value) { return (_next = value); }

  /**
   * @brief Sets whether the block has been parsed.
   * @param value `true` if parsed; otherwise, `false`.
   * @return The assigned parsed state.
   */
  bool parsed(bool value) { return (_parsed = value); }

  /**
   * @brief Sets the previous block in the program.
   * @param value The preceding block, or `nullptr` if there is none.
   * @return The assigned previous-block pointer.
   */
  Block *prev(Block *value) { return (_prev = value); }

  /**
   * @brief Sets the arc's radius.
   * @param value The radius.
   * @return The assigned radius.
   */
  data_t r(data_t value) { return (_r = value); }

  /**
   * @brief Sets the spindle speed.
   * @param value The spindle speed.
   * @return The assigned spindle speed.
   */
  data_t spindle(data_t value) { return (_spindle = value); }

  /**
   * @brief Sets the initial position of the block.
   * @param value The initial position.
   * @return A constant reference to the assigned position.
   */
  Point const &start(Point const &value) { return (_start = value); }

  /**
   * @brief Sets the final position of the block.
   * @param value The final position.
   * @return A constant reference to the assigned position.
   */
  Point const &target(Point const &value) { return (_target = value); }

  /**
   * @brief Sets the initial angle of the arc.
   * @param value The initial angle.
   * @return The assigned angle.
   */
  data_t theta_0(data_t value) { return (_theta_0 = value); }

  /**
   * @brief Sets the tool number.
   * @param value The tool number.
   * @return The assigned tool number.
   */
  size_t tool(size_t value) { return (_tool = value); }

  const Profile &profile() const { return _profile; }

private:
  std::string _line; // original G-Code line
  data_t _tq;        // quantization time, to be loaded from machine
  size_t _n;     // block number (size_t is the longest unsigned integer on the
                 // platform)
  Point _start;  // initial block position
  Point _target; // final block position
  Point _center; // arc center coords
  Point _delta;  // projections on axes
  data_t _length = 0.0;
  data_t _i = 0, _j = 0, _r = 0;
  data_t _theta_0 = 0, _dtheta = 0;
  data_t _feedrate = 0;
  data_t _spindle = 0;
  size_t _m = 0;
  size_t _tool = 0;
  bool _parsed = false;
  Block *_prev = nullptr, *_next = nullptr;
  Profile _profile;
  BlockType _type = BlockType::NO_MOTION;

  // Methods
  void parse_token(std::string const &token);

}; // class Block

} // namespace cncpp