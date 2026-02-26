cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TEST_MODE OFF)

set(COMMON_FILES
  src/http.c
  src/router.c
  src/http_response.c
  src/variables.c
  src/handlers/notFoundHandler.c
  src/handlers/gen204Handler.c
  src/handlers/dashboardHandler.c
  src/handlers/definitionsHandler.c
)

if(NOT TEST_MODE)

set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/pico-sdk)
set(PICO_BOARD pico_w)
set(PICO_PLATFORM rp2040)

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(kmn_webconfig C CXX ASM)

pico_sdk_init()

add_executable(kmn_webconfig
  ${COMMON_FILES}
  src/main.c
  src/server.c
  deps/dhcpserver/dhcpserver.c
  deps/dnsserver/dnsserver.c
)

target_link_libraries(kmn_webconfig
  pico_stdlib
  hardware_spi
  pico_cyw43_arch_lwip_threadsafe_background
)

pico_enable_stdio_uart(kmn_webconfig 0)
pico_enable_stdio_usb(kmn_webconfig 1)
pico_add_extra_outputs(kmn_webconfig)
pico_configure_ip4_address(kmn_webconfig PRIVATE CYW43_DEFAULT_IP_AP_ADDRESS 192.168.4.1)
else()

project(kmn_webconfig C CXX ASM)

add_executable(kmn_webconfig
  ${COMMON_FILES}
  test/test.c
)

endif()

target_include_directories(kmn_webconfig PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/deps
  ${CMAKE_CURRENT_LIST_DIR}/www
)
