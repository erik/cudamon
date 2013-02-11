#include <stdlib.h>
#include <string.h>

#include "nvml.h"
#include "mongoose.h"

#include "server.h"
#include "monitor.h"

static const char* MG_OPTIONS[] = {
  "listening_ports", "4321",
  "document_root", "www/",
  NULL
};

static int begin_request_handler(struct mg_connection *conn)
{
  const struct mg_request_info* req_info = mg_get_request_info(conn);
  int processed = 1;

  if(strcmp(req_info->uri, "/query/asdf") == 0) {
    // TODO
  } else {
    processed = 0;
  }

  return processed;
}

struct server* server_new(struct monitor* mon)
{
  struct server* srv = malloc(sizeof(struct server));

  srv->mon = mon;

  memset(&srv->callbacks, 0, sizeof(struct mg_callbacks));

  srv->callbacks.begin_request = begin_request_handler;

  return srv;
}

// Run in a new thread
void server_start(struct server* srv)
{
  if((srv->ctx = mg_start(&srv->callbacks, NULL, MG_OPTIONS)) == NULL) {
    fprintf(stderr, "Server start failed, bailing out.");

    srv->mon->active = 0;

    return;
  }

}

void server_destroy(struct server* srv)
{
  mg_stop(srv->ctx);

  free(srv);
}
