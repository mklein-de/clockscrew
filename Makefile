UNAME:=$(shell uname -s)

ifeq (Darwin, $(UNAME))
	SO_EXT:=dylib
	LDFLAGS:=-dynamiclib
else
	SO_EXT:=so
	LDFLAGS:=-shared
endif

CFLAGS:=-fpic
CPPFLAGS:=-DSO_EXT="\"$(SO_EXT)\""
LDFLAGS+=-init _libfaketime_init

.PHONY: all clean install
all: libfaketime.$(SO_EXT)

libfaketime.$(SO_EXT): faketime.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ faketime.c

clean:
	rm -f libfaketime.$(SO_EXT) core *~
