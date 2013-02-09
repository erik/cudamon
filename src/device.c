#include <stdio.h>

#include "device.h"

// Make sure we de-init NVML before quitting
void shutdown_nvml(void)
{
  nvml_try(nvmlShutdown(), "Failed to shutdown NVML");
}
