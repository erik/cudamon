#pragma once

#ifndef _MONITOR_H
#define _MONITOR_H

enum feature {
  TEMPERATURE  = 1 << 0,
  COMPUTE_MODE = 1 << 1,
  POWER_USAGE  = 1 << 2,
  MEMORY_INFO  = 1 << 3,
  CLOCK_INFO   = 1 << 4
};

struct device {
  unsigned index;

  nvmlDevice_t handle;

  nvmlPciInfo_t pci;
  nvmlComputeMode_t compute_mode;
  nvmlMemory_t memory;
  nvmlEventSet_t event_set;

  // In Celsius
  unsigned temperature;

  // In milliwatts
  unsigned power_usage;

  // Maximum clock speeds, in MHz
  nvmlClockType_t clock[NVML_CLOCK_COUNT], max_clock[NVML_CLOCK_COUNT];

  char name[NVML_DEVICE_NAME_BUFFER_SIZE];
  char serial[NVML_DEVICE_SERIAL_BUFFER_SIZE];
  char uuid[NVML_DEVICE_UUID_BUFFER_SIZE];

  // Bitmask of enum feature
  unsigned feature_support;
};

struct monitor {
  // How long should we wait before polling the devices again? In milliseconds.
  unsigned update_interval;
  // Whether or not the monitor should continue running
  int active;

  char driver_version[NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE];
  char nvml_version[NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE];
  char hostname[64];

  // When we last updated this data
  time_t last_update;

  unsigned dev_count;
  struct device* devices;
};

struct monitor* monitor_new(void);
void monitor_start(struct monitor* mon);
void monitor_destroy(struct monitor*);

#endif /* _MONITOR_H */
