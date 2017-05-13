#include "callbacks.h"
#include "utils.h"
#include <assert.h>
void dukext_close_cb(uv_handle_t *handle) {
  dukext_handle_t *h = (dukext_handle_t *)handle->data;

  dukext_emit_event(h->ctx, h, DUKEXT_CLOSED, 0);
  handle->data = dukext_cleanup_handle(h->ctx, h);
}

void dukext_timer_cb(uv_timer_t *handle) {
  dukext_handle_t *h = (dukext_handle_t *)handle->data;
  dukext_emit_event(h->ctx, h, DUKEXT_TIMEOUT, 0);
}

void dukext_async_cb(uv_async_t *handle) {
  dukext_handle_t *h = (dukext_handle_t *)handle->data;
  dukext_emit_event(h->ctx, h, DUKEXT_TIMEOUT, 0);

  dukext_cleanup_handle(h->ctx, h);
  uv_close((uv_handle_t *)handle, NULL);
}

void dukext_connect_cb(uv_connect_t *req, int status) {
  duk_context *ctx = req->handle->loop->data;
  dukext_push_status(ctx, status);
  dukext_fulfill_req(ctx, (uv_req_t *)req, 1);
  req->data = dukext_cleanup_req(ctx, req->data);
}

void dukext_shutdown_cb(uv_shutdown_t *req, int status) {
  duk_context *ctx = req->handle->loop->data;
  dukext_push_status(ctx, status);
  dukext_fulfill_req(ctx, (uv_req_t *)req, 1);
  req->data = dukext_cleanup_req(ctx, req->data);
}

void dukext_connection_cb(uv_stream_t *handle, int status) {
  duk_context *ctx = handle->loop->data;
  dukext_push_status(ctx, status);
  dukext_emit_event(ctx, handle->data, DUKEXT_CONNECTION, 1);
}

void dukext_read_cb(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf) {
  dukext_handle_t *h = (dukext_handle_t *)handle->data;
  duk_context *ctx = h->ctx;

  if (nread >= 0) {
    char *out;
    duk_push_null(ctx);
    out = duk_push_fixed_buffer(ctx, nread);
    memcpy(out, buf->base, nread);
  }

  free(buf->base);
  if (nread == 0)
    return;

  if (nread == UV_EOF) {
    duk_push_null(ctx);      // no error
    duk_push_undefined(ctx); // undefined value to signify EOF
  } else if (nread < 0) {
    dukext_push_status(ctx, nread);
  }

  dukext_emit_event(ctx, handle->data, DUKEXT_READ, 2);
}

void dukext_write_cb(uv_write_t *req, int status) {

  dukext_req_t *h = (dukext_req_t *)req->data;

  duk_context *ctx = h->ctx;
  dukext_push_status(ctx, status);
  dukext_fulfill_req(ctx, (uv_req_t *)req, 1);
  dukext_cleanup_req(ctx, req->data);
}

void dukext_alloc_cb(uv_handle_t *handle, size_t suggested_size,
                     uv_buf_t *buf) {
  buf->base = malloc(suggested_size);
  assert(buf->base);
  buf->len = suggested_size;
}
