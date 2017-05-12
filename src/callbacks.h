#ifndef DUKEXT_CALLBACKS_H
#define DUKEXT_CALLBACKS_H
#include "dukext.h"

void dukext_timer_cb(uv_timer_t *handle);
void dukext_close_cb(uv_handle_t *handle);
void dukext_async_cb(uv_async_t *handle);
void dukext_connect_cb(uv_connect_t *req, int status);
void dukext_shutdown_cb(uv_shutdown_t *req, int status);
void dukext_connection_cb(uv_stream_t *handle, int status);
void dukext_read_cb(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf);
void dukext_write_cb(uv_write_t *req, int status);
void dukext_alloc_cb(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);

#endif