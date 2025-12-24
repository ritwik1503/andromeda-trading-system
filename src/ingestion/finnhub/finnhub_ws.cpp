#include <ingestion/finnhub_ws.h>
#include <ingestion/types.h>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <nlohmann/json.hpp>

#include <cstdlib>
#include <iostream>
#include <mutex>

using json = nlohmann::json;

namespace ingestion {

namespace {
}

struct FinnhubWsClient::Impl {
  ix::WebSocket ws;
  std::string api_key;
  std::string base_url;
  std::function<void(const Trade&)> on_trade;
  std::mutex send_mtx;


  explicit Impl(std::string key, std::string url)
      : api_key(std::move(key)), base_url(std::move(url)) {}
};

FinnhubWsClient::FinnhubWsClient(std::string api_key, std::string base_url)
    : impl_(new Impl(std::move(api_key), std::move(base_url))) {}

void FinnhubWsClient::set_on_trade(std::function<void(const Trade&)> cb) {
  impl_->on_trade = std::move(cb);
}

 

void FinnhubWsClient::start() {
  // Set URL with token
  std::string url = impl_->base_url + "?token=" + impl_->api_key;
  impl_->ws.setUrl(url);

  // Basic reconnect/backoff handled by ixwebsocket internally; we just log
  impl_->ws.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
    if (!msg) return;
    if (msg->type == ix::WebSocketMessageType::Open) {
      std::cerr << "[finnhub] connected" << std::endl;
    } else if (msg->type == ix::WebSocketMessageType::Close) {
      std::cerr << "[finnhub] closed: code=" << msg->closeInfo.code
                << " reason=" << msg->closeInfo.reason << std::endl;
    } else if (msg->type == ix::WebSocketMessageType::Error) {
      std::cerr << "[finnhub] error: " << msg->errorInfo.reason << std::endl;
    } else if (msg->type == ix::WebSocketMessageType::Message) {
      try {
        auto j = json::parse(msg->str, nullptr, false);
        if (j.is_discarded()) return;
        if (!j.contains("type")) return;
        if (j["type"].get<std::string>() != "trade") return;
        if (!j.contains("data") || !j["data"].is_array()) return;
        for (const auto& it : j["data"]) {
          Trade t;
          t.symbol = it.value("s", std::string{});
          t.price = it.value("p", 0.0);
          t.volume = it.value("v", 0.0);
          t.ts_ms = it.value("t", 0LL);
          if (it.contains("c") && it["c"].is_array()) {
            for (const auto& c : it["c"]) t.conditions.push_back(c.get<std::string>());
          }
          if (impl_->on_trade) impl_->on_trade(t);
        }
      } catch (...) {
        // ignore malformed frames
      }
    }
  });

  impl_->ws.start();
}

void FinnhubWsClient::stop() { impl_->ws.stop(); }

void FinnhubWsClient::subscribe(const std::string& symbol) {
  json sub{{"type", "subscribe"}, {"symbol", symbol}};
  std::lock_guard<std::mutex> lk(impl_->send_mtx);
  impl_->ws.send(sub.dump());
}

void FinnhubWsClient::unsubscribe(const std::string& symbol) {
  json unsub{{"type", "unsubscribe"}, {"symbol", symbol}};
  std::lock_guard<std::mutex> lk(impl_->send_mtx);
  impl_->ws.send(unsub.dump());
}

}  // namespace ingestion
