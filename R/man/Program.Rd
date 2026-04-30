\name{Program}
\alias{Program}
\alias{program_desc}
\alias{program_load}
\alias{program_append}
\alias{program_reset}
\alias{program_rewind}
\alias{program_size}
\alias{program_done}
\alias{program_lines}
\alias{program_blocks}
\alias{program_summary}
\alias{program_current}
\alias{program_load_next}
\alias{program_walk}
\title{Program Wrapper}
\usage{
Program(filename = NULL, machine = NULL)

program_desc(program, colored = FALSE)

program_load(program, filename, append = FALSE)

program_append(program, lines)

program_reset(program)

program_rewind(program)

program_size(program)

program_done(program)

program_lines(program)

program_blocks(program)

program_summary(program)

program_current(program)

program_load_next(program)

program_walk(program)
}
\arguments{
\item{filename}{Optional path to a G-code program file.}

\item{machine}{Optional \code{cncpp_machine} external pointer. If omitted, a
new machine is created for the program.}

\item{program}{A \code{cncpp_program} external pointer returned by
\code{Program()}.}

\item{colored}{Logical flag. If \code{TRUE}, descriptions may include ANSI
color escape sequences.}

\item{append}{Logical flag. If \code{TRUE}, loaded blocks are appended to the
current program. If \code{FALSE}, the program is cleared before loading.}

\item{lines}{A character vector of G-code block lines to append and parse.}
}
\value{
\code{Program()} returns a \code{cncpp_program} external pointer. Mutating
functions return the input program invisibly. Accessors return character
vectors, lists, data frames, logical values, sizes, or \code{cncpp_block}
objects.
}
\description{
Create, load, traverse, and summarize a C++ \code{cncpp::Program} object from R.
}
\details{
A program owns an ordered sequence of parsed G-code blocks and a machine context.
Blocks can be loaded from a file or appended from R character vectors. Traversal
uses \code{program_load_next()}, \code{program_current()},
\code{program_done()}, and \code{program_rewind()}.

\code{program_blocks()} returns regular \code{cncpp_block} external pointers
rebuilt from the parsed program lines, preserving the block sequence context.
\code{program_walk()} samples every interpolatable motion block and returns a
data frame of positions and kinematics.
}
\examples{
machine <- Machine()
program <- Program(machine = machine)
program_append(program, c("N10 G00 X0 Y0 Z0", "N20 G01 X1 Y0 Z0 F100"))

program_size(program)
program_lines(program)
program_summary(program)

first <- program_load_next(program)
block_line(first)
program_rewind(program)

head(program_walk(program))
}
