#pragma once

#include <ingestion/types.h>

#include <functional>
#include <unordered_map>

namespace ingestion
{

    // Aggregates raw Trade ticks into time-bucketed Bars [default 1 minute]
    class BarAggregator {
        public:
        // Used when a bar is completed [1 min is passed]
        using BarCallback = std::function<void(const Bar&)>;

        // Controls the bar size [Default as 1min]
        explicit BarAggregator(long long bucket_ms = 60000);

        // Register a callback to receive completed bars.
        void set_on_bar(BarCallback cb);

        // Feed each Trade into the aggregator. It updates the current bar or emits it if the time window changes.
        void on_trade(const Trade& trade);

        // Emit any in-progress bars
        void flush();

        private:
            struct State {
                Bar bar;
            };

            long long bucket_ms_;
            BarCallback on_bar_;
            std::unordered_map<std::string, State> states_;
    };
    
} // namespace ingestion
