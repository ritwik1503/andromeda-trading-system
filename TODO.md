SPSC Queue (Low-Latency Ingestion) — TODO

- Add optional SPSC ring buffer path for WS → strategy handoff
  - Library: Boost lockfree spsc_queue (default) or moodycamel (tbd)
  - Config: build/runtime flag to select direct-callback vs SPSC
  - Overflow policy: choose one and document
    - DROP_NEWEST (fail push) | OVERWRITE_OLDEST | BLOCK/SPIN
  - Metrics: pushed, popped, dropped, high-water mark, lag
  - Threading: dedicate WS IO thread; consumer on strategy thread
  - Logging: disable in WS hot path; move to async logger if needed

- Parser optimization (WS)
  - Replace nlohmann/json with simdjson ondemand in WS path
  - Extract only fields needed: s, p, v, t, c[]
  - Avoid allocations (intern symbols later if needed)

- HTTP Quotes — Concurrency (deferred)
  - Add bounded concurrency for REST /quote fetches
    - Config: static config header or env var for parallelism
    - Backoff/retry policy per symbol with jitter
    - Optional simple rate-limiter to respect API limits
  - Telemetry: per-run success/fail counts and latencies
  - Keep default path sequential for simplicity/MVP

- Build/Run
  - Release flags: -O3 -march=native -flto
  - CI: add simdjson dependency (FetchContent)

- Validation
  - Add counters + simple perf timer around push/pop
  - Stress test with synthetic bursts; verify policy + drops
