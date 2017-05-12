#include "misc.h"
#include "utils.h"
#include "uv.h"

duk_ret_t duv_cwd(duk_context *ctx) {
  size_t size = 2 * PATH_MAX;
  char path[2 * PATH_MAX];
  duv_check(ctx, uv_cwd(path, &size));
  duk_push_lstring(ctx, path, size);
  return 1;
}