#include <stdlib.h>
#include <stdio.h>

#include "device.h"
#include "monitor.h"

static void monitor_get_device_info(struct monitor* mon)
{
  nvml_try(nvmlDeviceGetCount(&mon->dev_count), "Failed to get device count");

  mon->devices = malloc(mon->dev_count * sizeof(struct device));

  for(unsigned i = 0; i < mon->dev_count; ++i) {
    struct device dev;

    nvml_try(nvmlDeviceGetHandleByIndex(i, &dev.handle), "Failed to get device handle");
    nvml_try(nvmlDeviceGetName(dev.handle, dev.name, NVML_DEVICE_NAME_BUFFER_SIZE), "Failed to get device name");
    nvml_try(nvmlDeviceGetPciInfo(dev.handle, &dev.pci), "Failed to get device PCI info");
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
  free(mon);
}
