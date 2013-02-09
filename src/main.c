#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>
#include <pthread.h>

#include "device.h"
#include "server.h"
#include "monitor.h"

int main(void)
{
  // No point in continuing if we can't even initialize the library.
  if(nvml_try(nvmlInit(), "Failed to initialize NVML"))
    exit(1);

  atexit(shutdown_nvml);

  struct monitor* mon = monitor_new();
  struct server* srv = server_new();

  pthread_t server_thr, monitor_thr;

  pthread_create(&server_thr, NULL, server_thread, srv);
  pthread_create(&monitor_thr, NULL, monitor_thread, mon);

  // Wait for both threads to complete
  (void)pthread_join(server_thr, NULL);
  (void)pthread_join(monitor_thr, NULL);

  monitor_destroy(mon);
  server_destroy(srv);

  return 0;
}
