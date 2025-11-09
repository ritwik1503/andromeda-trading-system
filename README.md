Andromeda Trading System

How to build and run
- Configure: cmake -S . -B build
- Build: cmake -S . -B build && cmake --build build -j
- Run: ./build/bin/andromeda

Notes
- Requires CMake 3.15+ and a C++17 compiler.
- Local secrets (if any) go under secrets/ (ignored by git).
