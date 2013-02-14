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

static const char* ajax_reply_start =
  "HTTP/1.1 200 OK\r\n"
  "Cache: no-cache\r\n"
  "Content-Type: application/x-javascript\r\n"
  "\r\n";

static struct monitor* monitor = NULL;

// TODO: Some of these values are static and only need to be sent once.
static void ajax_send_update(struct mg_connection *conn)
{
  mg_printf(conn, "%s", ajax_reply_start);

  mg_printf(conn, "{");

  mg_printf(conn, "'driver_version': '%s',", monitor->driver_version);
  mg_printf(conn, "'nvml_version': '%s',", monitor->nvml_version);

  mg_printf(conn, "'devices' : [");

  for(unsigned i = 0; i < monitor->dev_count; ++i) {
    mg_printf(conn, "{");

    struct device dev = monitor->devices[i];

    mg_printf(conn, "'index':%d,", dev.index);
    mg_printf(conn, "'name':'%s',", dev.name);
    mg_printf(conn, "'serial':'%s',", dev.serial);
    mg_printf(conn, "'uuid':'%s',", dev.uuid);

    if(dev.feature_support & TEMPERATURE)
      mg_printf(conn, "'temperature':'%d',", dev.temperature);

    mg_printf(conn, "},");
  }

  mg_printf(conn, "],");

  mg_printf(conn, "}");
}

static int begin_request_handler(struct mg_connection *conn)
{
  const struct mg_request_info* req_info = mg_get_request_info(conn);
  int processed = 1;

  if(strcmp(req_info->uri, "/ajax/update") == 0) {
    ajax_send_update(conn);
  } else if(strcmp(req_info->uri, "/query/info") == 0) {
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

void server_start(struct server* srv)
{
  if(monitor != NULL) {
    fprintf(stderr, "This shouldn't have happened");
    exit(1);
  }

  monitor = srv->mon;

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
