#include <iostream>

#include <ingestion/finnhub_live_source.h>
#include <ingestion/ingestion_engine.h>
#include <util/secrets.h>
#include <util/time.h>
#include <curl/curl.h>

int main() {
  std::cout << util::Time::now_utc_iso8601() << " [info] service=andromeda, action=start" << std::endl;

  // Initialize libcurl globally (thread-safe usage)
  curl_global_init(CURL_GLOBAL_DEFAULT);

  // Read API key from env or secrets/finnhub.env
  const std::string api_key = util::read_api_key("FINNHUB_API_KEY", "secrets/finnhub.env");
  if (api_key.empty()) {
    std::cout << util::Time::now_utc_iso8601() << " [error] missing_api_key=FINNHUB_API_KEY source=env_or_file skip_http=true" << std::endl;
    curl_global_cleanup();
    return 0;  // allow CI to pass without secrets
  }

  ingestion::FinnhubLiveSource source(api_key, "US", {}, {}, {}, 1000, 25);
  ingestion::IngestionEngine engine;

  source.set_on_trade([&](const ingestion::Trade& t) {
    engine.on_trade(t);
  });

    engine.set_on_bar([&](const ingestion::Bar& b) {
    std::cout << util::Time::now_utc_iso8601()
              << " [bar] symbol=" << b.symbol
              << " close=" << b.close_price << std::endl;
  });

  source.start();  // blocking poll loop
  
  curl_global_cleanup();
  return 0;
}
