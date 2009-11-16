#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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
    fprintf(stderr, "faketime_str=%s\n", faketime_str);
    if (faketime_str && *faketime_str)
    {
        if (strptime(faketime_str, "@%F %T", &abstime))
        {
            faketime_diff = mktime(&abstime) - time(NULL);
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
            }
        }
    }
}
