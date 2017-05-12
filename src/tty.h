#ifndef DUKEXT_TTY_H
#define DUKEXT_TTY_H

#include "dukext.h"

duk_ret_t dukext_new_tty(duk_context *ctx);
duk_ret_t dukext_tty_set_mode(duk_context *ctx);
duk_ret_t dukext_tty_reset_mode(duk_context *ctx);
duk_ret_t dukext_tty_get_winsize(duk_context *ctx);

#endif
