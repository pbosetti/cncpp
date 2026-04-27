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
#include "machine.hpp"
#include <functional>

namespace cncpp {

/**
 * @brief Represents a single parsed G-code block.
 *
 * A block stores one line of G-code together with its decoded motion,
 * geometric parameters, and feed profile. It can be linked with previous and
 * next blocks to form a full program path.
 */
class Block : public Object {
public:
  /**
   * @brief Supported motion type for the block.
   */
  enum class BlockType { RAPID = 0, LINE, CWA, CCWA, NO_MOTION };

  /**
   * @brief Kinematic profile used to execute this block.
   */
  struct Profile {
    /// Acceleration and deceleration magnitudes.
    data_t a = 0.0, d = 0.0;
    /// Peak feed and path length.
    data_t f = 0.0, l = 0.0;
    /// Initial and final feed values.
    data_t fs = 0.0, fe = 0.0;
    /// Phase durations (accel, cruise, decel).
    data_t dt_1 = 0.0, dt_m = 0.0, dt_2 = 0.0;
    /// Total block duration.
    data_t dt = 0.0;
    /// Instantaneous acceleration value, used mainly for arc motion.
    data_t current_acc;

    /**
     * @brief Computes normalized path position over profile time.
     * @param t Elapsed time from the beginning of the block.
     * @param s Output instantaneous speed.
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
   * @brief Creates a block linked to a previous block.
   * @param line Raw block text.
   * @param prev Previous block in the sequence.
   */
  Block(std::string line, Block &prev);

  /**
   * @brief Destroys the block instance.
   */
  ~Block();

  /**
   * @brief Returns a human-readable description of the block.
   * @param colored Enables ANSI color formatting in the output.
   * @return Text description of block state and parameters.
   */
  std::string desc(bool colored = true) const override;

  /**
   * @brief Assigns another block to this one.
   * @param b Source block.
   * @return Reference to this block.
   */
  Block &operator=(Block &b);

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
  /** @brief Returns programmed linear feedrate. */
  data_t feedrate() const { return _feedrate; }
  /** @brief Returns feedrate projected along arc geometry. */
  data_t arc_feedrate() const { return _arc_feedrate; }
  /** @brief Returns spindle speed value. */
  data_t spindle() const { return _spindle; }
  /** @brief Returns geometric path length of the block. */
  data_t length() const { return _length; }
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

  /// Previous block in the linked sequence, or nullptr.
  Block *prev = nullptr;
  /// Next block in the linked sequence, or nullptr.
  Block *next = nullptr;

private:
  std::string _line;                // original G-Code line, eg "N01 G00 X100"
  size_t _n = 0;                    // block number
  Point _target = Point();          // block destination
  Point _center = Point();          // arc center
  Point _delta = Point();           // block projections
  data_t _length = 0;               // block length
  data_t _i = 0, _j = 0, _r = 0;    // arc parameters
  data_t _theta_0 = 0, _dtheta = 0; // arc initial and included angles
  data_t _feedrate = 0;             // feedrate
  data_t _arc_feedrate = 0;         // feedrate along the arc
  data_t _spindle = 0;              // spindle rate
  data_t _acc = 0;                  // actual acceleration
  size_t _m = 0;                    // M command argument
  size_t _tool = 0;                 // current tool number
  BlockType _type = BlockType::NO_MOTION; // type of block
  Profile _profile;                       // feedrate profile
  bool _parsed = false;                   // has it been parsed?
  Machine const *_machine = nullptr;      // machine we're running on

  // UTILITIES =================================================================
  bool parse_token(std::string &token);
  Point start_point();
  void compute();
  void calc_arc();


}; // class Block

} // namespace cncpp