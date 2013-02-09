#pragma once

#ifndef _DEVICE_H
#define _DEVICE_H

#include "nvml.h"

// Simple wrapper function to remove boiler plate code of checking
// NVML API return codes.
//
// Returns non-zero on error, 0 otherwise
static inline int nvml_try(nvmlReturn_t ret, const char* msg)
{
  if(ret == NVML_ERROR_NOT_SUPPORTED) {
    fprintf(stderr, "Not supported: %s: %s", msg, nvmlErrorString(ret));
  } else if(ret != NVML_SUCCESS) {
    fprintf(stderr, "Failed: %s: %s", msg, nvmlErrorString(ret));

  } else { return 1; }

  return 1;
}

void shutdown_nvml(void);

#endif /* _DEVICE_H */
