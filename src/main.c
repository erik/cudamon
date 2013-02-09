#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>
#include <pthread.h>

#include "device.h"
#include "server.h"
#include "monitor.h"


int main(void)
{
  atexit(shutdown_nvml);

  // No point in continuing if we can't even initialize the library.
  if(nvml_try(nvmlInit(), "Failed to initialize NVML"))
    exit(1);

  pthread_t server_thread, monitor_thread;

  pthread_create(&server_thread, NULL, server_start, NULL);
  pthread_create(&monitor_thread, NULL, monitor_start, NULL);

  // Wait for both threads to complete
  (void)pthread_join(server_thread, NULL);
  (void)pthread_join(monitor_thread, NULL);

  return 0;
}
