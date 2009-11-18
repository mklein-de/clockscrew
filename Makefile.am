ACLOCAL_AMFLAGS = -I m4

bin_SCRIPTS = clockscrew
lib_LTLIBRARIES = libclockscrew.la
libclockscrew_la_SOURCES = libclockscrew.c
libclockscrew_la_DEPENDENCIES = svnversion.h
libclockscrew_la_LDFLAGS = -dynamiclib -avoid-version

EXTRA_DIST = svnversion.h
BUILT_SOURCES = update_svnversion

.PHONY: update_svnversion
update_svnversion svnversion.h:
	[ \! -d .svn ] || \
		( echo "#define SVNVERSION \"`svnversion -c | cut -f2 -d:`\"" > svnversion.h.new && \
		  ( [ -f svnversion.h ] && cmp svnversion.h svnversion.h.new > /dev/null &&  \
		    rm svnversion.h.new || mv svnversion.h.new svnversion.h ) )
