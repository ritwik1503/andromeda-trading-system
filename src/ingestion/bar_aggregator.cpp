#include <ingestion/bar_aggregator.h>
#include <utility>

namespace ingestion {
    BarAggregator::BarAggregator(long long bucket_ms) : bucket_ms_(bucket_ms) {}

    void BarAggregator::set_on_bar(BarCallback cb) {
        on_bar_ = std::move(cb);
    }

    void BarAggregator::on_trade(const Trade& trade) {
        if (trade.symbol.empty() || trade.ts_ms <= 0) {
            return;
        }

        const long long bucket_start = (trade.ts_ms / bucket_ms_) * bucket_ms_;
        auto& state = states_[trade.symbol];

        // First trade for this symbol.
        if (state.bar.symbol.empty()) {
            state.bar.symbol = trade.symbol;
            state.bar.open_price = trade.price;
            state.bar.high_price = trade.price;
            state.bar.low_price = trade.price;
            state.bar.close_price = trade.price;
            state.bar.volume = trade.volume;
            state.bar.start_ts_ms = bucket_start;
            return;
        }

        // Same time bucket → update bar.
        if (state.bar.start_ts_ms == bucket_start) {
            if (trade.price > state.bar.high_price) {
                state.bar.high_price = trade.price;
            }

            if (trade.price < state.bar.low_price) {
                state.bar.low_price = trade.price;
            }

            state.bar.close_price = trade.price;
            state.bar.volume += trade.volume;
            return;
        }
        
        // New time bucket → emit old bar, start a new one.
        if (on_bar_) {
            on_bar_(state.bar);
        }

        state.bar.symbol = trade.symbol;
        state.bar.open_price = trade.price;
        state.bar.high_price = trade.price;
        state.bar.low_price = trade.price;
        state.bar.close_price = trade.price;
        state.bar.volume = trade.volume;
        state.bar.start_ts_ms = bucket_start;
    }

    void BarAggregator::flush() {
        if (!on_bar_) {
            return;
        }

        for (auto& kv : states_) {
            if (!kv.second.bar.symbol.empty()) on_bar_(kv.second.bar);
        }
    }
}
