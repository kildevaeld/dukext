#ifndef DUKEXT_DUKEXT_H
#define DUKEXT_DUKEXT_H

#include "duktape.h"
#include "uv.h"

// There are two slots for holding callbacks.  One is for the CLOSED event.
// The other slot is for all others since they never conflict in practice.
#define dukext_callback_id int
#define DUKEXT_CLOSED 0
#define DUKEXT_TIMEOUT 1
#define DUKEXT_PREPARE 1
#define DUKEXT_IDLE 1
#define DUKEXT_CHECK 1
#define DUKEXT_ASYNC 1
#define DUKEXT_POLL 1
#define DUKEXT_SIGNAL 1
#define DUKEXT_EXIT 1
#define DUKEXT_CONNECTION 1
#define DUKEXT_READ 1
#define DUKEXT_RECV 1
#define DUKEXT_FS_EVENT 1
#define DUKEXT_FS_POLL 1

// Ref for userdata and event callbacks
typedef struct {
  int ref;
  int context;
  int callbacks[2];
  duk_context *ctx;
} dukext_handle_t;

typedef struct {
  int req_ref; // ref for uv_req_t's userdata
  int context;
  int callback_ref; // ref for callback
  int data_ref;
  void *data; // extra data
  duk_context *ctx;
} dukext_req_t;

struct dukext_s {
  duk_context *ctx;
  uv_loop_t *loop;
};

typedef struct dukext_s dukext_t;

int dukext_init(dukext_t *req, uv_loop_t *loop);

duk_ret_t dukext_run(dukext_t *req, const char *source);

void dukext_cleanup(dukext_t *req);

#endif