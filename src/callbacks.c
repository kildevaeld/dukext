#include "callbacks.h"
#include "utils.h"

void dukext_timer_cb(uv_timer_t *handle) {
  dukext_handle_t *h = (dukext_handle_t *)handle->data;
  dukext_emit_event(h->ctx, h, DUKEXT_TIMEOUT, 0);
}