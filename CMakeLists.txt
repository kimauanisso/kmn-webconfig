cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TEST_MODE ON)

set(COMMON_FILES
  src/http.c
  src/router.c
  src/http_response.c
  src/variables.c
  src/handlers/notFoundHandler.c
  src/handlers/gen204Handler.c
  src/handlers/dashboardHandler.c
  src/handlers/definitionsHandler.c
  src/handlers/variableHandler.c
)

add_subdirectory(www)

project(kmn_webconfig_proj C CXX ASM)

pico_sdk_init()

add_library(kmn_webconfig STATIC
  ${COMMON_FILES}
  src/server.c
  deps/dhcpserver/dhcpserver.c
  deps/dnsserver/dnsserver.c
)

target_link_libraries(kmn_webconfig
  pico_stdlib
  hardware_spi
  pico_cyw43_arch_lwip_threadsafe_background
)

add_dependencies(kmn_webconfig build_www)

pico_configure_ip4_address(kmn_webconfig PRIVATE CYW43_DEFAULT_IP_AP_ADDRESS 192.168.4.1)

target_include_directories(kmn_webconfig PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/deps
  ${CMAKE_CURRENT_LIST_DIR}/www
  ${CMAKE_BINARY_DIR}
)