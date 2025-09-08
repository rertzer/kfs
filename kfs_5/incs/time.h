#ifndef TIME_H
#define TIME_H

#include <stdint.h>

#define MS_IN_SEC 1000U
#define SEC_IN_MIN 60U
#define MIN_IN_HOUR 60U
#define HOUR_IN_DAY 24U
#define MS_IN_DAY (uint32_t)(HOUR_IN_DAY * MIN_IN_HOUR * SEC_IN_MIN * MS_IN_SEC)
#define MS_IN_HOUR (uint32_t)(MIN_IN_HOUR * SEC_IN_MIN * MS_IN_SEC)
#define MS_IN_MIN (uint32_t)(SEC_IN_MIN * MS_IN_SEC)

typedef struct time_s {
	uint32_t days;
	uint32_t hours;
	uint32_t minutes;
	uint32_t seconds;
	uint32_t milliseconds;
} time_t;

#endif
