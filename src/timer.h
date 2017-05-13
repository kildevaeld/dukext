#ifndef DUKEXT_TIMER_H
#define DUKEXT_TIMER_H

#include "dukext.h"

duk_ret_t dukext_timer_create(duk_context *ctx);
duk_ret_t dukext_timer_start(duk_context *ctx);
duk_ret_t dukext_timer_stop(duk_context *ctx);
duk_ret_t dukext_async_create(duk_context *ctx);
duk_ret_t dukext_async_start(duk_context *ctx);

void dukext_timer_cleanup(duk_context *ctx);

#endif