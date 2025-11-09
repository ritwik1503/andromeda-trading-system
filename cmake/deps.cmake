# Centralized third-party dependencies

include(FetchContent)

# ixwebsocket options
# TLS enabled; disable zlib/per-message-deflate to avoid CI issues on Linux
set(IXWEBSOCKET_USE_TLS ON CACHE BOOL "Enable TLS for ixwebsocket" FORCE)
set(USE_ZLIB OFF CACHE BOOL "Enable zlib for ixwebsocket" FORCE) 

# nlohmann/json (header-only)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# ixwebsocket
FetchContent_Declare(
  ixwebsocket
  GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
  GIT_TAG v11.3.3
)
FetchContent_MakeAvailable(ixwebsocket)

# Workaround/warnings hygiene for ixwebsocket on some toolchains.
if (TARGET ixwebsocket)
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(ixwebsocket PRIVATE
      -include cstdint
      -Wno-unused-parameter
      -Wno-unused-private-field
      -Wno-unused-const-variable
    )
  elseif (MSVC)
    target_compile_options(ixwebsocket PRIVATE
      /FIcstdint
      /wd4100  # unreferenced formal parameter
      /wd4189  # local variable initialized but not referenced
    )
  endif()
endif()
