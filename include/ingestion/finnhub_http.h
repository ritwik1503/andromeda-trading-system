#pragma once

#include <curl/curl.h>

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

  // Calls GET /stock/symbol?exchange=EXCHANGE[&mic=...][&securityType=...][&currency=...]&token=API_KEY
  // Returns the raw JSON array response string.
  // Throws std::runtime_error on invalid args or HTTP/network errors.
  std::string get_stock_symbols(const std::string& exchange,
                                const std::string& mic = {},
                                const std::string& security_type = {},
                                const std::string& currency = {}) const;

 private:
  std::string perform_get(CURL* curl, const std::string& url) const;

  std::string api_key_;
  std::string base_url_;
};

}  // namespace ingestion
