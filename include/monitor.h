#pragma once

#ifndef _MONITOR_H
#define _MONITOR_H

struct device {
  nvmlDevice_t handle;
  char name[NVML_DEVICE_NAME_BUFFER_SIZE];
  nvmlPciInfo_t pci;

  nvmlComputeMode_t compute_mode;
};

struct monitor {
  unsigned dev_count;
  struct device* devices;
};

struct monitor* monitor_new(void);
void* monitor_thread(void* mon);
void monitor_stop(void);
void monitor_destroy(struct monitor*);

#endif /* _MONITOR_H */
