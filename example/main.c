#include "dukext.h"
#include "fs.h"
#include "uv.h"
int main() {

  uv_loop_t *loop = uv_default_loop();

  // uv_loop_init(loop);

  dukext_t duk;

  dukext_init(&duk, loop);
  loop->data = &duk;

  /*dukext_run(&duk, "console.log('hello');var timer = EventLoop.createTimer();"
                   "EventLoop.startTimer(timer, 10000, 0, function() {"
                   "console.log('hello');"
                   "});");*/
  const char *str = dukext_read_file(loop, "../test.js");

  if (dukext_run(&duk, str) != 0) {
    printf("%s\n", duk_get_string(duk.ctx, -1));

    // return 2;
  }

  uv_run(loop, UV_RUN_DEFAULT);
  dukext_cleanup(&duk);

  return 0;
}