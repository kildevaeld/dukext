#include "utils.h"
#include "refs.h"
uv_loop_t *dukext_loop(duk_context *ctx) {
  duk_memory_functions funcs;
  duk_get_memory_functions(ctx, &funcs);
  return funcs.udata;
}

void dukext_error(duk_context *ctx, int status) {
  duk_error(ctx, DUK_ERR_ERROR, "%s: %s", uv_err_name(status),
            uv_strerror(status));
}

void dukext_check(duk_context *ctx, int status) {
  if (status < 0)
    dukext_error(ctx, status);
}

dukext_handle_t *dukext_setup_handle(duk_context *ctx) {
  dukext_handle_t *data = duk_alloc(ctx, sizeof(*data));
  duk_push_this(ctx);
  data->context = dukext_ref(ctx);
  duk_dup(ctx, -1);
  data->ref = dukext_ref(ctx);
  data->callbacks[0] = 0;
  data->callbacks[1] = 0;
  data->ctx = ctx;
  return data;
}

dukext_handle_t *dukext_cleanup_handle(duk_context *ctx,
                                       dukext_handle_t *data) {
  dukext_unref(ctx, data->ref);
  dukext_unref(ctx, data->context);
  dukext_unref(ctx, data->callbacks[0]);
  dukext_unref(ctx, data->callbacks[1]);
  duk_free(ctx, data);
  return NULL;
}

void dukext_store_handler(duk_context *ctx, dukext_handle_t *data, int type,
                          int index) {
  if (!duk_is_function(ctx, index))
    return;
  duk_dup(ctx, index);
  data->callbacks[type] = dukext_ref(ctx);
}

static duk_ret_t unsafe_code(duk_context *ctx, void *udata) {
  int *i = (int *)udata;
  int nargs = *i;

  duk_call_method(ctx, *i);

  return 0;
}

void dukext_emit_event(duk_context *ctx, dukext_handle_t *data,
                       dukext_callback_id type, int nargs) {

  int fn_ref = data->callbacks[type];

  if (fn_ref && ctx) {
    dukext_push_ref(ctx, fn_ref);
    if (nargs) {
      duk_insert(ctx, -1 - nargs);
    }
    dukext_push_ref(ctx, data->context);
    if (nargs) {
      duk_insert(ctx, -1 - nargs);
    }
    if (duk_safe_call(ctx, unsafe_code, &nargs /*udata*/, nargs /*nargs*/,
                      1 /*nrets */) != 0) {
      /* The 'nrets' argument should be at least 1 so that an error value
       * is left on the stack if an error occurs.  To avoid further errors,
       * use duk_safe_to_string() for safe error printing.
       */
      printf("Unexpected error: %s\n", duk_safe_to_string(ctx, -1));
    }
    duk_pop(ctx);
    /*duk_call_method(ctx, nargs);
    duk_pop(ctx);*/
  } else if (nargs) {
    duk_pop_n(ctx, nargs);
  }
}