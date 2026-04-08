#pragma once

#include <vector>

using Vec = std::vector<double>;

template <typename T>
using s_ptr = std::shared_ptr<T>;
// usage: s_ptr<double>
