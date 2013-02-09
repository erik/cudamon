CSRC := $(shell find src -name "*.c")
COBJ := $(CSRC:.c=.o)
CDEPS := $(CSRC:.c=.d)

OPT += -O2
LFLAGS = -lm
CFLAGS = -ggdb -Wall -Wextra -pedantic -std=c99 -Llib/nvml/lib
IFLAGS = -Ilib/nvml/include

all: cudamond

cudamond: $(COBJ)
	$(CC) $(COBJ) $(LFLAGS) -o cudamond

%.o: %.c
	$(CC) -c -MMD $(CFLAGS) $(IFLAGS) $(OPT) $< -o $@

clean:
	rm -f $(COBJ) $(DEPS) cudamond

todo:
	@find . -type f | xargs egrep -n --color=auto 'XXX|TODO|FIXME'

check-syntax:
	$(CC) -o /dev/null $(CFLAGS) -S $(CHK_SOURCES)
