#include "config.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define LOGNAME "lib" PACKAGE LT_MODULE_EXT ": "

static int(*real_gettimeofday)(struct timeval *, void*);
static long diff_secs;

int gettimeofday(struct timeval * tp, void * tzp)
{
    int ret;
    ret = real_gettimeofday(tp, tzp);
    tp->tv_sec += diff_secs;
    return ret;
}

static void __attribute__ ((constructor)) init_func()
{
    const char *env_str;
    char *unit;
    struct tm abstime;
    time_t now;

    real_gettimeofday = dlsym(RTLD_NEXT, "gettimeofday");

    env_str = getenv(SCREW_ENV);
    if (env_str && *env_str)
    {
        if (*env_str == '@')
        {
            now = time(NULL);
            localtime_r(&now, &abstime);
            static const char *date_fmts[] =
            {
                "@%Y-%m-%d %H:%M:%S",
                "@%Y-%m-%d %H:%M",
                "@%Y-%m-%d",
                "@%Y",
                0
            }, **fmt;

            for (fmt = date_fmts; *fmt; fmt++)
            {
                const char * p;
                if ((p = strptime(env_str, *fmt, &abstime)) && !*p)
                {
                    diff_secs = mktime(&abstime) - now;
                    break;
                }
            }

            if (!*fmt)
            {
                fprintf(stderr, LOGNAME SCREW_ENV " doesn't contain a valid date string: \"%s\"\n", env_str);
            }
        }
        else
        {
            diff_secs = strtol(env_str, &unit, 10);
            switch(*unit)
            {
                case 'y': diff_secs *= 365;
                case 'd': diff_secs *= 24;
                case 'h': diff_secs *= 60;
                case 'm': diff_secs *= 60;
                case 's':
                case 0:
                          break;

                default:
                          fprintf(stderr, LOGNAME "invalid offset unit: \"%c\"\n", *unit);
                          break;
            }

            if (*unit && *(unit+1))
            {
                fprintf(stderr, LOGNAME "ignoring trailing chars: \"%s\"\n", unit+1);
            }
        }
    }
}
