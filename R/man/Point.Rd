\name{Point}
\alias{Point}
\alias{point_desc}
\alias{point_is_complete}
\alias{point_reset}
\alias{point_x}
\alias{point_y}
\alias{point_z}
\alias{point_set}
\alias{point_vec}
\alias{point_length}
\alias{point_delta}
\alias{point_add}
\alias{point_modal}
\title{Point Wrapper}
\usage{
Point(x = NULL, y = NULL, z = NULL)

point_desc(point, colored = FALSE)

point_is_complete(point)

point_reset(point)

point_x(point)

point_y(point)

point_z(point)

point_set(point, x = NULL, y = NULL, z = NULL)

point_vec(point)

point_length(point)

point_delta(point, other)

point_add(point, other)

point_modal(point, other)
}
\arguments{
\item{x, y, z}{Optional numeric scalar coordinates. Missing or \code{NULL}
values create an unset coordinate.}

\item{point}{A \code{cncpp_point} external pointer returned by \code{Point()}.}

\item{colored}{Logical flag. If \code{TRUE}, descriptions may include ANSI
color escape sequences.}

\item{other}{Another \code{cncpp_point} external pointer.}
}
\value{
\code{Point()} and functions that create a new point return a
\code{cncpp_point} external pointer. Accessors return numeric values, logical
values, character descriptions, or named numeric vectors. Mutating functions
return the input point invisibly.
}
\description{
Create and manipulate a C++ \code{cncpp::Point} object from R.
}
\details{
A point stores optional x, y, and z coordinates. It is held by an R external
pointer; use the wrapper functions for coordinate access, modal updates, vector
conversion, and point arithmetic.
}
\examples{
p <- Point(1, 2, 3)
point_vec(p)
point_length(p)

q <- Point(4, 2, 3)
point_vec(point_delta(q, p))
point_set(p, z = 10)
point_vec(p)
}
