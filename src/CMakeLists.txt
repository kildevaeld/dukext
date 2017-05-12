
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
)


add_library(dukext ${SOURCES})
target_link_libraries(dukext duktape uvext)