ACLOCAL_AMFLAGS = -I m4

bin_SCRIPTS = clockscrew
lib_LTLIBRARIES = libclockscrew.la
libclockscrew_la_SOURCES = libclockscrew.c
libclockscrew_la_DEPENDENCIES = svnversion.h
libclockscrew_la_LDFLAGS = -dynamiclib -avoid-version

EXTRA_DIST = svnversion.h
BUILT_SOURCES = update_svnversion update_gitversion

.PHONY: update_svnversion
update_svnversion svnversion.h:
	>svnversion.h
	[ \! -d .svn ] || \
		( echo "#define SVNVERSION \"`svnversion -c | cut -f2 -d:`\"" > svnversion.h.new && \
		  ( [ -f svnversion.h ] && cmp svnversion.h svnversion.h.new > /dev/null &&  \
		    rm svnversion.h.new || mv svnversion.h.new svnversion.h ) )

.PHONY: update_gitversion
update_gitversion gitversion.h:
	>gitversion.h
	[ \! -d .git ] || \
		( echo "#define GITVERSION \"`git log -1 --pretty=format:%H`\"" > gitversion.h.new && \
		  ( [ -f gitversion.h ] && cmp gitversion.h gitversion.h.new > /dev/null &&  \
		    rm gitversion.h.new || mv gitversion.h.new gitversion.h ) )
