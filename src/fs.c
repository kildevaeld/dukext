#include "fs.h"
#include <stdlib.h>

const char *dukext_read_file(uv_loop_t *loop, const char *path) {

  uv_fs_t req;
  int fd = 0;
  uint64_t size;
  char *chunk;
  uv_buf_t buf;

  if (uv_fs_open(loop, &req, path, O_RDONLY, 0644, NULL) < 0)
    goto fail;
  uv_fs_req_cleanup(&req);
  fd = req.result;
  if (uv_fs_fstat(loop, &req, fd, NULL) < 0)
    goto fail;
  uv_fs_req_cleanup(&req);
  size = req.statbuf.st_size;
  chunk = malloc(sizeof(char) * size + 1);
  buf = uv_buf_init(chunk, size);
  if (uv_fs_read(loop, &req, fd, &buf, 1, 0, NULL) < 0) {
    goto fail;
  }
  uv_fs_req_cleanup(&req);

  uv_fs_close(loop, &req, fd, NULL);
  uv_fs_req_cleanup(&req);
  chunk[size] = '\0';
  return chunk;

fail:
  uv_fs_req_cleanup(&req);
  if (fd)
    uv_fs_close(loop, &req, fd, NULL);
  uv_fs_req_cleanup(&req);
}