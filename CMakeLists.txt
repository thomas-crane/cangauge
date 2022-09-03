cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(APP_NAME cangauge)

include(libs/pico-sdk/pico_sdk_init.cmake)

project(${APP_NAME}
  C
  CXX
  ASM
  )

include(libs/mcp/mcp.cmake)

add_executable(${APP_NAME}
  cangauge.c
  )

target_link_libraries(${APP_NAME}
  pico_stdlib
  hardware_i2c
  mcp
  )

pico_sdk_init()
pico_enable_stdio_usb(${APP_NAME} 1)
pico_enable_stdio_uart(${APP_NAME} 0)

pico_add_extra_outputs(${APP_NAME})
