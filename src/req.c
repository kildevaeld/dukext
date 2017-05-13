#include "req.h"
#include "dschema.h"
#include "dschema_uv.h"
#include "utils.h"

duk_ret_t dukext_cancel(duk_context *ctx) {
  uv_req_t *req;

  dschema_check(ctx,
                (const dukext_schema_entry[]){{"req", dukext_is_req}, {NULL}});

  req = duk_get_buffer(ctx, 0, NULL);
  dukext_check(ctx, uv_cancel(req));
  return 0;
}
