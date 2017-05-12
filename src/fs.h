#ifndef DUKEXT_FS_H
#define DUKEXT_FS_H

#include "uv.h"

const char *dukext_read_file(uv_loop_t *loop, const char *path);

#endif