#pragma once

#ifndef _DEVICE_H
#define _DEVICE_H

#include "nvml.h"

// Simple wrapper function to remove boiler plate code of checking
// NVML API return codes.
//
// Returns 1 on error, 0 otherwise
static inline int nvml_try(nvmlReturn_t ret, const char* msg)
{
  if(ret != NVML_SUCCESS) {
    fprintf(stderr, "%s: %s", msg, nvmlErrorString(ret));
    return 1;
  }

  return 0;
}

void shutdown_nvml(void);

#endif /* _DEVICE_H */
