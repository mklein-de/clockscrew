#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define LIBFAKETIME "libfaketime." SO_EXT ": "

static int(*real_gettimeofday)(struct timeval *, void*);
static long faketime_diff;

int gettimeofday(struct timeval * tp, void * tzp)
{
    int ret;
    ret = real_gettimeofday(tp, tzp);
    tp->tv_sec += faketime_diff;
    return ret;
}

void libfaketime_init()
{
    const char *faketime_str;
    char *unit;
    struct tm abstime;
    time_t now;

    real_gettimeofday = dlsym(RTLD_NEXT, "gettimeofday");

    faketime_str = getenv("FAKETIME");
    if (faketime_str && *faketime_str)
    {
        if (*faketime_str == '@')
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
                if ((p = strptime(faketime_str, *fmt, &abstime)) && !*p)
                {
                    faketime_diff = mktime(&abstime) - now;
                    break;
                }
            }

            if (!*fmt)
            {
                fprintf(stderr, LIBFAKETIME "FAKETIME doesn't contain a valid date string: \"%s\"\n", faketime_str);
            }
        }
        else
        {
            faketime_diff = strtol(faketime_str, &unit, 10);
            switch(*unit)
            {
                case 'y': faketime_diff *= 365;
                case 'd': faketime_diff *= 24;
                case 'h': faketime_diff *= 60;
                case 'm': faketime_diff *= 60;
                case 's':
                case 0:
                          break;

                default:
                          fprintf(stderr, LIBFAKETIME "invalid offset unit: \"%c\"\n", *unit);
                          break;
            }

            if (*unit && *(unit+1))
            {
                fprintf(stderr, LIBFAKETIME "ignoring trailing chars: \"%s\"\n", unit+1);
            }
        }
    }
}
