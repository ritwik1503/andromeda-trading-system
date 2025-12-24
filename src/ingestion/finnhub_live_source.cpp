#include <iostream>
#include <thread>
#include <chrono>
#include <util/time.h>
#include <algorithm>

#include <ingestion/finnhub_live_source.h>

#include <nlohmann/json.hpp>
#include <utility>

using json = nlohmann::json;

namespace ingestion {

    // Constructor
    FinnhubLiveSource::FinnhubLiveSource(
        std::string api_token,
        std::string exchange,
        std::string mic,
        std::string security_type,
        std::string currency,
        std::size_t poll_interval_ms,
        std::size_t max_symbols_per_poll)
        :   api_token_(std::move(api_token)),
            exchange_(std::move(exchange)),
            mic_(std::move(mic)),
            security_type_(std::move(security_type)),
            currency_(std::move(currency)),
            poll_interval_ms_(poll_interval_ms),
            max_symbols_per_poll_(max_symbols_per_poll),
            http_client_(api_token_) {}

    // Save the callback so we can call it when a trade arrives.
    void FinnhubLiveSource::set_on_trade(TradeCallback cb) {
        on_trade_ = std::move(cb);
    }

    void FinnhubLiveSource::start() {
        running_ = true;

        // Step-1 : Fetch the list of symbols from API
        const auto symbol_response =
            http_client_.get_stock_symbols(exchange_, mic_, security_type_, currency_);
        auto symbols_json = json::parse(symbol_response, nullptr, false);
        if (symbols_json.is_discarded() || !symbols_json.is_array()) {
            throw std::runtime_error("invalid symbols response");
        }
        std::cout << util::Time::now_utc_iso8601() << " [info] symbols received from API " << symbols_json << std::endl;

        symbols_.clear();
        for (const auto& it : symbols_json) {
            auto sym = it.value("symbol", std::string{});
            if (!sym.empty()) symbols_.push_back(std::move(sym));
        }

        // Step-2 : Poll quotes untill stop() is called
        while(running_) {
            const std::size_t limit = std::min(symbols_.size(), max_symbols_per_poll_);

            for (std::size_t i = 0; i < limit; ++i) {
                const auto& sym = symbols_[i];

                if (!running_) {
                    break;
                }

                const auto quote_response = http_client_.get_quote(sym);
                auto quotes_json = json::parse(quote_response, nullptr, false);
                if (quotes_json.is_discarded() || !quotes_json.is_object()) {
                    continue;
                }

                const double price = quotes_json.value("c", 0.0);
                const long long ts_sec = quotes_json.value("t", 0LL);
                if (price <= 0.0 || ts_sec <= 0) {
                    continue;
                }

                Trade t;
                t.symbol = sym;
                t.price = price;
                t.ts_ms = ts_sec * 1000;
                t.volume = 0.0;  // quote endpoint doesn't include volume

                if (on_trade_) {
                    on_trade_(t);
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(poll_interval_ms_));
        }
    }

    void FinnhubLiveSource::stop() {
        running_ = false;
    }
} // namespace ingestion
