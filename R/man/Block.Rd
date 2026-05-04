\name{Block}
\alias{Block}
\alias{block_parse}
\alias{block_desc}
\alias{block_line}
\alias{block_n}
\alias{block_dt}
\alias{block_type}
\alias{block_tool}
\alias{block_feedrate}
\alias{block_arc_feedrate}
\alias{block_spindle}
\alias{block_length}
\alias{block_target}
\alias{block_center}
\alias{block_delta}
\alias{block_m}
\alias{block_profile}
\alias{block_parsed}
\alias{block_r}
\alias{block_lambda}
\alias{block_interpolate_lambda}
\alias{block_interpolate_time}
\alias{block_walk}
\alias{block_summary}
\title{Block Wrapper}
\usage{
Block(line, previous = NULL)

block_parse(block)

block_desc(block, colored = FALSE)

block_line(block)

block_n(block)

block_dt(block)

block_type(block)

block_tool(block)

block_feedrate(block)

block_arc_feedrate(block)

block_spindle(block)

block_length(block)

block_target(block)

block_center(block)

block_delta(block)

block_m(block)

block_profile(block)

block_parsed(block)

block_r(block)

block_lambda(block, time)

block_interpolate_lambda(block, lambda)

block_interpolate_time(block, time)

block_walk(block)

block_summary(block)
}
\arguments{
\item{line}{A non-missing character scalar containing one G-code block.}

\item{previous}{Optional \code{cncpp_block} external pointer used to link this
block to the previous parsed block in a sequence.}

\item{block}{A \code{cncpp_block} external pointer returned by \code{Block()}
or by program traversal helpers.}

\item{colored}{Logical flag. If \code{TRUE}, descriptions may include ANSI
color escape sequences.}

\item{time}{A non-missing numeric scalar elapsed time within the block.}

\item{lambda}{A non-missing numeric scalar normalized path coordinate.}
}
\value{
\code{Block()} returns a \code{cncpp_block} external pointer. Accessors return
numeric values, logical values, character strings, lists, data frames, or
\code{cncpp_point} objects depending on the requested block property. Mutating
functions return the input block invisibly.
}
\description{
Create, parse, inspect, and interpolate a C++ \code{cncpp::Block} object from R.
}
\details{
A block wraps one G-code line. Calling \code{block_parse()} decodes the line
using the machine context stored with the block. Blocks can be linked through
the \code{previous} argument so modal coordinates and interpolation have the
same context as a program sequence.

Interpolation helpers are valid for parsed motion blocks of type \code{LINE},
\code{CWA}, or \code{CCWA}. A linked previous block is required for
interpolation.
}
\examples{
b0 <- Block("N10 G00 X0 Y0 Z0")
block_parse(b0)

b1 <- Block("N20 G01 X1 Y0 Z0 F100", previous = b0)
block_parse(b1)
block_summary(b1)
block_type(b1)

head(block_walk(b1))
}
