# Measuring time intervals in C++ with `chrono`

The given C++ program demonstrates high-resolution time measurement using the `chrono` library, focusing on timing intervals and analyzing execution performance. The program includes three timing modes:

1. **Free run** without any delays.
2. **Free run with a fixed period**, introducing a sleep delay.
3. **Free run with alarm signals**, utilizing `setitimer` and `SIGALRM`.

This document provides an introduction to the `chrono` library and a breakdown of key parts of the code.

## The `chrono` library

The `<chrono>` library is a part of the C++ standard library that provides functionality for measuring time durations, timestamps, and clock operations. It includes:

- **Clocks** (`steady_clock`, `system_clock`, `high_resolution_clock`)
- **Durations** (time intervals)
- **Time points** (specific times from a clock)

### Key `chrono` functions in this code

1. **`high_resolution_clock::now()`**: retrieves the current time with the highest available precision.
2. **`duration` and `to_ms` alias**: the `duration<double, ratio<1, 1000>>` alias is used to convert nanoseconds to milliseconds.
3. **`sleep_for(milliseconds(period_ms))`**: pauses execution for a given duration.

## Code breakdown

### `free_run` template function

This function records timestamps at every iteration and computes time differences. The template parameters allow it to be customized:
- `period_ms`: Specifies sleep duration between iterations.
- `alrm`: If `true`, uses `setitimer` signals instead of `sleep_for`.

Key operations:
- Uses `high_resolution_clock::now()` to get timestamps.
- Computes elapsed time and time since the last iteration.
- Stores time deltas in a vector.
- Uses `if constexpr` for compile-time conditional logic.

### `stats` function

Calculates and prints basic statistics (mean and standard deviation) of the time deltas recorded in `free_run`.

### `main` function

- Parses the number of iterations from command-line arguments.
- Runs `free_run` three times with different configurations.
- Uses `setitimer` to trigger periodic signals when `alrm` is enabled.
- Outputs time statistics.
