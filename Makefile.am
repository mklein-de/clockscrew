ACLOCAL_AMFLAGS = -I m4

bin_SCRIPTS = clockscrew
lib_LTLIBRARIES = libclockscrew.la
libclockscrew_la_SOURCES = libclockscrew.c
libclockscrew_la_DEPENDENCIES = svnversion.h
libclockscrew_la_LDFLAGS = -dynamiclib -avoid-version

EXTRA_DIST = svnversion.h

.PHONY: svnversion.h
svnversion.h:
	[ \! -d .svn ] || \
		( echo "#define SVNVERSION \"`svnversion`\"" > $@.new && \
		  ( cmp $@ $@.new && rm $@.new || mv $@.new $@ ) )

