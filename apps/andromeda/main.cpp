#include <ingestion/finnhub_ws.h>
#include <ingestion/finnhub_http.h>
#include <config/symbols.h>
#include <util/time.h>
#include <util/env.h>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <curl/curl.h>

int main() {
  std::cout << util::Time::now_utc_iso8601() << " [info] Andromeda Trading System — Starting the service" << std::endl;

  // Initialize libcurl globally (thread-safe usage)
  curl_global_init(CURL_GLOBAL_DEFAULT);

  // Read API key from env or secrets/finnhub.env
  const std::string api_key = ingestion::read_finnhub_api_key();
  if (api_key.empty()) {
    std::cout << util::Time::now_utc_iso8601() << " [warn] FINNHUB_API_KEY not found (env or secrets/finnhub.env)." << std::endl
              << util::Time::now_utc_iso8601() << " [warn] Skipping HTTP calls (CI-safe)." << std::endl;
    curl_global_cleanup();
    return 0;  // allow CI to pass without secrets
  }

  // Resolve symbols from constant config
  const auto& symbols = config::kSymbols;
  if (symbols.empty()) {
    std::cerr << util::Time::now_utc_iso8601() << " [error] No valid symbols provided." << std::endl;
    curl_global_cleanup();
    return 2;
  }

  const std::size_t max_retries = util::env_size_t("RETRIES", 2, 0, 10);

  // Fetch a snapshot quote via HTTP and log raw JSON for each symbol (sequential with retries)
  try {
    ingestion::FinnhubHttpClient http(api_key);
    for (const auto& sym : symbols) {
      std::size_t attempt = 0;
      int backoff_ms = 250;
      for (;;) {
        try {
          const auto quote_json = http.get_quote(sym);
          std::cout << util::Time::now_utc_iso8601() << " [info] HTTP /quote for " << sym << ": " << quote_json << std::endl;
          break;
        } catch (const std::exception& e) {
          if (attempt >= max_retries) {
            std::cerr << util::Time::now_utc_iso8601() << " [error] HTTP quote failed for " << sym
                      << " after " << (attempt + 1) << " attempt(s): " << e.what() << std::endl;
            break;
          }
          std::cerr << util::Time::now_utc_iso8601() << " [warn] HTTP quote error for " << sym << ": " << e.what()
                    << " (retry " << (attempt + 1) << "/" << (max_retries + 1) << ")" << std::endl;
          std::this_thread::sleep_for(std::chrono::milliseconds(backoff_ms));
          backoff_ms = std::min(backoff_ms * 2, 4000);
          ++attempt;
        }
      }
    }
  } catch (const std::exception& e) {
    std::cerr << util::Time::now_utc_iso8601() << " [error] HTTP client error: " << e.what() << std::endl;
    curl_global_cleanup();
    return 1;
  }

  curl_global_cleanup();
  return 0;
}

