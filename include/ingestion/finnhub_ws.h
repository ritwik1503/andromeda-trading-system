#pragma once

#include <functional>
#include <string>

#include <ingestion/types.h>

namespace ingestion {

// Minimal WebSocket client for Finnhub streaming trades.
class FinnhubWsClient {
 public:
  explicit FinnhubWsClient(std::string api_key,
                           std::string base_url = "wss://ws.finnhub.io");

  void set_on_trade(std::function<void(const Trade&)> cb);

  // Connect and start streaming. Call subscribe() after start or pass initial
  // symbols via subscribe() before starting.
  void start();
  void stop();

  void subscribe(const std::string& symbol);
  void unsubscribe(const std::string& symbol);

 private:
  struct Impl;
  Impl* impl_;
};

// Utility: try to read FINNHUB_API_KEY, first from env var, then from
// secrets/finnhub.env (KEY=VALUE lines). Returns empty string if not found.
std::string read_finnhub_api_key();

}  // namespace ingestion
