set(WWW_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(DIST_DIR "${WWW_DIR}/dist")
set(OUTPUT_HEADER "${CMAKE_BINARY_DIR}/index_html.h")

add_custom_command(
    OUTPUT ${OUTPUT_HEADER}
    COMMAND yarn install
    COMMAND yarn build
    COMMAND xxd -n index_html -i ${DIST_DIR}/index.html > ${OUTPUT_HEADER}
    WORKING_DIRECTORY ${WWW_DIR}
    COMMENT "Building web assets and generating index_html.h"
    VERBATIM
)

add_custom_target(build_www
    DEPENDS ${OUTPUT_HEADER}
)
