/*
  ____  _            _           _
 | __ )| | ___   ___| | __   ___| | __ _ ___ ___
 |  _ \| |/ _ \ / __| |/ /  / __| |/ _` / __/ __|
 | |_) | | (_) | (__|   <  | (__| | (_| \__ \__ \
 |____/|_|\___/ \___|_|\_\  \___|_|\__,_|___/___/


Implements a G-code block, which is a collection of G-code commands that are
executed together. Each block can contain multiple commands, and each command
can have multiple parameters.
*/

#pragma once

#include "defines.hpp"
#include "point.hpp"
#include <functional>

namespace cncpp {

class Machine;

/**
 * @brief Represents a single parsed G-code block.
 *
 * A block stores one line of G-code together with its decoded motion,
 * geometric parameters, and feed profile. A block is a plain value: when it
 * is created from a previous block, it copies the modal state (motion type,
 * feedrate, spindle, tool) and the start point, and keeps no reference to it.
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
    /// Acceleration and deceleration magnitudes.
    data_t a = 0.0, d = 0.0;
    /// Peak feed and path length.
    data_t f = 0.0, l = 0.0;
    /// Phase durations (accel, cruise, decel).
    data_t dt_1 = 0.0, dt_m = 0.0, dt_2 = 0.0;
    /// Total block duration.
    data_t dt = 0.0;
    /// Instantaneous acceleration value, used mainly for arc motion.
    data_t current_acc = 0.0;

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
    data_t lambda(data_t t, data_t &s);
  };

  // LIFECYCLE =================================================================
  /**
   * @brief Creates a block from a raw G-code line.
   * @param line Raw block text.
   */
  Block(std::string line);

  /**
   * @brief Creates a block following a previous one, inheriting modal state.
   * @param line Raw block text.
   * @param prev Previous (already parsed) block in the sequence.
   */
  Block(std::string line, Block const &prev);

  /**
   * @brief Returns a human-readable description of the block.
   * @param colored Enables ANSI color formatting in the output.
   * @return Text description of block state and parameters.
   */
  std::string desc(bool colored = true) const override;

  // OPERATIONS/OPERATORS ======================================================
  /**
   * @brief Parses the block using the provided machine context.
   * @param m Machine settings and limits used for interpretation.
   * @return Reference to this block.
   */
  Block &parse(const Machine *m);

  /**
   * @brief Computes normalized path position for the given time.
   * @param time Elapsed time from block start.
   * @param speed Output speed at the given time.
   * @return Normalized path coordinate in [0, 1].
   */
  data_t lambda(data_t time, data_t &speed);

  /**
   * @brief Interpolates a point from normalized path coordinate.
   * @param lambda Normalized path coordinate in [0, 1].
   * @return Interpolated point on this block trajectory.
   */
  Point interpolate(data_t lambda);

  /**
   * @brief Interpolates trajectory point and kinematics at a given time.
   * @param time Elapsed time from block start.
   * @param lambda Output normalized path coordinate.
   * @param speed Output speed value.
   * @return Interpolated point on this block trajectory.
   */
  Point interpolate(data_t time, data_t &lambda, data_t &speed);

  /**
   * @brief Iterates along the block profile and invokes a callback.
   *
   * The callback is invoked at t = 0, tq, 2 tq, ..., dt.
   * @param func Callback receiving block, time, lambda and speed values.
   */
  void walk(std::function<void(Block &b, data_t t, data_t l, data_t s)> func);

  // ACCESSORS =================================================================
  /** @brief Returns the original G-code line text. */
  std::string line() const { return _line; }
  /** @brief Returns the block number parsed from the N word. */
  size_t n() const { return _n; }
  /** @brief Returns total execution time of the block. */
  data_t dt() const { return _profile.dt; }
  /** @brief Returns the decoded block motion type. */
  BlockType type() const { return _type; }
  /** @brief Returns the active tool number for this block. */
  size_t tool() const { return _tool; }
  /** @brief Returns programmed feedrate (mm/min). */
  data_t feedrate() const { return _feedrate; }
  /** @brief Returns spindle speed value. */
  data_t spindle() const { return _spindle; }
  /** @brief Returns geometric path length of the block. */
  data_t length() const { return _length; }
  /** @brief Returns start point of the block. */
  const Point &start() const { return _start; }
  /** @brief Returns target point of the block. */
  const Point &target() const { return _target; }
  /** @brief Returns arc center point for circular motion. */
  const Point &center() const { return _center; }
  /** @brief Returns path delta between start and target. */
  const Point &delta() const { return _delta; }
  /** @brief Returns the argument value of the M word (if present). */
  size_t m() const { return _m; }
  /** @brief Returns computed motion profile for this block. */
  const Profile &profile() const { return _profile; }
  /** @brief Returns true when the block has been parsed. */
  bool parsed() const { return _parsed; }
  /** @brief Returns the radius of the arc. */
  data_t r() const { return _r; }

private:
  std::string _line;                // original G-Code line, eg "N01 G00 X100"
  size_t _n = 0;                    // block number
  size_t _min_n = 0;                // smallest allowed block number
  Point _start;                     // previous target (machine zero if first)
  Point _target;                    // block destination
  Point _center;                    // arc center
  Point _delta;                     // block projections
  data_t _length = 0;               // block length
  data_t _i = 0, _j = 0, _r = 0;    // arc parameters
  data_t _theta_0 = 0, _dtheta = 0; // arc initial and included angles
  data_t _feedrate = 0;             // programmed feedrate (mm/min)
  data_t _spindle = 0;              // spindle rate
  size_t _m = 0;                    // M command argument
  size_t _tool = 0;                 // current tool number
  BlockType _type = BlockType::NO_MOTION; // type of block
  Profile _profile;                       // feedrate profile
  data_t _tq = 0;                         // machine sampling period
  bool _parsed = false;                   // has it been parsed?

  // UTILITIES =================================================================
  void parse_token(std::string const &token);
  void calc_arc(data_t max_error);

}; // class Block

} // namespace cncpp
