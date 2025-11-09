#pragma once

#include <string>

namespace ingestion {

// Minimal HTTP client for Finnhub REST endpoints.
class FinnhubHttpClient {
 public:
  explicit FinnhubHttpClient(std::string api_key,
                             std::string base_url = "https://finnhub.io/api/v1");

  // Calls GET /quote?symbol=SYMBOL&token=API_KEY
  // Returns the raw JSON response string (e.g., {"c":..., "h":..., ...}).
  // Throws std::runtime_error on HTTP/network errors.
  std::string get_quote(const std::string& symbol) const;

 private:
  std::string api_key_;
  std::string base_url_;
};

}  // namespace ingestion

