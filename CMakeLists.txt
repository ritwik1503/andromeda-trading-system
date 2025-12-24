cmake_minimum_required(VERSION 3.15)

project(AndromedaTradingSystem VERSION 0.1.0 LANGUAGES CXX)

# Generate compile_commands.json for IDEs (VS Code, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Put all executables under build/bin to avoid nested target paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(cmake/deps.cmake)
find_package(CURL REQUIRED)

# Optimize Release builds
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

function(apply_release_opts target)
  target_compile_features(${target} PRIVATE cxx_std_17)
  set_target_properties(${target} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
  target_compile_options(${target} PRIVATE
    $<$<CONFIG:Release>:
      -O3
      -DNDEBUG
      -march=native
    >
  )
endfunction()

# Subdirectories for libraries and apps
add_subdirectory(src/util)
add_subdirectory(src/ingestion)
add_subdirectory(src/strategy)
add_subdirectory(apps/andromeda)
add_subdirectory(apps/recommendations)

# Enable Link Time Optimization (if supported)
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_msg)
