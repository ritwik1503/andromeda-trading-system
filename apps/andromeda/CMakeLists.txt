add_executable(andromeda
  main.cpp
)

target_link_libraries(andromeda PRIVATE andromeda_ingestion)

# andromeda_ingestion exposes include/ publicly, so app inherits include path

apply_release_opts(andromeda)

# Enable LTO for the app if supported
if (ipo_supported)
  set_property(TARGET andromeda PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

