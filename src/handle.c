#include "handle.h"
#include "callbacks.h"
#include "dschema.h"
#include "dschema_uv.h"
#include "utils.h"

duk_ret_t dukext_close(duk_context *ctx) {
  uv_handle_t *handle;

  dschema_check(
      ctx, (const dukext_schema_entry[]){{"handle", dukext_is_handle},
                                         {"onclosed", dschema_is_continuation},
                                         {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);
  uv_close(handle, dukext_close_cb);
  dukext_store_handler(ctx, handle->data, DUKEXT_CLOSED, 1);
  return 0;
}
