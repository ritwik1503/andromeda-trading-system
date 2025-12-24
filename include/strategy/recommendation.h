#pragma once

#include <string>
#include <vector>

namespace strategy {

// Finnhub recommendation trend item for a symbol/period.
struct RecommendationItem {
  int buy{0};
  int hold{0};
  int sell{0};
  int strongBuy{0};
  int strongSell{0};
  std::string period;  // e.g. "2025-03-01"
  std::string symbol;  // e.g. "AAPL"
};

// Minimal client for Finnhub Recommendation Trends API.
class FinnhubRecommendationClient {
 public:
  explicit FinnhubRecommendationClient(std::string api_key,
                                       std::string base_url = "https://finnhub.io/api/v1");

  // Returns parsed vector of recommendation items.
  std::vector<RecommendationItem> get_recommendations(const std::string& symbol) const;

  // Returns raw JSON array string as provided by the API.
  std::string get_recommendations_raw(const std::string& symbol) const;

 private:
  std::string api_key_;
  std::string base_url_;
};

}  // namespace strategy

