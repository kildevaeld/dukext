#ifndef DUKEXT_FS_H
#define DUKEXT_FS_H

#include "dukext.h"

const char *dukext_read_file(uv_loop_t *loop, const char *path);

void dukext_fs_init(dukext_t *duk);

#endif