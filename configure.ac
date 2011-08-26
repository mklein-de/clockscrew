AC_INIT([clockscrew], [1.2])
AM_INIT_AUTOMAKE
AM_DISABLE_STATIC

AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_PROG_LIBTOOL

AC_MSG_CHECKING(for shared library extension)
module=no
eval so_ext=$shrext_cmds
AC_MSG_RESULT($so_ext)

LIBCLOCKSCREW="lib${PACKAGE}${so_ext}"
AC_SUBST(LIBCLOCKSCREW)
AC_DEFINE_UNQUOTED(LIBCLOCKSCREW, "$LIBCLOCKSCREW", [libclockscrew file name])

AC_MSG_CHECKING(for prefix)
if test "x$prefix" = "xNONE"; then
    my_prefix=$ac_default_prefix
else
    my_prefix=$prefix
fi
AC_MSG_RESULT($my_prefix)

AC_MSG_CHECKING(for libdir)
eval LIBDIR=${libdir}
LIBDIR=`echo ${LIBDIR} | sed "s+^NONE/+$my_prefix/+"`
AC_MSG_RESULT($LIBDIR)
AC_SUBST(LIBDIR)

# dynamic loader variables
AC_MSG_CHECKING(for dynamic loader variables)
case `uname -s` in
	Darwin)
	PRELOAD_VAR="DYLD_INSERT_LIBRARIES"
	EXTRA_ENV="DYLD_FORCE_FLAT_NAMESPACE=1; export DYLD_FORCE_FLAT_NAMESPACE"
	;;
	SunOS|Linux)
	PRELOAD_VAR="LD_PRELOAD"
	EXTRA_ENV=""
	;;
	*)
	AC_MSG_WARN(unknown platform, assuming LD_PRELOAD)
	PRELOAD_VAR="LD_PRELOAD"
	EXTRA_ENV=""
	;;
esac
AC_MSG_RESULT($PRELOAD_VAR)
AC_SUBST(PRELOAD_VAR)
AC_SUBST(EXTRA_ENV)

# name of clockscrew environment variable
SCREW_ENV="CLOCKSCREW"
AC_SUBST(SCREW_ENV)
AC_DEFINE_UNQUOTED(SCREW_ENV, "$SCREW_ENV", [Name of environment variable])

AC_CHECK_FUNCS([localtime_r strtol mktime],,
	       AC_MSG_ERROR(A required function was not found))

AC_CHECK_FUNCS([gettimeofday clock_gettime])
AC_SEARCH_LIBS(clock_gettime, rt, AC_DEFINE(HAVE_CLOCK_GETTIME, 1))
AC_SEARCH_LIBS(dlsym, dl)

AC_MSG_CHECKING(for RTLD_NEXT)
AC_TRY_COMPILE([#include <dlfcn.h>], [RTLD_NEXT;], AC_MSG_RESULT(yes), 
	AC_TRY_COMPILE([#define _GNU_SOURCE
			#include <dlfcn.h>], [RTLD_NEXT;],
				CFLAGS="$CFLAGS -D_GNU_SOURCE"
				AC_MSG_RESULT(needs _GNU_SOURCE),
				AC_MSG_ERROR(no)))

AC_OUTPUT([Makefile clockscrew])
