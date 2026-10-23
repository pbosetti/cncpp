/*
  ____                                             _               
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___     ___| | __ _ ___ ___ 
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \   / __| |/ _` / __/ __|
 |  __/| | | (_) | (_| | | | (_| | | | | | | | (__| | (_| \__ \__ \
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|  \___|_|\__,_|___/___/
                  |___/                                            

Class representing a G-code program, as a list of Block instances
*/

#pragma once

#include "block.hpp"
#include "defines.hpp"
#include "machine.hpp"
#include <list>

namespace cncpp {

/**
 * @brief Represents a complete G-code program.
 *
 * A program owns an ordered list of parsed and linked Block instances and
 * keeps track of the current block while the program is being traversed.
 * Blocks are parsed against the associated Machine context when they are
 * loaded, appended or inserted.
 *
 * A program cannot be copied, since its blocks point at each other.
 */
class Program : public Object {
public:
  // LIFECYCLE =================================================================
  /**
   * @brief Creates a program and loads G-code from a file.
   * @param filename Path to the G-code file to load.
   * @param machine Machine context used to parse program blocks.
   */
  Program(const std::string &filename, Machine *machine);

  /**
   * @brief Creates an empty program for the given machine context.
   * @param machine Machine context used to parse appended or loaded blocks.
   */
  Program(Machine *machine);

  Program(const Program &) = delete;
  Program &operator=(const Program &) = delete;

  /**
   * @brief Returns a human-readable description of all blocks.
   * @param colored Enables ANSI color formatting in the output.
   * @return Text description of the program blocks.
   */
  std::string desc(bool colored = true) const override;

  // OPERATORS/OPERATIONS ======================================================
  /**
   * @brief Loads G-code blocks from a file.
   *
   * Blank lines and lines starting with `#` or `;` are skipped.
   * @param filename Path to the G-code file to load.
   * @param append If true, appends blocks to the existing program; otherwise
   * clears the current program before loading.
   */
  void load(const std::string &filename, bool append = false);

  /**
   * @brief Appends and parses one raw G-code line.
   *
   * If parsing fails, the block is not appended.
   * @param line Raw G-code block text.
   * @return Reference to this program.
   */
  Program &operator<<(const std::string &line);

  /**
   * @brief Inserts and parses one raw G-code line before the given block.
   *
   * All the following blocks are parsed again, so that they inherit the
   * new start point and modal state. If parsing fails (including a block
   * number that is no longer increasing), the program is left unchanged and
   * the exception is rethrown.
   * @param index Position of the new block (0 = first, size() = append).
   * @param line Raw G-code block text.
   * @return Reference to the inserted block.
   */
  Block &insert(size_t index, const std::string &line);

  /**
   * @brief Advances to the next block in the program.
   *
   * Once the end is reached the program stays done until rewind() is called.
   * @return Pointer to the current block, or nullptr when complete.
   */
  Block *load_next();

  /**
   * @brief Moves the traversal back to before the first block.
   */
  void rewind();

  /**
   * @brief Clears all blocks and rewinds program traversal state.
   */
  void reset();

  // ACCESSORS =================================================================
  /** @brief Returns an iterator to the first block. */
  std::list<Block>::iterator begin() { return _blocks.begin(); }
  /** @brief Returns an iterator past the last block. */
  std::list<Block>::iterator end() { return _blocks.end(); }
  /** @brief Returns a const iterator to the first block. */
  std::list<Block>::const_iterator begin() const { return _blocks.begin(); }
  /** @brief Returns a const iterator past the last block. */
  std::list<Block>::const_iterator end() const { return _blocks.end(); }

  /**
   * @brief Returns the current block.
   * @return Pointer to the current block, or nullptr before the first
   * load_next() and after the end.
   */
  Block *current() const { return _current; }

  /** @brief Returns true when program traversal has reached the end. */
  bool done() const { return _started && !_current; }

  /** @brief Returns the number of blocks. */
  size_t size() const { return _blocks.size(); }

private:
  Machine *_machine = nullptr;
  std::string _filename;
  // std::list never moves its elements, so the blocks' links stay valid
  std::list<Block> _blocks;
  Block *_current = nullptr; // current block during traversal
  bool _started = false;     // has load_next() been called since rewind()?

  // Parses again all the blocks from b to the end of the program
  void reparse_from(Block *b);
};

} // namespace cncpp
