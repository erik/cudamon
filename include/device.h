#pragma once

#ifndef _DEVICE_H
#define _DEVICE_H

#include "nvml.h"

// Return string representation of return code
// Strings are directly from NVML documentation
const char* nvml_error_code_string(nvmlReturn_t ret);

// Simple wrapper function to remove boiler plate code of checking
// NVML API return codes.
//
// Returns non-zero on error, 0 otherwise
static inline int nvml_try(nvmlReturn_t ret, const char* fn)
{
  if(ret != NVML_SUCCESS) {
    fprintf(stderr, "%s: %s: %s\n", fn, nvml_error_code_string(ret),
            nvmlErrorString(ret));
    return 1;
  }

  return 0;
}

#define NVML_TRY(code) nvml_try(code, #code)

void shutdown_nvml(void);

#endif /* _DEVICE_H */
