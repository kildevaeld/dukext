#include "tty.h"
#include "dschema.h"
#include "dschema_uv.h"
#include "utils.h"

duk_ret_t dukext_new_tty(duk_context *ctx) {
  uv_tty_t *handle;
  uv_file fd;
  duk_bool_t readable;

  dschema_check(ctx, (const dukext_schema_entry[]){{"fd", duk_is_number},
                                                   {"readable", duk_is_boolean},
                                                   {NULL}});

  handle = duk_push_fixed_buffer(ctx, sizeof(*handle));

  fd = duk_get_int(ctx, 0);
  readable = duk_get_boolean(ctx, 1);
  dukext_check(ctx, uv_tty_init(dukext_loop(ctx), handle, fd, readable));
  handle->data = dukext_setup_handle(ctx);

  return 1;
}

duk_ret_t dukext_tty_set_mode(duk_context *ctx) {
  uv_tty_t *handle;
  int mode;

  dschema_check(ctx, (const dukext_schema_entry[]){{"tty", dukext_is_tty},
                                                   {"mode", duk_is_number},
                                                   {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);
  mode = duk_get_int(ctx, 1);
  dukext_check(ctx, uv_tty_set_mode(handle, mode));
  return 0;
}

duk_ret_t dukext_tty_reset_mode(duk_context *ctx) {
  dukext_check(ctx, uv_tty_reset_mode());
  return 0;
}

duk_ret_t dukext_tty_get_winsize(duk_context *ctx) {
  uv_tty_t *handle;
  int width, height;

  dschema_check(ctx,
                (const dukext_schema_entry[]){{"tty", dukext_is_tty}, {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);
  dukext_check(ctx, uv_tty_get_winsize(handle, &width, &height));
  duk_push_object(ctx);
  duk_push_int(ctx, width);
  duk_put_prop_string(ctx, -2, "width");
  duk_push_int(ctx, height);
  duk_put_prop_string(ctx, -2, "height");
  return 1;
}
