add_executable(recommendations
  main.cpp
)

target_link_libraries(recommendations PRIVATE andromeda_strategy andromeda_ingestion andromeda_util)

apply_release_opts(recommendations)

if (ipo_supported)
  set_property(TARGET recommendations PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()
