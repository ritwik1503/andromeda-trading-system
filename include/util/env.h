#pragma once

#include <cstddef>
#include <cstdlib>
#include <string>

namespace util {

// Reads an environment variable as a bounded integer.
// - Returns 'def' if missing or invalid.
// - Clamps to [min_v, max_v].
inline std::size_t env_size_t(const char* name, std::size_t def,
                              std::size_t min_v, std::size_t max_v) {
  if (const char* v = std::getenv(name)) {
    try {
      long long n = std::stoll(v);
      if (n < static_cast<long long>(min_v)) return min_v;
      if (n > static_cast<long long>(max_v)) return max_v;
      return static_cast<std::size_t>(n);
    } catch (...) {
      return def;
    }
  }
  return def;
}

}  // namespace util

