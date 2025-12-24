#pragma once

#include <ingestion/bar_aggregator.h>
#include <ingestion/types.h>

#include <functional>

namespace ingestion {

    // IngestionEngine is the entry point for raw market data. It forwards trades to the aggregator and emits completed bars.
    class IngestionEngine {
    public:
        
        using TradeCallback = std::function<void(const Trade&)>;
        using BarCallback = std::function<void(const Bar&)>;

        explicit IngestionEngine(long long bar_bucket_ms = 60000);

        // Callback for raw trades
        void set_on_trade(TradeCallback cb);

        // Callback for completed bars
        void set_on_bar(BarCallback cb);

        // Feed raw trades into the engine
        void on_trade(const Trade& trade);

        // Flish any in-progress workflows
        void flush();
    
    private:
        BarAggregator aggregator_;
        TradeCallback on_trade_;
        BarCallback on_bar_;
    };

} // namespace ingestion