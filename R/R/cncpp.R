Machine <- function(filename = NULL) {
  cncpp_machine_create(.nullable_string(filename, "filename"))
}

machine_desc <- function(machine, colored = FALSE) {
  cncpp_machine_desc(machine, isTRUE(colored))
}

machine_load <- function(machine, filename) {
  cncpp_machine_load(machine, .required_string(filename, "filename"))
  invisible(machine)
}

machine_quantize <- function(machine, time) {
  cncpp_machine_quantize(machine, .required_scalar(time, "time"))
}

machine_A <- function(machine) {
  cncpp_machine_A(machine)
}

machine_tq <- function(machine) {
  cncpp_machine_tq(machine)
}

machine_fmax <- function(machine) {
  cncpp_machine_fmax(machine)
}

machine_error <- function(machine) {
  cncpp_machine_error(machine)
}

machine_max_error <- function(machine) {
  cncpp_machine_max_error(machine)
}

machine_zero <- function(machine) {
  cncpp_machine_zero(machine)
}

machine_offset <- function(machine) {
  cncpp_machine_offset(machine)
}

machine_setpoint <- function(machine) {
  cncpp_machine_setpoint(machine)
}

machine_position <- function(machine) {
  cncpp_machine_position(machine)
}

machine_set_setpoint <- function(machine, point = NULL, x = NULL, y = NULL,
                                 z = NULL) {
  if (!is.null(point)) {
    cncpp_machine_set_setpoint(machine, point)
  } else {
    cncpp_machine_set_setpoint_xyz(machine, .required_scalar(x, "x"),
                                   .required_scalar(y, "y"),
                                   .required_scalar(z, "z"))
  }
  invisible(machine)
}

machine_set_position <- function(machine, point) {
  cncpp_machine_set_position(machine, point)
  invisible(machine)
}

machine_summary <- function(machine) {
  cncpp_machine_summary(machine)
}

Point <- function(x = NULL, y = NULL, z = NULL) {
  cncpp_point_create(.nullable_scalar(x), .nullable_scalar(y),
                     .nullable_scalar(z))
}

point_desc <- function(point, colored = FALSE) {
  cncpp_point_desc(point, isTRUE(colored))
}

point_is_complete <- function(point) {
  cncpp_point_is_complete(point)
}

point_reset <- function(point) {
  cncpp_point_reset(point)
  invisible(point)
}

point_x <- function(point) {
  cncpp_point_x(point)
}

point_y <- function(point) {
  cncpp_point_y(point)
}

point_z <- function(point) {
  cncpp_point_z(point)
}

point_set <- function(point, x = NULL, y = NULL, z = NULL) {
  if (!is.null(x)) {
    cncpp_point_set_x(point, .required_scalar(x, "x"))
  }
  if (!is.null(y)) {
    cncpp_point_set_y(point, .required_scalar(y, "y"))
  }
  if (!is.null(z)) {
    cncpp_point_set_z(point, .required_scalar(z, "z"))
  }
  invisible(point)
}

point_vec <- function(point) {
  cncpp_point_vec(point)
}

point_length <- function(point) {
  cncpp_point_length(point)
}

point_delta <- function(point, other) {
  cncpp_point_delta(point, other)
}

point_add <- function(point, other) {
  cncpp_point_add(point, other)
}

point_modal <- function(point, other) {
  cncpp_point_modal(point, other)
  invisible(point)
}

Block <- function(line, previous = NULL) {
  line <- as.character(line)
  if (length(line) != 1L || is.na(line)) {
    stop("line must be a non-missing character scalar", call. = FALSE)
  }
  cncpp_block_create(line, previous)
}

block_parse <- function(block) {
  cncpp_block_parse(block)
  invisible(block)
}

block_desc <- function(block, colored = FALSE) {
  cncpp_block_desc(block, isTRUE(colored))
}

block_line <- function(block) {
  cncpp_block_line(block)
}

block_n <- function(block) {
  cncpp_block_n(block)
}

block_dt <- function(block) {
  cncpp_block_dt(block)
}

block_type <- function(block) {
  cncpp_block_type(block)
}

block_tool <- function(block) {
  cncpp_block_tool(block)
}

