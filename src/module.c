#include "duktape.h"
#include "utils.h"
#include "uv.h"

extern duk_ret_t cb_resolve_module(duk_context *ctx) {
  const char *module_id;
  const char *parent_id;

  module_id = duk_require_string(ctx, 0);
  parent_id = duk_require_string(ctx, 1);

  duk_push_sprintf(ctx, "%s.js", module_id);
  printf("resolve_cb: id:'%s', parent-id:'%s', resolve-to:'%s'\n", module_id,
         parent_id, duk_get_string(ctx, -1));

  return 1;
}

extern duk_ret_t cb_load_module(duk_context *ctx) {
  const char *filename;
  const char *module_id;

  module_id = duk_require_string(ctx, 0);
  duk_get_prop_string(ctx, 2, "filename");
  filename = duk_require_string(ctx, -1);

  printf("load_cb: id:'%s', filename:'%s'\n", module_id, filename);

  const char *path = filename; // duk_require_string(ctx, 0);
  uv_fs_t req;
  int fd = 0;
  uint64_t size;
  char *chunk;
  uv_buf_t buf;

  uv_loop_t *loop = dukext_loop(ctx);

  if (uv_fs_open(loop, &req, path, O_RDONLY, 0644, NULL) < 0)
    goto fail;
  uv_fs_req_cleanup(&req);
  fd = req.result;
  if (uv_fs_fstat(loop, &req, fd, NULL) < 0)
    goto fail;
  uv_fs_req_cleanup(&req);
  size = req.statbuf.st_size;
  chunk = duk_alloc(ctx, size);
  buf = uv_buf_init(chunk, size);
  if (uv_fs_read(loop, &req, fd, &buf, 1, 0, NULL) < 0) {
    duk_free(ctx, chunk);
    goto fail;
  }
  uv_fs_req_cleanup(&req);
  duk_push_lstring(ctx, chunk, size);
  duk_free(ctx, chunk);
  uv_fs_close(loop, &req, fd, NULL);
  uv_fs_req_cleanup(&req);

  return 1;

fail:
  uv_fs_req_cleanup(&req);
  if (fd)
    uv_fs_close(loop, &req, fd, NULL);
  uv_fs_req_cleanup(&req);
  duk_error(ctx, DUK_ERR_ERROR, "%s: %s: %s", uv_err_name(req.result),
            uv_strerror(req.result), path);

  return 1;
}