#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include <pthread.h>

#include "nvml.h"

int main(void)
{

  nvmlReturn_t result;

  if((result = nvmlInit()) != NVML_SUCCESS) {
    fprintf(stderr, "Failed to initialize NVML: %s\n", nvmlErrorString(result));
  }

  return 0;
}
