#include "dukext.h"
#include "console/duk_console.h"
#include "duktape.h"
#include "module-node/duk_module_node.h"
#include "refs.h"
#include "timer.h"
//#include "module.h"
extern duk_ret_t cb_resolve_module(duk_context *ctx);
extern duk_ret_t cb_load_module(duk_context *ctx);

static void init_modules(dukext_t *req) {
  duk_context *ctx = req->ctx;
  duk_push_object(ctx);
  duk_push_c_function(ctx, cb_resolve_module, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, "resolve");
  duk_push_c_function(ctx, cb_load_module, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, "load");
  duk_module_node_init(ctx);
}

int dukext_init(dukext_t *req, uv_loop_t *loop) {
  req->ctx = duk_create_heap(NULL, NULL, NULL, loop, NULL);
  req->loop = loop;
  dukext_ref_setup(req->ctx);

  duk_console_init(req->ctx, DUK_CONSOLE_PROXY_WRAPPER);
  dukext_timer_init(req->ctx);

  init_modules(req);

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