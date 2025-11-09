#pragma once

#include <string>
#include <chrono>
#include <ctime>
#include <cstdio>

namespace util {

// Lightweight time utilities for logging and timestamps.
class Time {
 public:
  // Returns current UTC timestamp in ISO-8601-like format: YYYY-MM-DDTHH:MM:SSZ
  static inline std::string now_utc_iso8601() {
    using namespace std::chrono;
    const auto now = system_clock::now();
    const auto t = system_clock::to_time_t(now);
    std::tm tm{};
#if defined(_WIN32)
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02dZ",
                  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                  tm.tm_hour, tm.tm_min, tm.tm_sec);
    return std::string(buf);
  }
};

}  // namespace util

