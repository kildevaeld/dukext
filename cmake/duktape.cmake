set(DUKTAPEDIR ${PROJECT_ROOT}/libs/duktape)

include_directories(
  ${DUKTAPEDIR}/src
  ${DUKTAPEDIR}/extras
)

add_library(duktape STATIC
    ${DUKTAPEDIR}/src/duktape.c
    ${DUKTAPEDIR}/extras/module-node/duk_module_node.c
    ${DUKTAPEDIR}/extras/console/duk_console.c
    ${DUKTAPEDIR}/extras/duk-v1-compat/duk_v1_compat.c
)

if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  target_link_libraries(duktape
    m dl rt
  )
endif()