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

#include "defines.hpp"
#include "machine.hpp"
#include "block.hpp"
#include <list>

namespace cncpp {
using block_iterator = std::list<Block>::iterator;

/**
 * @brief Represents a complete G-code program.
 *
 * A program owns an ordered list of parsed Block instances and keeps track of
 * the current block while the program is being traversed. Blocks are parsed
 * against the associated Machine context when they are loaded or appended.
 */
class Program : public Object, public std::list<Block> {
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

  /**
   * @brief Destroys the program instance.
   */
  ~Program();

  /**
   * @brief Returns a human-readable description of all blocks.
   * @param colored Enables ANSI color formatting in the output.
   * @return Text description of the program blocks.
   */
  std::string desc(bool colored = true) const override;

  // OPERATORS/OPERATIONS ======================================================
  /**
   * @brief Loads G-code blocks from a file.
   * @param filename Path to the G-code file to load.
   * @param append If true, appends blocks to the existing program; otherwise
   * clears the current program before loading.
   */
  void load(const std::string &filename, bool append = false);

  /**
   * @brief Appends and parses one raw G-code line.
   * @param line Raw G-code block text.
   * @return Reference to this program.
   */
  Program &operator<<(const std::string &line);

  /**
   * @brief Advances to the next block in the program.
   * @return Iterator pointing to the current block, or end() when complete.
   */
  iterator load_next();

  /**
   * @brief Moves the current block iterator back to the first block.
   */
  void rewind();

  /**
   * @brief Clears all blocks and rewinds program traversal state.
   */
  void reset();

  // ACCESSORS =================================================================
  /** @brief Returns an iterator to the current block. */
  block_iterator current() { return _current; }

  /** @brief Returns true when program traversal has reached the end. */
  bool done() { return _done; };

  private:
  Machine *_machine = nullptr;
  std::string _filename = "";
  iterator _current = this->end();
  bool _done = false;
};



} // namespace cncpp
