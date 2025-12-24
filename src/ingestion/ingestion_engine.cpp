#include <ingestion/ingestion_engine.h>

#include <utility>

namespace ingestion {

    // Constructor: Initialzes the IngestionEngine with the bucket time.
    IngestionEngine::IngestionEngine(long long bar_bucket_ms)
        : aggregator_(bar_bucket_ms) {
        // Link the aggregator's on_bar to this engine's on_bar callback
        aggregator_.set_on_bar([this](const Bar& bar) {
            if (on_bar_) on_bar_(bar);
        });
    }

    void IngestionEngine::set_on_trade(TradeCallback cb) {
        on_trade_ = std::move(cb);
    }

    void IngestionEngine::set_on_bar(BarCallback cb) {
        on_bar_ = std::move(cb);
    }

    void IngestionEngine::on_trade(const Trade& trade) {
    if (on_trade_) on_trade_(trade);
        aggregator_.on_trade(trade);
    }

    void IngestionEngine::flush() {
        aggregator_.flush();
    }
}