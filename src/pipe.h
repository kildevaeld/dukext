#ifndef DUKEXT_PIPE_H
#define DUKEXT_PIPE_H

#include "dukext.h"

duk_ret_t dukext_new_pipe(duk_context *ctx);
duk_ret_t dukext_pipe_open(duk_context *ctx);
duk_ret_t dukext_pipe_bind(duk_context *ctx);
duk_ret_t dukext_pipe_connect(duk_context *ctx);
duk_ret_t dukext_pipe_getsockname(duk_context *ctx);
duk_ret_t dukext_pipe_pending_instances(duk_context *ctx);
duk_ret_t dukext_pipe_pending_count(duk_context *ctx);
duk_ret_t dukext_pipe_pending_type(duk_context *ctx);

#endif
