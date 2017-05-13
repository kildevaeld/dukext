#ifndef DUKEXT_TCP_H
#define DUKEXT_TCP_H

#include "dukext.h"

duk_ret_t dukext_new_tcp(duk_context *ctx);
duk_ret_t dukext_tcp_open(duk_context *ctx);
duk_ret_t dukext_tcp_nodelay(duk_context *ctx);
duk_ret_t dukext_tcp_keepalive(duk_context *ctx);
duk_ret_t dukext_tcp_simultaneous_accepts(duk_context *ctx);
duk_ret_t dukext_tcp_bind(duk_context *ctx);
duk_ret_t dukext_tcp_getsockname(duk_context *ctx);
duk_ret_t dukext_tcp_getpeername(duk_context *ctx);
duk_ret_t dukext_tcp_connect(duk_context *ctx);

#endif