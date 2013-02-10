#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "device.h"
#include "monitor.h"

static void init_device_info(struct monitor* mon)
{
  NVML_TRY(nvmlSystemGetDriverVersion(mon->driver_version,
                                      sizeof(mon->driver_version)));
  NVML_TRY(nvmlSystemGetNVMLVersion(mon->nvml_version,
                                    sizeof(mon->nvml_version)));

  NVML_TRY(nvmlDeviceGetCount(&mon->dev_count));

  mon->devices = malloc(mon->dev_count * sizeof(struct device));

  for(unsigned i = 0; i < mon->dev_count; ++i) {
    struct device dev;

    NVML_TRY(nvmlDeviceGetHandleByIndex(i, &dev.handle));

    NVML_TRY(nvmlDeviceGetName(dev.handle, dev.name, sizeof(dev.name)));
    NVML_TRY(nvmlDeviceGetSerial(dev.handle, dev.serial, sizeof(dev.serial)));

    NVML_TRY(nvmlDeviceGetPciInfo(dev.handle, &dev.pci));
    NVML_TRY(nvmlDeviceGetMemoryInfo(dev.handle, &dev.memory));

    mon->devices[i] = dev;
  }
}

static void update_device_info(struct monitor* mon)
{
  // NVML is thread safe, and the order we grab GPU information here
  // doesn't particularly matter, so might as well take advantage of
  // parallelism here.

  unsigned i;

# pragma omp parallel for private(i)
  for(i = 0; i < mon->dev_count; ++i) {
    struct device* dev = &mon->devices[i];

    NVML_TRY(nvmlDeviceGetMemoryInfo(dev->handle, &dev->memory));
  }

}

struct monitor* monitor_new(void)
{
  struct monitor* mon = malloc(sizeof(struct monitor));

  // Half a second is a decent default
  mon->update_interval = 500;

  mon->active = 1;

  // Fetch the basic info that should hold true throughout execution
  init_device_info(mon);

  return mon;
}

void* monitor_thread(void* ptr)
{
  struct monitor* mon = (struct monitor*) ptr;

  while(mon->active) {
    update_device_info(mon);

    usleep(mon->update_interval * 1000);
  }

  return NULL;
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
