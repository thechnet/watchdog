/*
dogshed.h - watchdog
Modified 2021-12-04
*/

#ifndef WD_DOGSHED_H
#define WD_DOGSHED_H

/* Header-specific includes. */
#include <stdbool.h>
#include <assert.h>
#include "../logging/escseq.h" // FIXME: It is currently required to include escseq before logging here to avoid defining the escape sequences as wide-character strings.
#define LOGGING_WIDE
#define LOGGING_ID "watchdog"
#include "../logging/logging.h"

/*
*** Constants.
*/

#define WD_MSG_OUT_OF_MEMORY "Out of memory – Wanted %zu(+%zu) b."
#define WD_MSG_OUT_OF_MEMORY_INTERNAL "(Internal) Out of memory – Wanted %zu+%zu b."
#define WD_MSG_UNTRACKED_MEMORY "Untracked memory."
#define WD_MSG_NOT_FREED "Memory not freed."
#define WD_MSG_ALL_FREED "All memory freed."
#define WD_MSG_ATEXIT "atexit failed. Call wd_restrain manually."
#define WD_MSG_SIZE_0 "Size is 0."
#define WD_MSG_INCOMING_NULL "Incoming NULL pointer."
#define WD_MSG_REALLOC_SIZE "Size smaller or same."
#define WD_MSG_TRACK "Tracks."
#define WD_MSG_PADDING "Padding not intact at " LOGGING_WHERE "."
#define WD_MSG_SNAPSHOT "Snapshot changed at " LOGGING_WHERE "."
#define WD_MSG_OUT_OF_BOUNDS "Index out of range. (%d of #%zu)"
#define WD_MSG_SIGNAL "signal failed. Not catching signals."
#define WD_MSG_SIGFPE "Arithmetic error."
#define WD_MSG_SIGILL "Illegal instruction."
#define WD_MSG_SIGSEGV "Segmentation fault."
#define WD_MSG_INCOMING_DANGLING "Incoming dangling pointer (freed at " LOGGING_WHERE ")."
#define WD_MSG_ASSERT "Assertion failed: %s"

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

#define WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(allocator_return_value, alloc_size, size_markup)\
  if (allocator_return_value == NULL) {\
    wd_alerts++;\
    fail_at(WD_STD_ARGS, WD_MSG_OUT_OF_MEMORY_INTERNAL, alloc_size, size_markup);\
  }

#endif
