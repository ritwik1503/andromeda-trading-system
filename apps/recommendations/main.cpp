#include <strategy/recommendation.h>
#include <config/symbols.h>
#include <util/time.h>
#include <util/secrets.h>

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>

namespace {
bool is_buy(const strategy::RecommendationItem& r) {
  // Simple heuristic: weighted buys exceed weighted sells.
  const int buy_score = r.strongBuy * 2 + r.buy;
  const int sell_score = r.strongSell * 2 + r.sell;
  return buy_score > sell_score;
}
}

int main() {
  std::cout << util::Time::now_utc_iso8601() << " [info] service=recommendations action=start" << std::endl;

  const std::string api_key = util::read_api_key("FINNHUB_API_KEY", "secrets/finnhub.env");
  if (api_key.empty()) {
    std::cout << util::Time::now_utc_iso8601()
              << " [warn] missing_api_key=FINNHUB_API_KEY source=env_or_file skip_http=true" << std::endl;
    return 0;
  }

  strategy::FinnhubRecommendationClient client(api_key);

  for (const auto& sym : config::kUSStockSymbols) {
    const auto it = config::kUSStockNames.find(sym);
    const std::string& name = (it != config::kUSStockNames.end()) ? it->second : sym;
    try {
      const auto items = client.get_recommendations(sym);
      if (items.empty()) {
        std::cerr << util::Time::now_utc_iso8601() << " [warn] symbol=" << sym << " name=" << name
                  << " error=no_data" << std::endl;
        continue;
      }
      // Grouped, readable output: header with name, followed by last 3 months
      const std::size_t n = std::min<std::size_t>(3, items.size());
      std::cout << util::Time::now_utc_iso8601() << " [info] symbol=" << sym << " name=" << name
                << " months=" << n << std::endl;
      for (std::size_t i = 0; i < n; ++i) {
        const auto& r = items[i];
        const bool buy = is_buy(r);
        std::cout << "  - period=" << r.period
                  << "; decision=" << (buy ? "BUY" : "NO-BUY")
                  << "; strong buy=" << r.strongBuy
                  << "; buy=" << r.buy
                  << "; hold=" << r.hold
                  << "; sell=" << r.sell
                  << "; strong sell=" << r.strongSell
                  << std::endl;
      }
      std::cout << std::endl;
    } catch (const std::exception& e) {
      std::cerr << util::Time::now_utc_iso8601() << " [error] symbol=" << sym << " name=" << name
                << " error=recommendation_request_failed detail=\"" << e.what() << "\"" << std::endl;
    }
  }

  return 0;
}
