add_library(andromeda_strategy STATIC
  recommendation.cpp
)

target_include_directories(andromeda_strategy
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(andromeda_strategy
  PRIVATE nlohmann_json::nlohmann_json CURL::libcurl
)

apply_release_opts(andromeda_strategy)

