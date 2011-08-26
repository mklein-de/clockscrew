#include "config.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "svnversion.h"
#include "gitversion.h"

#define LOGNAME LIBCLOCKSCREW ": "

#ifdef SVNVERSION
static const char * svn_id = "@(#)"
        LIBCLOCKSCREW " " PACKAGE_VERSION " (" SVNVERSION "), compiled " __DATE__ " " __TIME__;
#endif

#ifdef GITVERSION
static const char * git_id = "@(#)"
        LIBCLOCKSCREW " " PACKAGE_VERSION " (" GITVERSION "), compiled " __DATE__ " " __TIME__;
#endif

struct clockfuncs
{
    time_t(*time)(time_t *);
#ifdef HAVE_GETTIMEOFDAY
    int(*gettimeofday)(struct timeval *, void *);
#endif
#ifdef HAVE_CLOCK_GETTIME
    int(*clock_gettime)(clockid_t, struct timespec *);
#endif
};

static struct clockfuncs screwed_funcs;
static struct clockfuncs real_funcs;
static const struct clockfuncs * curr_funcs;

pthread_mutex_t funcs_mutex = PTHREAD_MUTEX_INITIALIZER;

#define CALL_UNSCREWED(F) \
    pthread_mutex_lock(&funcs_mutex); \
    curr_funcs = &real_funcs; \
    F; \
    curr_funcs = &screwed_funcs; \
    pthread_mutex_unlock(&funcs_mutex);

static long diff_secs;

#ifdef HAVE_CLOCK_GETTIME
int screwed_clock_gettime(clockid_t clk_id, struct timespec * tv)
{
    int ret;
    CALL_UNSCREWED(ret = clock_gettime(clk_id, tv))
    tv->tv_sec += diff_secs;
    return ret;
}

int clock_gettime(clockid_t clk_id, struct timespec * tv)
{
    return curr_funcs->clock_gettime(clk_id, tv);
}
#endif /* HAVE_CLOCK_GETTIME */


#ifdef HAVE_GETTIMEOFDAY
int screwed_gettimeofday(struct timeval * tp, void * tzp)
{
    int ret;
    CALL_UNSCREWED(ret = gettimeofday(tp, tzp));
    tp->tv_sec += diff_secs;
    return ret;
}

int gettimeofday(struct timeval * tp, void * tzp)
{
    return curr_funcs->gettimeofday(tp, tzp);
}
#endif /* HAVE_GETTIMEOFDAY */


time_t screwed_time(time_t * t)
{
    time_t ret;
    CALL_UNSCREWED(ret = time(t));
    ret += diff_secs;
    if (t)
    {
        *t = ret;
    }
    return ret;
}

time_t time(time_t * t)
{
    return curr_funcs->time(t);
}

static void __attribute__ ((constructor)) init_func()
{
    const char *env_str;
    char *unit;
    struct tm abstime;
    time_t now;

#ifdef SVNVERSION
    svn_id = svn_id;
#endif

#ifdef GITVERSION
    git_id = git_id;
#endif

#ifdef HAVE_CLOCK_GETTIME
    real_funcs.clock_gettime = dlsym(RTLD_NEXT, "clock_gettime");
    screwed_funcs.clock_gettime = screwed_clock_gettime;
#endif
#ifdef HAVE_GETTIMEOFDAY
    real_funcs.gettimeofday = dlsym(RTLD_NEXT, "gettimeofday");
    screwed_funcs.gettimeofday = screwed_gettimeofday;
#endif
    real_funcs.time = dlsym(RTLD_NEXT, "time");
    screwed_funcs.time = screwed_time;

    curr_funcs = &screwed_funcs;

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