block_feedrate <- function(block) {
  cncpp_block_feedrate(block)
}

block_arc_feedrate <- function(block) {
  cncpp_block_arc_feedrate(block)
}

block_spindle <- function(block) {
  cncpp_block_spindle(block)
}

block_length <- function(block) {
  cncpp_block_length(block)
}

block_target <- function(block) {
  cncpp_block_target(block)
}

block_center <- function(block) {
  cncpp_block_center(block)
}

block_delta <- function(block) {
  cncpp_block_delta(block)
}

block_m <- function(block) {
  cncpp_block_m(block)
}

block_profile <- function(block) {
  cncpp_block_profile(block)
}

block_parsed <- function(block) {
  cncpp_block_parsed(block)
}

block_r <- function(block) {
  cncpp_block_r(block)
}

block_lambda <- function(block, time) {
  cncpp_block_lambda(block, .required_scalar(time, "time"))
}

block_interpolate_lambda <- function(block, lambda) {
  cncpp_block_interpolate_lambda(block, .required_scalar(lambda, "lambda"))
}

block_interpolate_time <- function(block, time) {
  cncpp_block_interpolate_time(block, .required_scalar(time, "time"))
}

block_walk <- function(block) {
  cncpp_block_walk(block)
}

block_summary <- function(block) {
  cncpp_block_summary(block)
}

Program <- function(filename = NULL, machine = NULL) {
  cncpp_program_create(.nullable_string(filename, "filename"), machine)
}

program_desc <- function(program, colored = FALSE) {
  cncpp_program_desc(program, isTRUE(colored))
}

program_load <- function(program, filename, append = FALSE) {
  cncpp_program_load(program, .required_string(filename, "filename"),
                     isTRUE(append))
  invisible(program)
}

program_append <- function(program, lines) {
  lines <- as.character(lines)
  if (anyNA(lines)) {
    stop("lines must not contain missing values", call. = FALSE)
  }
  cncpp_program_append(program, lines)
  invisible(program)
}

program_reset <- function(program) {
  cncpp_program_reset(program)
  invisible(program)
}

program_rewind <- function(program) {
  cncpp_program_rewind(program)
  invisible(program)
}

program_size <- function(program) {
  cncpp_program_size(program)
}

program_done <- function(program) {
  cncpp_program_done(program)
}

program_lines <- function(program) {
  cncpp_program_lines(program)
}

program_blocks <- function(program) {
  cncpp_program_blocks(program)
}

program_summary <- function(program) {
  cncpp_program_summary(program)
}

program_current <- function(program) {
  cncpp_program_current(program)
}

program_load_next <- function(program) {
  cncpp_program_load_next(program)
}

program_walk <- function(program) {
  cncpp_program_walk(program)
}

print.cncpp_machine <- function(x, ...) {
  cat(machine_desc(x, colored = FALSE), "\n", sep = "")
  invisible(x)
}

print.cncpp_point <- function(x, ...) {
  cat(point_desc(x, colored = FALSE), "\n", sep = "")
  invisible(x)
}

print.cncpp_block <- function(x, ...) {
  line <- tryCatch(block_desc(x, colored = FALSE), error = function(e) {
    paste0("<cncpp_block: ", block_line(x), " (unparsed)>")
  })
  cat(line, "\n", sep = "")
  invisible(x)
}

print.cncpp_program <- function(x, ...) {
  cat(program_desc(x, colored = FALSE), sep = "")
  invisible(x)
}

.nullable_scalar <- function(value) {
  if (is.null(value) || length(value) == 0L || is.na(value[[1L]])) {
    return(NULL)
  }
  .required_scalar(value, "value")
}

.required_scalar <- function(value, name) {
  if (!is.numeric(value) || length(value) != 1L || is.na(value)) {
    stop(name, " must be a non-missing numeric scalar", call. = FALSE)
  }
  as.numeric(value)
}

.nullable_string <- function(value, name) {
  if (is.null(value)) {
    return(NULL)
  }
  .required_string(value, name)
}

.required_string <- function(value, name) {
  value <- as.character(value)
  if (length(value) != 1L || is.na(value)) {
    stop(name, " must be a non-missing character scalar", call. = FALSE)
  }
  value
}
