#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "nvml.h"

#include "device.h"
#include "monitor.h"

// Build the set of device features
static void get_device_features(struct device* dev)
{
  if(nvmlDeviceGetTemperature(dev->handle, NVML_TEMPERATURE_GPU,
                              &dev->temperature) == NVML_SUCCESS) {
    dev->feature_support |= TEMPERATURE;
  }

  if(nvmlDeviceGetMemoryInfo(dev->handle, &dev->memory) == NVML_SUCCESS) {
    dev->feature_support |= MEMORY_INFO;
  }

  if(nvmlDeviceGetPowerUsage(dev->handle, &dev->power_usage) == NVML_SUCCESS) {
    dev->feature_support |= POWER_USAGE;
  }

  if(nvmlDeviceGetClockInfo(dev->handle, NVML_CLOCK_GRAPHICS,
                            &dev->clock[NVML_CLOCK_GRAPHICS]) == NVML_SUCCESS &&

     nvmlDeviceGetClockInfo(dev->handle, NVML_CLOCK_SM,
                            &dev->clock[NVML_CLOCK_SM]) == NVML_SUCCESS &&

     nvmlDeviceGetClockInfo(dev->handle, NVML_CLOCK_COUNT,
                            &dev->clock[NVML_CLOCK_COUNT]) == NVML_SUCCESS) {

    dev->feature_support |= CLOCK_INFO;
  }

  if(nvmlDeviceGetFanSpeed(dev->handle, &dev->fan) == NVML_SUCCESS) {
    dev->feature_support |= FAN_INFO;
  }

  if(nvmlDeviceGetUtilizationRates(dev->handle, &dev->util) == NVML_SUCCESS) {
    dev->feature_support |= UTILIZATION_INFO;
  }
}

static void init_device_info(struct monitor* mon)
{
  gethostname(mon->hostname, 64);

  NVML_TRY(nvmlSystemGetDriverVersion(mon->driver_version,
                                      sizeof(mon->driver_version)));
  NVML_TRY(nvmlSystemGetNVMLVersion(mon->nvml_version,
                                    sizeof(mon->nvml_version)));

  NVML_TRY(nvmlDeviceGetCount(&mon->dev_count));

  mon->devices = calloc(mon->dev_count, sizeof(struct device));

  for(unsigned i = 0; i < mon->dev_count; ++i) {
    struct device dev;
    memset(&dev, 0, sizeof(struct device));

    dev.index = i;

    NVML_TRY(nvmlDeviceGetHandleByIndex(i, &dev.handle));

    NVML_TRY(nvmlDeviceGetName(dev.handle, dev.name, sizeof(dev.name)));
    NVML_TRY(nvmlDeviceGetSerial(dev.handle, dev.serial, sizeof(dev.serial)));
    NVML_TRY(nvmlDeviceGetUUID(dev.handle, dev.uuid, sizeof(dev.uuid)));

    NVML_TRY(nvmlDeviceGetPciInfo(dev.handle, &dev.pci));
    NVML_TRY(nvmlDeviceGetMemoryInfo(dev.handle, &dev.memory));

    unsigned long long event_types;
    NVML_TRY(nvmlEventSetCreate(&dev.event_set));
    if(0 == NVML_TRY(nvmlDeviceGetSupportedEventTypes(dev.handle, &event_types))) {
      NVML_TRY(nvmlDeviceRegisterEvents(dev.handle, event_types, dev.event_set));
    } else {
      dev.event_set = NULL;
    }

    for(nvmlClockType_t type = NVML_CLOCK_GRAPHICS; type < NVML_CLOCK_COUNT;
        ++type) {
      if(NVML_TRY(nvmlDeviceGetMaxClockInfo(dev.handle, type,
                                            &dev.max_clock[type])))
        break;
    }

    get_device_features(&dev);

    mon->devices[i] = dev;
  }

  mon->last_update = time(NULL);
}

static void update_device_info(struct monitor* mon)
{
  // TODO: NVML is thread safe, and the order we grab GPU information
  // here doesn't particularly matter, so might as well take advantage
  // of parallelism here.

  unsigned i;

  for(i = 0; i < mon->dev_count; ++i) {
    struct device* dev = &mon->devices[i];

    if(dev->feature_support & MEMORY_INFO) {
      NVML_TRY(nvmlDeviceGetMemoryInfo(dev->handle, &dev->memory));
    }

    if(dev->feature_support & TEMPERATURE) {
      NVML_TRY(nvmlDeviceGetTemperature(dev->handle, NVML_TEMPERATURE_GPU,
                                        &dev->temperature));
    }

    if(dev->feature_support & POWER_USAGE) {
      NVML_TRY(nvmlDeviceGetPowerUsage(dev->handle, &dev->power_usage));
    }

    if(dev->feature_support & CLOCK_INFO) {
      for(nvmlClockType_t type = NVML_CLOCK_GRAPHICS; type < NVML_CLOCK_COUNT;
          ++type) {
        NVML_TRY(nvmlDeviceGetClockInfo(dev->handle, type, &dev->clock[type]));
      }
    }

    if(dev->feature_support & FAN_INFO) {
      NVML_TRY(nvmlDeviceGetFanSpeed(dev->handle, &dev->fan));
    }

    if(dev->event_set != NULL) {
      nvmlEventData_t data;

      NVML_TRY(nvmlEventSetWait(dev->event_set, &data, 1));

      // TODO: Do something with the returned information.
    }
  }

  mon->last_update = time(NULL);
}

struct monitor* monitor_new(void)
{
  struct monitor* mon = calloc(1, sizeof(struct monitor));

  // Half a second is a decent default
  mon->update_interval = 500;

  mon->active = 1;

  // Fetch the basic info that should hold true throughout execution
  init_device_info(mon);

  return mon;
}

void monitor_start(struct monitor* mon)
{
  while(mon->active) {
    update_device_info(mon);

    usleep(mon->update_interval * 1000);
  }

}

void monitor_destroy(struct monitor* mon)
{
  for(unsigned i = 0; i < mon->dev_count; ++i) {
    struct device* dev = &mon->devices[i];

    NVML_TRY(nvmlEventSetFree(dev->event_set));
  }

  free(mon->devices);
  free(mon);
}
