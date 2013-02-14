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

#define JSON_KEY_STRING(key, value) mg_printf(conn, ",\"%s\": \"%s\"", key, value)
#define JSON_KEY_INTEGER(key, value) mg_printf(conn, ",\"%s\": %d", key, value)

// Send one-time information to the client
static void ajax_send_init(struct mg_connection *conn)
{
  mg_printf(conn, "%s", ajax_reply_start);

  mg_printf(conn, "{");

  mg_printf(conn, "\"devices\" : [");
  for(unsigned i = 0; i < monitor->dev_count; ++i) {
    if(i != 0) mg_printf(conn, ",");

    mg_printf(conn, "{");

    struct device dev = monitor->devices[i];

    mg_printf(conn, "\"index\": %d", dev.index);

    JSON_KEY_STRING("name",   dev.name);
    JSON_KEY_STRING("serial", dev.serial);
    JSON_KEY_STRING("uuid",   dev.uuid);

    mg_printf(conn, ",\"pci\": { \"bus\": \"%s\"", dev.pci.busId);
    JSON_KEY_INTEGER("devId", dev.pci.pciDeviceId);
    JSON_KEY_INTEGER("subId", dev.pci.pciSubSystemId);

    mg_printf(conn, "}");

    mg_printf(conn, "}");

  }
  mg_printf(conn, "]");

  JSON_KEY_STRING("driver_version", monitor->driver_version);
  JSON_KEY_STRING("nvml_version", monitor->nvml_version);

  mg_printf(conn, "}");
}

// TODO: Some of these values are static and only need to be sent once.
static void ajax_send_update(struct mg_connection *conn)
{
  mg_printf(conn, "%s", ajax_reply_start);

  mg_printf(conn, "{");

  mg_printf(conn, "\"devices\" : [");

  for(unsigned i = 0; i < monitor->dev_count; ++i) {
    if(i != 0) mg_printf(conn, ",");

    mg_printf(conn, "{");

    struct device dev = monitor->devices[i];
    mg_printf(conn, "\"index\": %d", dev.index);

    if(dev.feature_support & TEMPERATURE)
      JSON_KEY_INTEGER("temperature", dev.temperature);

    if(dev.feature_support & POWER_USAGE)
      JSON_KEY_INTEGER("power", dev.power_usage);

    // Memory is reported in bytes, but we'll just return MiB here.
    if(dev.feature_support & MEMORY_INFO) {
      mg_printf(conn, ",\"memory\": { \"total\": %d",
                (unsigned)(dev.memory.total / 1024 / 1024));

      JSON_KEY_INTEGER("free", (unsigned)(dev.memory.free / 1024 / 1024));
      JSON_KEY_INTEGER("used", (unsigned)(dev.memory.used / 1024 / 1024));

      mg_printf(conn, "}");
    }

    mg_printf(conn, "}");
  }
  mg_printf(conn, "]");

  JSON_KEY_INTEGER("time", (unsigned)monitor->last_update);

  mg_printf(conn, "}");
}

static int begin_request_handler(struct mg_connection *conn)
{
  const struct mg_request_info* req_info = mg_get_request_info(conn);
  int processed = 1;

  if(strcmp(req_info->uri, "/ajax/init") == 0) {
    ajax_send_init(conn);
  } else if(strcmp(req_info->uri, "/ajax/update") == 0) {
    ajax_send_update(conn);
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
