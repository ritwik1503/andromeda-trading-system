add_library(andromeda_ingestion STATIC
  finnhub_ws.cpp
  finnhub_http.cpp
)

target_include_directories(andromeda_ingestion
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(andromeda_ingestion
  PRIVATE ixwebsocket nlohmann_json::nlohmann_json CURL::libcurl
)

apply_release_opts(andromeda_ingestion)
