\name{Machine}
\alias{Machine}
\alias{machine_desc}
\alias{machine_load}
\alias{machine_quantize}
\alias{machine_A}
\alias{machine_tq}
\alias{machine_fmax}
\alias{machine_error}
\alias{machine_max_error}
\alias{machine_zero}
\alias{machine_offset}
\alias{machine_setpoint}
\alias{machine_position}
\alias{machine_set_setpoint}
\alias{machine_set_position}
\alias{machine_summary}
\title{Machine Wrapper}
\usage{
Machine(filename = NULL)

machine_desc(machine, colored = FALSE)

machine_load(machine, filename)

machine_quantize(machine, time)

machine_A(machine)

machine_tq(machine)

machine_fmax(machine)

machine_error(machine)

machine_max_error(machine)

machine_zero(machine)

machine_offset(machine)

machine_setpoint(machine)

machine_position(machine)

machine_set_setpoint(machine, point = NULL, x = NULL, y = NULL, z = NULL)

machine_set_position(machine, point)

machine_summary(machine)
}
\arguments{
\item{filename}{Optional path to a machine configuration file.}

\item{machine}{A \code{cncpp_machine} external pointer returned by
\code{Machine()}.}

\item{colored}{Logical flag. If \code{TRUE}, descriptions may include ANSI
color escape sequences.}

\item{time}{A non-missing numeric scalar time value.}

\item{point}{A \code{cncpp_point} external pointer.}

\item{x, y, z}{Non-missing numeric scalar coordinates used when setting the
machine setpoint without a \code{point} object.}
}
\value{
\code{Machine()} returns a \code{cncpp_machine} external pointer. Mutating
functions return the input machine invisibly. Accessor functions return numeric
values, \code{cncpp_point} objects, or lists describing the machine state.
}
\description{
Create and inspect a C++ \code{cncpp::Machine} object from R.
}
\details{
The machine object stores the CNC kernel settings and runtime state used while
parsing blocks and programs. It is held by an R external pointer; callers should
treat it as an opaque object and use the wrapper functions to inspect or modify
it.
}
\examples{
machine <- Machine()
machine_tq(machine)
machine_quantize(machine, 0.0012)

target <- Point(1, 2, 3)
machine_set_setpoint(machine, target)
machine_set_position(machine, target)
point_vec(machine_setpoint(machine))
machine_summary(machine)
}
