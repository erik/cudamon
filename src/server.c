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

#define JSON_KEY_ARRAY(key) mg_printf(conn, ",\"%s\": [ ", key)
#define JSON_KEY_STRING(key, value) mg_printf(conn, ",\"%s\": \"%s\"", key, value)
#define JSON_KEY_INTEGER(key, value) mg_printf(conn, ",\"%s\": %d", key, value)
#define JSON_STRING(str) mg_printf(conn, ",\"%s\"", str)

// Versions of the macro for the first value in a series (i.e. don't
// print out ',')
#define JSON_KEY_ARRAY_(key) mg_printf(conn, "\"%s\": [ ", key)
#define JSON_KEY_STRING_(key, value) mg_printf(conn, "\"%s\": \"%s\"", key, value)
#define JSON_KEY_INTEGER_(key, value) mg_printf(conn, "\"%s\": %d", key, value)
#define JSON_STRING_(str) mg_printf(conn, "\"%s\"", str)

// Send one-time information to the client
static void ajax_send_init(struct mg_connection *conn)
{
  mg_printf(conn, "%s", ajax_reply_start);

  mg_printf(conn, "{");

  JSON_KEY_ARRAY_("devices");
  for(unsigned i = 0; i < monitor->dev_count; ++i) {
    if(i != 0) mg_printf(conn, ",");

    mg_printf(conn, "{");

    struct device dev = monitor->devices[i];

    JSON_KEY_INTEGER_("index", dev.index);

    JSON_KEY_STRING("name",   dev.name);
    JSON_KEY_STRING("serial", dev.serial);
    JSON_KEY_STRING("uuid",   dev.uuid);

    mg_printf(conn, ",\"pci\": {");
    JSON_KEY_STRING_("bus", dev.pci.busId);
    JSON_KEY_INTEGER("devId", dev.pci.pciDeviceId);
    JSON_KEY_INTEGER("subId", dev.pci.pciSubSystemId);
    mg_printf(conn, "}");

    JSON_KEY_ARRAY("features");

    // We know it can at least print its name
    JSON_STRING_("name");

    if(dev.feature_support & TEMPERATURE)  JSON_STRING("temperature");
    if(dev.feature_support & COMPUTE_MODE) JSON_STRING("compute");
    if(dev.feature_support & POWER_USAGE)  JSON_STRING("power");
    if(dev.feature_support & MEMORY_INFO)  JSON_STRING("memory");
    if(dev.feature_support & CLOCK_INFO)   JSON_STRING("clock");
    if(dev.feature_support & FAN_INFO)     JSON_STRING("fan");

    mg_printf(conn, "]");

    mg_printf(conn, "}");

  }
  mg_printf(conn, "]");

  JSON_KEY_STRING("driver_version", monitor->driver_version);
  JSON_KEY_STRING("nvml_version", monitor->nvml_version);
  JSON_KEY_STRING("host", monitor->hostname);

  mg_printf(conn, "}");
}

// TODO: Some of these values are static and only need to be sent once.
static void ajax_send_update(struct mg_connection *conn)
{
  mg_printf(conn, "%s", ajax_reply_start);

  mg_printf(conn, "{");

  JSON_KEY_ARRAY_("devices");
  for(unsigned i = 0; i < monitor->dev_count; ++i) {
    if(i != 0) mg_printf(conn, ",");

    mg_printf(conn, "{");

    struct device dev = monitor->devices[i];
    JSON_KEY_INTEGER_("index", dev.index);

    JSON_KEY_STRING("name", dev.name);

    if(dev.feature_support & TEMPERATURE)
      JSON_KEY_INTEGER("temperature", dev.temperature);

    if(dev.feature_support & POWER_USAGE)
      JSON_KEY_INTEGER("power", dev.power_usage);

    // Memory is reported in bytes, but we'll just return MiB here.
    if(dev.feature_support & MEMORY_INFO) {
      mg_printf(conn, ",\"memory\": {");

      JSON_KEY_INTEGER_("total", (unsigned)(dev.memory.total / 1024 / 1024));
      JSON_KEY_INTEGER("free", (unsigned)(dev.memory.free / 1024 / 1024));
      JSON_KEY_INTEGER("used", (unsigned)(dev.memory.used / 1024 / 1024));

      mg_printf(conn, "}");
    }

    if(dev.feature_support & CLOCK_INFO) {
      mg_printf(conn, ",\"clock\" : {");

      JSON_KEY_INTEGER_("graphics",
                        dev.clock[NVML_CLOCK_GRAPHICS]);
      JSON_KEY_INTEGER("sm", dev.clock[NVML_CLOCK_SM]);
      JSON_KEY_INTEGER("mem", dev.clock[NVML_CLOCK_MEM]);

      mg_printf(conn, "}");
    }

    if(dev.feature_support & FAN_INFO)
      JSON_KEY_INTEGER("fan", dev.fan);

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
