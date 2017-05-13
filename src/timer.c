#include "timer.h"
#include "callbacks.h"
#include "dschema.h"
#include "dschema_uv.h"
#include "refs.h"
//#include "timers_data.h"
#include "utils.h"
#define MAX_TIMERS                                                             \
  4096 /* this is quite excessive for embedded use, but good for testing */
#define MIN_DELAY 1.0
#define MIN_WAIT 1.0
#define MAX_WAIT 60000.0
#define MAX_EXPIRYS 10

duk_ret_t dukext_timer_create(duk_context *ctx) {
  uv_timer_t *handle;

  dschema_check(ctx, (const dukext_schema_entry[]){{NULL}});

  handle = duk_push_fixed_buffer(ctx, sizeof(*handle));

  dukext_check(ctx, uv_timer_init(dukext_loop(ctx), handle));
  handle->data = dukext_setup_handle(ctx);

  return 1;
}

duk_ret_t dukext_timer_start(duk_context *ctx) {
  uv_timer_t *handle;
  uint64_t timeout;
  uint64_t repeat;

  dschema_check(ctx,
                (const dukext_schema_entry[]){{"timer", dukext_is_timer},
                                              {"timeout", duk_is_number},
                                              {"repeat", duk_is_number},
                                              {"ontimeout", duk_is_function},
                                              {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);
  timeout = duk_get_uint(ctx, 1);
  repeat = duk_get_uint(ctx, 2);
  dukext_check(ctx, uv_timer_start(handle, dukext_timer_cb, timeout, repeat));
  dukext_store_handler(ctx, handle->data, DUKEXT_TIMEOUT, 3);

  return 0;
}

duk_ret_t dukext_timer_stop(duk_context *ctx) {

  uv_timer_t *handle;

  dschema_check(
      ctx, (const dukext_schema_entry[]){{"timer", dukext_is_timer}, {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);
  dukext_check(ctx, uv_timer_stop(handle));
  return 0;
}

duk_ret_t dukext_async_create(duk_context *ctx) {
  uv_async_t *handle;

  dschema_check(ctx, (const dukext_schema_entry[]){
                         {"ontimeout", duk_is_function}, {NULL}});

  handle = duk_push_fixed_buffer(ctx, sizeof(*handle));

  dukext_check(ctx, uv_async_init(dukext_loop(ctx), handle, dukext_async_cb));
  handle->data = dukext_setup_handle(ctx);

  // duk_pop(ctx);
  dukext_store_handler(ctx, handle->data, DUKEXT_TIMEOUT, -2);

  dukext_check(ctx, uv_async_send(handle));

  return 1;
}
duk_ret_t dukext_async_start(duk_context *ctx) {

  uv_timer_t *handle;

  dschema_check(
      ctx, (const dukext_schema_entry[]){{"timer", dukext_is_async}, {NULL}});

  handle = duk_get_buffer(ctx, 0, NULL);

  return 0;
}

void dukext_timer_cleanup(duk_context *ctx) {

  duk_push_heap_stash(ctx);
  duk_get_prop_string(ctx, -1, "refs");

  duk_enum(ctx, -1, DUK_ENUM_ARRAY_INDICES_ONLY);
  uv_timer_t *handle;
  while (duk_next(ctx, -1, 1)) {
    // [ ... enum key ]
    if (dukext_is_timer(ctx, -1)) {

      const char *key = duk_get_string(ctx, -2);
      handle = duk_get_buffer(ctx, -1, NULL);

      dukext_check(ctx, uv_timer_stop(handle));

      duk_dup(ctx, -4);                       // refs
      duk_get_prop_string(ctx, -1, "splice"); // load arr.splice onto stack
      duk_dup(ctx, -2);                       // this
      duk_push_string(ctx, key);              // start
      duk_push_int(ctx, 1);                   // deleteCount
      duk_call_method(ctx, 2);                // nargs = 3
      duk_pop_2(ctx);                         // retval+refs
    }

    duk_pop_2(ctx); // pop_key and value
  }

  duk_pop_3(ctx);
}