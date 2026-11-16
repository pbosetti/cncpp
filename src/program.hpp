/*
  ____                                             _
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___     ___| | __ _ ___ ___
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \   / __| |/ _` / __/ __|
 |  __/| | | (_) | (_| | | | (_| | | | | | | | (__| | (_| \__ \__ \
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|  \___|_|\__,_|___/___/
                  |___/
*/

#pragma once

#include "block.hpp"
#include "defines.hpp"
#include "machine.hpp"
#include "point.hpp"
#include <list>

namespace cncpp {

/**
 * @brief A program is a sequence of blocks that can be executed by a machine.
 * 
 * A program is a sequence of blocks that can be executed by a machine. It manages the loading, parsing, and execution of G-code lines, tracking the current block being processed. The program can be loaded from a file or constructed line by line, and it provides methods to navigate through the blocks, reset the program, loop over the blocks with an iterator.
 */
class Program : public Object {
public:
  // Lifecicle: ================================================================

  /**
   * @brief Construct a new Program object form a file and a Machine pointer.
   * 
   * @param filename Path to a file containing G-code lines.
   * @param machine A pointer to the machine that will execute the program.
   */
  Program(const std::string &filename, Machine *machine);

  /**
   * @brief Construct an new empty Program object with a Machine pointer.
   * 
   * @param machine A pointer to the machine that will execute the program.
   */
  Program(Machine *machine);

  // A program cannot be copied or assigned, as it manages a sequence of blocks and a machine pointer. Copying a program would lead to undefined behavior, as the blocks and machine pointer would be shared between the copies. Therefore, the copy constructor and assignment operator are deleted.
  Program(const Program &) = delete;
  Program &operator=(const Program &) = delete;

  /**
   * @brief Provide a human-readable description of the program.
   * 
   * The program is describet as a sequence of blocks, routinely calling the desc() method of each block. The output can be colored or not, depending on the parameter.
   * @param colored 
   * @return std::string 
   */
  std::string desc(bool colored = true) const override;

  // Operations: ===============================================================

  /**
   * @brief Load a program from a file.
   * 
   * @param filename the filename of the file to load the program from.
   * @param append If true, the program will be appended to the existing blocks; if false, the program will be reset before loading.
   * @throws std::runtime_error if the file cannot be opened.
   */
  void load(const std::string &filename, bool append = false);

  /**
   * @brief Load a new G-code line and creates the corresponding block
   * 
   * The new block is created (via Program::insert) from line and appended to the end of the program.
   * 
   * @param line A line of G-code
   * @return Program& 
   */
  Program &operator<<(const std::string &line);

  /**
   * @brief Insert a new block at the specified index, the reparse the program from that block onward.
   * 
   * If reparsing fails due to the newly inserted block altering the next block, the newly inserted block is removed and the program is reparsed from the next block onward, throwing the parsing error.
   * 
   * @param index The index at which to insert the new block.
   * @param line A line of G-code to create the new block from.
   * @return Block& A reference to the newly inserted block.
   * @throws std::out_of_range if the index is out of bounds.
   * @throws std::runtime_error if the block cannot be parsed.
   */
  Block &insert(size_t index, const std::string &line);

  /**
   * @brief Load the next block and make it active.
   * 
   * If the program has not been started, the first block is loaded. If the program has been started, the next block is loaded. If there are no more blocks, the current block is set to nullptr.
   * 
   * @return Block& 
   */
  Block &load_next();

  /**
   * @brief Rewind the program to the beginning.
   */
  void rewind();

  /**
   * @brief Reset the program to its initial state.
   * 
   * This will delete all blocks and reset the current block to nullptr. 
   */
  void reset();

  // Accessors: ================================================================
  /**
   * @brief The block being loaded and executed by the machine. If the program has not been started, or if it has finished, this will be nullptr.
   * 
   * @return Block* 
   */
  Block *current() const { return _current; }

  /**
   * @brief The number of blocks in the program.
   * 
   * @return size_t 
   */
  size_t size() const { return _blocks.size(); }

  /**
   * @brief Indicates whether the program has finished executing.
   * 
   * @return bool 
   */
  bool done() const { return _started && !_current; }

  // Iterators: ================================================================
  std::list<Block>::iterator begin() { return _blocks.begin(); }
  std::list<Block>::iterator end() { return _blocks.end(); }
  std::list<Block>::const_iterator begin() const { return _blocks.begin(); }
  std::list<Block>::const_iterator end() const { return _blocks.end(); }

private:
  Machine *_machine = nullptr;
  std::string _filename = "";
  std::list<Block> _blocks;
  Block *_current = nullptr;
  bool _started = false;

  /**
   * @brief Parse again all blocks from the given block onward.
   * 
   * @param b The block from which to start re-parsing (included).
   */
  void reparse_from(Block *b);
};

} // namespace cncpp