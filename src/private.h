/*
private.h - watchdog
Modified 2021-11-30
*/

#ifndef WD_PRIVATE_H
#define WD_PRIVATE_H

/* Header-specific includes. */
#include <stdbool.h>
#include <assert.h>
#define LOGGING_ID L"watchdog"
#define LOGGING_WIDE
#include "../logging/logging.h"

/*
*** Constants.
*/

#define WD_MSG_OUT_OF_MEMORY "Out of memory."
#define WD_MSG_FREE_UNTRACKED_MEMORY "Freeing untracked memory."
#define WD_MSG_NOT_FREED "Memory not freed."
#define WD_MSG_ALL_FREED "All memory freed."
#define WD_MSG_ATEXIT "atexit failed. Call wd_restrain manually."
#define WD_MSG_SIZE_0 "Size is 0."

#define WD_STD_PARAMS char *file, size_t line
#define WD_STD_PARAMS_PASS file, line
#define WD_STD_ARGS __FILE__, __LINE__

/*
*** Types.
*/

typedef struct _wd_point {
  char *file;
  int line;
} wd_point;

typedef struct _wd_alloc {
  wd_point origin;
  char *memory;
  size_t memory_size;
} wd_alloc;

/*
*** Globals.
*/

extern bool wd_unleashed;

/*
*** Main interface.
*/

void wd_restrain(void);
void wd_unleash(WD_STD_PARAMS);

/*
*** Preprocessor functions.
*/

#define WD_ENSURE_UNLEASHED() \
  if (!wd_unleashed) \
    wd_unleash(WD_STD_PARAMS_PASS);

#endif

// FIXME: Yet to be needed.
// typedef struct _wd_time {
//   int hour;
//   int minute;
//   int second;
// } wd_time;
