#include <ingestion/finnhub_http.h>
#include <config/symbols.h>
#include <util/time.h>
#include <util/env.h>
#include <util/secrets.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>

int main() {
  std::cout << util::Time::now_utc_iso8601() << " [info] service=andromeda, action=start" << std::endl;

  // Initialize libcurl globally (thread-safe usage)
  curl_global_init(CURL_GLOBAL_DEFAULT);

  // Read API key from env or secrets/finnhub.env
  const std::string api_key = util::read_api_key("FINNHUB_API_KEY", "secrets/finnhub.env");
  if (api_key.empty()) {
    std::cout << util::Time::now_utc_iso8601() << " [warn] missing_api_key=FINNHUB_API_KEY source=env_or_file skip_http=true" << std::endl;
    curl_global_cleanup();
    return 0;  // allow CI to pass without secrets
  }

  // Resolve symbols from constant config
  const auto& symbols = config::kUSStockSymbols;
  if (symbols.empty()) {
    std::cerr << util::Time::now_utc_iso8601() << " [error] symbols=none reason=no_valid_input" << std::endl;
    curl_global_cleanup();
    return 2;
  }

  // Fetch a snapshot quote from Finnhub API and log raw JSON for each symbol
  try {
    ingestion::FinnhubHttpClient http(api_key);
    for (const auto& sym : symbols) {
      const auto it = config::kUSStockNames.find(sym);
      const std::string& name = (it != config::kUSStockNames.end()) ? it->second : sym;
      try {
        const auto quote_json = http.get_quote(sym);
        std::cout << util::Time::now_utc_iso8601() << " [info] symbol=" << sym << " name=" << name
                  << " quote_json=" << quote_json << std::endl;
      } catch (const std::exception& e) {
        std::cerr << util::Time::now_utc_iso8601() << " [error] symbol=" << sym << " name=" << name
                  << " error=http_request_failed detail=\"" << e.what() << "\"" << std::endl;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << util::Time::now_utc_iso8601() << " [error] error=client_init_failed detail=\"" << e.what() << "\"" << std::endl;
    curl_global_cleanup();
    return 1;
  }

  curl_global_cleanup();
  return 0;
}
