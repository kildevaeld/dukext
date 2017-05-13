#include "dukext.h"
#include "console/duk_console.h"
#include "duktape.h"
#include "fs.h"
#include "handle.h"
#include "module-node/duk_module_node.h"

#include "crypto.h"
#include "refs.h"
#include "req.h"
#include "stream.h"
#include "timer.h"
#include "tty.h"

extern duk_ret_t cb_resolve_module(duk_context *ctx);
extern duk_ret_t cb_load_module(duk_context *ctx);

#ifdef DUKEXT_NODE_COMPAT
extern unsigned char prelude_js[];
extern unsigned int prelude_js_len;

extern unsigned char promise_js[];
extern unsigned int promise_js_len;

extern unsigned char timers_js[];
extern unsigned int timers_js_len;
#endif

static void init_modules(dukext_t *req) {
  duk_context *ctx = req->ctx;
  duk_push_object(ctx);
  duk_push_c_function(ctx, cb_resolve_module, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, "resolve");
  duk_push_c_function(ctx, cb_load_module, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, "load");
  duk_module_node_init(ctx);
}

static const duk_function_list_entry dukext_funcs[] = {

    // req.c
    {"cancel", dukext_cancel, 1},

    // handle.c
    {"close", dukext_close, 2},

    {"timerCreate", dukext_timer_create, 0},
    {"timerStart", dukext_timer_start, 4},
    {"timerStop", dukext_timer_stop, 1},
    {"async", dukext_async_create, 1},

    // stream.c
    {"shutdown", dukext_shutdown, 2},
    {"listen", dukext_listen, 3},
    {"accept", dukext_accept, 2},
    {"read_start", dukext_read_start, 2},
    {"read_stop", dukext_read_stop, 1},
    {"write", dukext_write, 3},
    {"is_readable", dukext_is_readable, 1},
    {"is_writable", dukext_is_writable, 1},
    {"stream_set_blocking", dukext_stream_set_blocking, 2},

    // tty
    {"ttyCreate", dukext_new_tty, 2},
    {"ttySetMode", dukext_tty_set_mode, 2},
    {"ttyRestMode", dukext_tty_reset_mode, 0},
    {"ttyGetWinSize", dukext_tty_get_winsize, 1},

    /*{"createHash", dukext_crypto_hash_create, 1},
    {"updateHash", dukext_crypto_hash_update, 2},
    {"digestHash", dukext_crypto_hash_digest, 1},*/

    {NULL, NULL, 0}};

static void init_globals(duk_context *ctx) {
  duk_push_global_object(ctx);
  duk_push_object(ctx);
  duk_put_function_list(ctx, -1, dukext_funcs);
  duk_put_prop_string(ctx, -2, "uv");
#ifdef DUKEXT_NODE_COMPAT
  duk_push_string(ctx, (const char *)timers_js);
  duk_eval(ctx);
  duk_pop(ctx);
  duk_push_lstring(ctx, (const char *)prelude_js, prelude_js_len);
  duk_eval(ctx);
  duk_pop(ctx);
#endif
}

int dukext_init(dukext_t *req, uv_loop_t *loop) {
  req->ctx = duk_create_heap(NULL, NULL, NULL, loop, NULL);
  req->loop = loop;
  dukext_ref_setup(req->ctx);

  duk_console_init(req->ctx, DUK_CONSOLE_PROXY_WRAPPER);
  // dukext_timer_init(req->ctx);
  // dukext_fs_init(req);
  init_modules(req);

  init_globals(req->ctx);

  return 0;
}

duk_ret_t dukext_run(dukext_t *req, const char *source) {

  duk_context *ctx = req->ctx;
  if (duk_peval_string(ctx, source) != 0) {
    if (duk_get_prop_string(ctx, -1, "stack")) {
      duk_replace(ctx, -2);
    } else {
      duk_pop(ctx);
    }
    dukext_timer_cleanup(ctx);
    return 1;
  }
  return 0;
}

void dukext_cleanup(dukext_t *req) {
  if (!req && !req->ctx)
    return;
  dukext_timer_cleanup(req->ctx);
  duk_destroy_heap(req->ctx);
}