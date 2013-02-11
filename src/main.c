#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include "device.h"
#include "server.h"
#include "monitor.h"

int main(void)
{
  // No point in continuing if we can't even initialize the library.
  if(NVML_TRY(nvmlInit()))
    exit(1);

  atexit(shutdown_nvml);

  struct monitor* mon = monitor_new();
  struct server* srv = server_new(mon);

  // Spin up embedded mongoose server in new thread
  server_start(srv);

  // This function blocks
  monitor_start(mon);

  monitor_destroy(mon);
  server_destroy(srv);

  return 0;
}
