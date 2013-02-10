#include <stdio.h>

#include "device.h"

// Make sure we de-init NVML before quitting
void shutdown_nvml(void)
{
  NVML_TRY(nvmlShutdown());
}

const char* nvml_error_code_string(nvmlReturn_t ret)
{
  switch(ret) {
  case NVML_SUCCESS:
    return "The operation was successful";
  case NVML_ERROR_UNINITIALIZED:
    return "was not first initialized with nvmlInit()";
  case NVML_ERROR_INVALID_ARGUMENT:
    return "A supplied argument is invalid";
  case NVML_ERROR_NOT_SUPPORTED:
    return "The requested operation is not available on target device";
  case NVML_ERROR_NO_PERMISSION:
    return "The current user does not have permission for operation";
  case NVML_ERROR_ALREADY_INITIALIZED:
    return"Deprecated: Multiple initializations are now allowed through ref counting";
  case NVML_ERROR_NOT_FOUND:
    return "A query to find an object was unsuccessful";
  case NVML_ERROR_INSUFFICIENT_SIZE:
    return "An input argument is not large enough";
  case NVML_ERROR_INSUFFICIENT_POWER:
    return "A device’s external power cables are not properly attached";
  case NVML_ERROR_DRIVER_NOT_LOADED:
    return "NVIDIA driver is not loaded";
  case NVML_ERROR_TIMEOUT:
    return "User provided timeout passed";
  case NVML_ERROR_IRQ_ISSUE:
    return "NVIDIA Kernel detected an interrupt issue with a GPU";
  case NVML_ERROR_LIBRARY_NOT_FOUND:
    return "NVML Shared Library couldn’t be found or loaded";
  case NVML_ERROR_FUNCTION_NOT_FOUND:
    return"Local version of NVML doesn’t implement this function";
  case NVML_ERROR_CORRUPTED_INFOROM:
    return "infoROM is corrupted";
  case NVML_ERROR_UNKNOWN:
    return "An internal driver error occurred";
  }

  return "Unknown error";
}
