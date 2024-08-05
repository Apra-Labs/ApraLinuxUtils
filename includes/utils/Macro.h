/*
 * Macro.h
 *
 *  Created on: 15-Feb-2023
 *      Author: developer
 */

#ifndef INCLUDES_MACRO_H_
#define INCLUDES_MACRO_H_

#include "ScopeFunction.h"
#include <sys/time.h>
/*
 * Timestamp Functions
 */
#define GTTIME(ret) { \
    struct timeval tv; \
    gettimeofday(&tv, NULL); \
    ret = (tv.tv_sec); \
    ret = ret * 1000000; \
    ret+=tv.tv_usec;\
}

#define TIMEOP(n,x) { \
    int64_t st,et; \
    GTTIME(st); \
    {x;} \
    GTTIME(et); \
    printf("%s -> %lld\n",n,et-st); \
}

#define TIMEOP_LOG(n,x,d) { \
    int64_t st,et; \
    GTTIME(st); \
    {x;} \
    GTTIME(et); \
    if(d) printf("%s -> %ld\n",n,et-st); \
}

#define CURRTIME(var) int64_t var;\
    {    GTTIME(var);\
    }

#define LOG_CURRTIME(){\
    CURRTIME(t);\
    printf("%s : %lld\n",__func__,t);\
}

#define XPROCESSTIME(var,x)     {    x;\
    }

#define PROCESSTIME(var,x) int64_t var;\
    {    int64_t st,et;\
        GTTIME(st);\
        x;\
        GTTIME(et);\
        var=et-st;\
    }
#define PRINTFLOW apra::ScopeFunction s(string(__func__));

#define GTMONOTIMENS(ret) \
	{ \
	struct timespec ts; \
	clock_gettime(CLOCK_MONOTONIC, &ts); \
	ret=(ts.tv_sec); \
    ret=ret * 1000000000; \
    ret+=ts.tv_nsec;\
	}

#define MONOTIMEUS(ret) \
{ \
	GTMONOTIMENS(ret); \
	ret = ret / 1000; \
}

#define MONOCURRTIME(ret) int64_t ret; \
{ \
	MONOTIMEUS(ret); \
}

#endif /* INCLUDES_MACRO_H_ */
