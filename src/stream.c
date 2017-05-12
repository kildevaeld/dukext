#include "stream.h"
#include "callbacks.h"
#include "dschema.h"
#include "dschema_uv.h"
#include "utils.h"

duk_ret_t dukext_shutdown(duk_context *ctx) {
  uv_stream_t *handle;
  uv_shutdown_t *req;

  dschema_check(ctx,
                (const dukext_schema_entry[]){{"stream", dukext_is_stream},
                                              {"next", dschema_is_continuation},
                                              {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);
  req = duk_push_fixed_buffer(ctx, sizeof(*req));
  dukext_check(ctx, uv_shutdown(req, handle, dukext_shutdown_cb));
  req->data = dukext_setup_req(ctx, 1);
  return 1;
}

duk_ret_t dukext_listen(duk_context *ctx) {
  uv_stream_t *handle;
  int backlog;

  dschema_check(ctx,
                (const dukext_schema_entry[]){{"stream", dukext_is_stream},
                                              {"backlog", duk_is_number},
                                              {"onConnection", duk_is_function},
                                              {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);
  backlog = duk_get_number(ctx, 1);
  dukext_check(ctx, uv_listen(handle, backlog, dukext_connection_cb));
  dukext_store_handler(ctx, handle->data, DUKEXT_CONNECTION, 2);
  return 0;
}

duk_ret_t dukext_accept(duk_context *ctx) {
  uv_stream_t *server;
  uv_stream_t *client;

  dschema_check(ctx, (const dukext_schema_entry[]){{"server", dukext_is_stream},
                                                   {"client", dukext_is_stream},
                                                   {NULL}});

  server = duk_get_buffer(ctx, 0, NULL);
  client = duk_get_buffer(ctx, 1, NULL);
  dukext_check(ctx, uv_accept(server, client));
  return 0;
}

duk_ret_t dukext_read_start(duk_context *ctx) {
  uv_stream_t *handle;

  dschema_check(ctx, (const dukext_schema_entry[]){{"stream", dukext_is_stream},
                                                   {"onread", duk_is_function},
                                                   {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);
  dukext_check(ctx, uv_read_start(handle, dukext_alloc_cb, dukext_read_cb));
  dukext_store_handler(ctx, handle->data, DUKEXT_READ, 1);
  return 0;
}

duk_ret_t dukext_read_stop(duk_context *ctx) {
  uv_stream_t *handle;

  dschema_check(
      ctx, (const dukext_schema_entry[]){{"stream", dukext_is_stream}, {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);
  dukext_check(ctx, uv_read_stop(handle));
  return 0;
}

duk_ret_t dukext_write(duk_context *ctx) {
  uv_stream_t *handle;
  uv_buf_t buf;
  uv_write_t *req;

  dschema_check(ctx,
                (const dukext_schema_entry[]){{"stream", dukext_is_stream},
                                              {"data", dschema_is_data},
                                              {"next", dschema_is_continuation},
                                              {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);
  dukext_get_data(ctx, 1, &buf);
  req = duk_push_fixed_buffer(ctx, sizeof(*req));
  dukext_check(ctx, uv_write(req, handle, &buf, 1, dukext_write_cb));
  req->data = dukext_setup_req(ctx, 2);
  return 1;
}

// static duk_ret_t dukext_write2(duk_context *ctx) {
//   uv_stream_t* handle = dukext_require_stream(L, 1);
//   uv_write_t* req;
//   uv_buf_t buf;
//   int ret, ref;
//   uv_stream_t* send_handle;
//   buf.base = (char*) luaL_checklstring(L, 2, &buf.len);
//   send_handle = dukext_require_stream(L, 3);
//   ref = dukext_check_continuation(L, 4);
//   req = lua_newuserdata(L, sizeof(*req));
//   req->data = dukext_setup_req(L, ref);
//   ret = uv_write2(req, handle, &buf, 1, send_handle, dukext_write_cb);
//   if (ret < 0) {
//     lua_pop(L, 1);
//     return dukext_error(L, ret);
//   }
//   return 1;
// }

// static duk_ret_t dukext_try_write(duk_context *ctx) {
//   uv_stream_t* handle = dukext_require_stream(L, 1);
//   uv_buf_t buf;
//   int ret;
//   buf.base = (char*) luaL_checklstring(L, 2, &buf.len);
//   ret = uv_try_write(handle, &buf, 1);
//   if (ret < 0) return dukext_error(L, ret);
//   lua_pushinteger(L, ret);
//   return 1;
// }

duk_ret_t dukext_is_readable(duk_context *ctx) {
  uv_stream_t *handle;

  dschema_check(
      ctx, (const dukext_schema_entry[]){{"stream", dukext_is_stream}, {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);
  duk_push_boolean(ctx, uv_is_readable(handle));
  return 1;
}

duk_ret_t dukext_is_writable(duk_context *ctx) {
  dschema_check(
      ctx, (const dukext_schema_entry[]){{"stream", dukext_is_stream}, {NULL}});

  uv_stream_t *handle = duk_get_buffer(ctx, 0, NULL);
  duk_push_boolean(ctx, uv_is_writable(handle));
  return 1;
}

duk_ret_t dukext_stream_set_blocking(duk_context *ctx) {
  uv_stream_t *handle;
  int blocking;

  dschema_check(ctx,
                (const dukext_schema_entry[]){{"stream", dukext_is_stream},
                                              {"isblocking", duk_is_boolean},
                                              {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);
  blocking = duk_get_boolean(ctx, 1);
  dukext_check(ctx, uv_stream_set_blocking(handle, blocking));
  return 0;
}
