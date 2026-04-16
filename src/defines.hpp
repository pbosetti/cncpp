/*
Common defines file
Author: Paolo Bosetti, 2026

*/

#pragma once

#include <optional>

// Semantic versioning
#define CNCPP_VERSION "0.1.0"

#define CNCPP_NUMBERS_WIDTH "9"


using data_t = double; // Type for data values
using opt_data_t = std::optional<data_t>;
using opt_int_t = std::optional<int>;

