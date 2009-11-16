UNAME:=$(shell uname -s)

ifeq (Darwin, $(UNAME))
	SO_EXT:=dylib
	LDFLAGS:=-dynamiclib
else
	SO_EXT:=so
	LDFLAGS:=-shared
endif

CFLAGS:=-fpic
LDFLAGS+=-init _libfaketime_init

.PHONY: all clean
all: libfaketime.$(SO_EXT)

libfaketime.$(SO_EXT): faketime.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ faketime.c

clean:
	rm -f libfaketime.$(SO_EXT) core *~
