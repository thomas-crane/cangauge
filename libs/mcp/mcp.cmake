set(LIB_NAME mcp)

add_library(${LIB_NAME}
  INTERFACE
  )

target_sources(${LIB_NAME}
  INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/mcp.c
  )

target_link_libraries(${LIB_NAME}
  INTERFACE
  pico_stdlib
  hardware_i2c
  )

target_include_directories(${LIB_NAME}
  INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/include/
  )
