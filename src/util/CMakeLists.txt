add_library(andromeda_util STATIC
  ${PROJECT_SOURCE_DIR}/src/util/secrets.cpp
)

target_include_directories(andromeda_util
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)

apply_release_opts(andromeda_util)

