Andromeda Trading System

Build
- Configure: `cmake -S . -B build`
- Build: `cmake --build build -j`
- One-liner (configure + build): `cmake -S . -B build && cmake --build build -j`

API key
- Env var: `export FINNHUB_API_KEY=your_token`
- Or file: create `secrets/finnhub.env` with `FINNHUB_API_KEY=your_token`

Apps
- Quotes demo: `./build/bin/andromeda`
  - Fetches snapshot quotes for tickers in `include/config/symbols.h`
- Recommendations demo: `./build/bin/recommendations`
  - Prints BUY/NO-BUY per symbol for the last 3 months based on Finnhub recommendation trends

Customize
- Edit tickers in `include/config/symbols.h`

Notes
- Requires CMake 3.15+ and a C++17 compiler.
- Local secrets (if any) go under `secrets/` (ignored by git).
