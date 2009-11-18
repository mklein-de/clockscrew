#!/bin/sh
type -p glibtoolize >/dev/null && glibtoolize || libtoolize
aclocal
autoheader
autoconf
automake --add-missing --copy
