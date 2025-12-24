#pragma once

#include <ingestion/finnhub_http.h>
#include <ingestion/types.h>

#include <functional>
#include <string>
#include <vector>

namespace ingestion {

    // FinnhubLiveSource is the live data entrypoint. It fetches symbols through REST endpoints and fetches real time data.
    class FinnhubLiveSource {
        public:
            using TradeCallback = std::function<void(const Trade&)>;

            explicit FinnhubLiveSource(
                std::string api_token,
                std::string exchange = "US",
                std::string mic = {},
                std::string security_type = {},
                std::string currency = {},
                std::size_t poll_interval_ms = 1000,
                std::size_t max_symbols_per_poll = 25
            );

            // Consumer can register to receive raw trades.
            void set_on_trade(TradeCallback cb);

            // Fetch symbols and subscribe for real-time data.
            void start();

            // Stop real-time data and clean up.
            void stop();
        
        private:
            std::string api_token_;
            std::string exchange_;
            std::string mic_;
            std::string security_type_;
            std::string currency_;
            std::size_t poll_interval_ms_;
            std::size_t max_symbols_per_poll_;

            FinnhubHttpClient http_client_;

            TradeCallback on_trade_;
            std::vector<std::string> symbols_;
            bool running_{false};
    };

} // namespace ingestion