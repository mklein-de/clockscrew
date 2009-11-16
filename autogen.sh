#!/bin/sh
glibtoolize
aclocal
autoheader
autoconf
automake --add-missing
