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

dukext_req_t *dukext_setup_req(duk_context *ctx, int callback_index) {
  dukext_req_t *data = duk_alloc(ctx, sizeof(*data));
  duk_push_this(ctx);
  data->context = dukext_ref(ctx);
  duk_dup(ctx, -1);
  data->req_ref = dukext_ref(ctx);
  if (duk_is_function(ctx, callback_index)) {
    duk_dup(ctx, callback_index);
    data->callback_ref = dukext_ref(ctx);
  } else {
    data->callback_ref = 0;
  }
  data->data_ref = 0;
  data->data = NULL;
  return data;
}
dukext_req_t *dukext_cleanup_req(duk_context *ctx, dukext_req_t *data) {
  dukext_unref(ctx, data->req_ref);
  dukext_unref(ctx, data->context);
  dukext_unref(ctx, data->callback_ref);
  dukext_unref(ctx, data->data_ref);
  duk_free(ctx, data->data);
  duk_free(ctx, data);
  return NULL;
}

void dukext_fulfill_req(duk_context *ctx, uv_req_t *req, int nargs) {
  dukext_req_t *data = req->data;
  if (data->callback_ref) {
    dukext_push_ref(ctx, data->callback_ref);
    if (nargs)
      duk_insert(ctx, -1 - nargs);
    dukext_push_ref(ctx, data->context);
    if (nargs)
      duk_insert(ctx, -1 - nargs);
    duk_call_method(ctx, nargs);
    duk_pop(ctx);
  } else if (nargs) {
    duk_pop_n(ctx, nargs);
  }
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

int dukext_push_status(duk_context *ctx, int status) {
  if (status < 0) {
    return duk_push_error_object(ctx, DUK_ERR_ERROR, "%s: %s",
                                 uv_err_name(status), uv_strerror(status));
  }
  duk_push_null(ctx);
  return 0;
}
void dukext_get_data(duk_context *ctx, int index, uv_buf_t *buf) {
  duk_size_t len;
  if (duk_is_string(ctx, index)) {
    buf->base = (char *)duk_get_lstring(ctx, index, &len);
  } else {
    buf->base = duk_get_buffer(ctx, index, &len);
  }
  buf->len = len;
}