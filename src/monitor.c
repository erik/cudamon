#include <stdlib.h>
#include <stdio.h>

#include "device.h"
#include "monitor.h"

static void monitor_get_device_info(struct monitor* mon)
{
  NVML_TRY(nvmlDeviceGetCount(&mon->dev_count));

  mon->devices = malloc(mon->dev_count * sizeof(struct device));

  for(unsigned i = 0; i < mon->dev_count; ++i) {
    struct device dev;

    NVML_TRY(nvmlDeviceGetHandleByIndex(i, &dev.handle));
    NVML_TRY(nvmlDeviceGetName(dev.handle, dev.name, NVML_DEVICE_NAME_BUFFER_SIZE));
    NVML_TRY(nvmlDeviceGetPciInfo(dev.handle, &dev.pci));
  }

}

struct monitor* monitor_new(void)
{
  struct monitor* mon = malloc(sizeof(struct monitor));

  // Fetch the basic info that should hold true throughout execution
  monitor_get_device_info(mon);

  return mon;
}

void* monitor_thread(void* _)
{
  return _;
}

void monitor_stop(void)
{

}

void monitor_destroy(struct monitor* mon)
{
  for(unsigned i = 0; i < mon->dev_count; ++i) {
    // TODO
  }

  free(mon->devices);
  free(mon);
}
