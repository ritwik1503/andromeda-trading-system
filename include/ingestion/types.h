#pragma once

#include <string>
#include <vector>

namespace ingestion {

  // Generic trade event used across ingestion sources
  struct Trade {
    std::string symbol;
    double price{};
    double volume{};
    long long ts_ms{};                       // UNIX ms
    std::vector<std::string> conditions;     // optional
  };

} 
