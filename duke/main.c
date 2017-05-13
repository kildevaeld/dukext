#include "dukext.h"
#include "fs.h"
#include "uv.h"

int main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(stderr, "usage: %s <path>\n", argv[0]);
    return 2;
  }

  const char *path = argv[1];



  uv_loop_t *loop = uv_default_loop();

  dukext_t duk;



  dukext_init(&duk, loop);
  // loop->data = &duk;

  const char *str = dukext_read_file(loop, path);

  if (dukext_run(&duk, str) != 0) {
    printf("%s\n", duk_get_string(duk.ctx, -1));

    // return 2;
  }

  uv_run(loop, UV_RUN_DEFAULT);
  dukext_cleanup(&duk);

  return 0;
}