
set(SOURCES
    dschema.c
    misc.c
    utils.c
    dukext.c
    module.c
    dschema_uv.c
    timer.c
    refs.c
    callbacks.c
    fs.c
    handle.c
    req.c
    tty.c
    stream.c
)

set(LIBRARIES duktape uvext)

if(${DUKEXT_CRYPTO})
    set(SOURCES ${SOURCES} crypto.c)
    set(LIBRARIES ${LIBRARIES} ssl crypto)
endif(${DUKEXT_CRYPTO})

if (${DUKEXT_NODE_COMPAT})
    set(SOURCES ${SOURCES} module_data.c)
endif(${DUKEXT_NODE_COMPAT})

add_library(dukext ${SOURCES})
target_link_libraries(dukext ${LIBRARIES})