cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/pico-sdk)
set(PICO_BOARD pico_w)
set(PICO_PLATFORM rp2040)

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(kmn_picow_webconfig C CXX ASM)

pico_sdk_init()

add_executable(kmn_picow_webconfig
  src/main.c
  src/server.c
)

target_include_directories(kmn_picow_webconfig PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/include
)

pico_enable_stdio_uart(kmn_picow_webconfig 1)
pico_enable_stdio_usb(kmn_picow_webconfig 0)

target_link_libraries(kmn_picow_webconfig
  pico_stdlib
  hardware_spi
  pico_cyw43_arch_lwip_threadsafe_background
)

# Cria UF2 para flash no Pico W
pico_add_extra_outputs(kmn_picow_webconfig)

pico_configure_ip4_address(kmn_picow_webconfig PRIVATE
  CYW43_DEFAULT_IP_AP_ADDRESS 192.168.4.1
)
