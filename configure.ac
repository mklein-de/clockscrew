AC_INIT([faketime], m4_esyscmd(svnversion|tr -d '\n'))
AM_INIT_AUTOMAKE
AM_DISABLE_STATIC

AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_PROG_LIBTOOL

AC_LTDL_SHLIBEXT 
AC_SUBST(libltdl_cv_shlibext)

AC_CHECK_FUNCS([gettimeofday localtime_r strtol])
AC_CHECK_HEADERS([sys/time.h])
AC_FUNC_MKTIME 

AC_OUTPUT([Makefile faketime])
