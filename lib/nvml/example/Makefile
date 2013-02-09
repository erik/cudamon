ARCH   := $(shell getconf LONG_BIT)
ifeq (${ARCH},32)
  NVML_LIB := ../lib/
else ifeq (${ARCH},64)
  NVML_LIB := ../lib64/
else
 $(error Unknown architecture!)
endif

CFLAGS  := -I ../include
LDFLAGS := -lnvidia-ml -L $(NVML_LIB)

example: example.o
	$(CC) $(LDFLAGS) $< -o $@

clean:
	-@rm -f example.o
	-@rm -f example
