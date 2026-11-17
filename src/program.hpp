/*
  ____                                             _
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___     ___| | __ _ ___ ___
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \   / __| |/ _` / __/ __|
 |  __/| | | (_) | (_| | | | (_| | | | | | | | (__| | (_| \__ \__ \
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|  \___|_|\__,_|___/___/
                  |___/
*/

/* AGENT INSTRUCTIONS
Pretending the program.cpp does not exist, create the implementation of the 
Program class hereafter desribed in the _program.cpp file. Do not edit 
anything else.
Using the provided test harness (Catch2) implement unit tests for the methods
that have a @test tag in documentation comments.
At the end of _program.cpp create a main() function 
(protected by CNCPP_TEST_PROGRAM macro) that does the following:
- requires exactly one command line argument: the filename of the G-code program to load
- creates a Machine object with default parameters
- creates a Program object with the Machine pointer
- loads the program from the file, dealing with errors
- for each block that contains a linear or arc interpolation loop over it using 
  the Block::walk function, printing total time, block time, block number, lambda, speed and interpolated point coordinates in CSV format to stdout
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
   * @test The program can be constructed empty and then loaded from a file or constructed line by line.
   * @test a program with an error in the middle must throw an exception and allow printing its description up to the error block, excluded
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
   * 
   * @test an empty program returns an empty string
   * @test a program with blocks returns a string with the description of each block, one per line
   * @test a program with blocks returns a string with the description of each block, one per line, with or without color depending on the parameter
   * 
   * @param colored 
   * @return std::string 
   */
  std::string desc(bool colored = true) const override;

  // Operations: ===============================================================

  /**
   * @brief Load a program from a file.
   * 
   * @test The program can be loaded from a file, and the blocks are created in the order they appear in the file.
   * @test A second program can be appended to the first, provided that the line numbers are increasing. If not we must throw an error.
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
   * @test add a line to an empty program, the block is created and can be described
   * @test add a line to a non-empty program, the block is created and can be described
   * @param line A line of G-code
   * @return Program& 
   */
  Program &operator<<(const std::string &line);

  /**
   * @brief Insert a new block at the specified index, the reparse the program from that block onward.
   * 
   * If reparsing fails due to the newly inserted block altering the next block, the newly inserted block is removed and the program is reparsed from the next block onward, throwing the parsing error.
   * 
   * @test insert a block at the beginning of the program, the block is created and can be described
   * @test insert a block in the middle of the program, the block is created and can be described
   * @test insert a wrong block at the end of the program, the block is discarded and the program can be printed up to the previous block, and the parsing error is thrown
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
   * @test if the program is empty, not started, or done, must return nullptr
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