/*
private.h - watchdog
Modified 2021-11-30
*/

#ifndef WD_PRIVATE_H
#define WD_PRIVATE_H

/* Includes. */
#include <stdbool.h>
#include <assert.h>
#define LOGGING_ID L"watchdog"
#define LOGGING_WIDE
#include "../logging/logging.h"

/* Constants. */
#define WD_MSG_OUT_OF_MEMORY "Out of memory."
#define WD_MSG_FREE_UNKNOWN_MEMORY "Freeing unknown memory."
#define WD_STD_PARAMS char *file, size_t line
#define WD_STD_PARAMS_PASS file, line
#define WD_STD_ARGS __FILE__, __LINE__

typedef struct _wd_point {
  char *file;
  int line;
} wd_point;

typedef struct _wd_alloc {
  wd_point origin;
  char *memory;
  size_t memory_size;
} wd_alloc;

extern bool wd_unleashed;

void wd_restrain(void);
void wd_unleash(WD_STD_PARAMS);

/* Preprocessor Functions. */
#define WD_ENSURE_UNLEASHED() \
  if (!wd_unleashed) \
    wd_unleash(WD_STD_PARAMS_PASS);

// typedef struct _wd_time {
//   int hour;
//   int minute;
//   int second;
// } wd_time;

#endif
