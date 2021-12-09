/*
dogshed.h - watchdog
Modified 2021-12-09
*/

#ifndef WD_DOGSHED_H
#define WD_DOGSHED_H

/* Header-specific includes. */
#include <stdbool.h>
#include <assert.h>
#ifdef _WIN32
#include <io.h>
#define STDOUT_FILENO 1
#define write(fd, buffer, count) _write(fd, buffer, count)
#else
#include <unistd.h>
#endif
#include "../logging/escseq.h" /* FIXME: It is currently required to include escseq before logging
                                         here to avoid defining the escape sequences as
                                         wide-character strings. */
#define LOGGING_WIDE
#define LOGGING_ID "watchdog"
#include "../logging/logging.h"

/*
*** Constants.
*/

#define WD_STD_PARAMS char *file, long line
#define WD_STD_PARAMS_PASS file, line
#define WD_STD_ARGS __FILE__, __LINE__

#define WD_MSG_OUT_OF_MEMORY "Out of memory; wanted %zu(+%zu) b."
#define WD_MSG_OUT_OF_MEMORY_INTERNAL "(Out of memory; wanted %zu+%zu b.)"
#define WD_MSG_INCOMING_UNTRACKED "Incoming memory%s is untracked."
#define WD_MSG_INCOMING_ENCLOSED "Incoming address%s is an offset into an allocation."
#define WD_MSG_INCOMING_NULL "Incoming NULL pointer."
#define WD_MSG_INCOMING_DANGLING "Incoming dangling pointer (freed at " LOGGING_WHERE ")."
#define WD_MSG_NOT_FREED "Memory not freed."
#define WD_MSG_ALL_FREED "All memory freed."
#define WD_MSG_ATEXIT "(ATEXIT failed. Call wd_restrain manually.)"
#define WD_MSG_SIGNAL "(SIGNAL failed. Not catching signals.)"
#define WD_MSG_SIZE_LEQ_0 "Size less than/equal to 0."
#define WD_MSG_SIZE_NOT_BIGGER "Size smaller or same."
#define WD_MSG_PADDING_LEFT "Left padding of [" LOGGING_WHERE "] not intact."
#define WD_MSG_PADDING_RIGHT "Right padding of [" LOGGING_WHERE "] not intact."
#define WD_MSG_SIGFPE "Arithmetic error."
#define WD_MSG_SIGILL "Illegal instruction."
#define WD_MSG_SIGSEGV "Segmentation fault."
#define WD_MSG_OUT_OF_BOUNDS "Index out of range. (%d of #%zu)"
#define WD_MSG_TRACK "Tracks."
#define WD_MSG_SNAPSHOT "Snapshot of [" LOGGING_WHERE "] changed without notice."
#define WD_MSG_ASSERT "Assertion failed: %s"
#define WD_MSG_REPORTER_LOG "(Unable to create log file. Not reporting.)"

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
*** Preprocessor functions.
*/

#define WD_ENSURE_UNLEASHED() \
  if (!wd_unleashed) \
    wd_unleash(WD_STD_PARAMS_PASS);

#define WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(allocator_return_value, alloc_size, size_markup)\
  if (allocator_return_value == NULL) {\
    wd_alerts++;\
    fail_at(__FILE__, __LINE__, WD_MSG_OUT_OF_MEMORY_INTERNAL, alloc_size, size_markup);\
  }

/*
*** Tools.
*/

void wd_int_out(int num);
void wd_fill_with_random_bytes(char *memory, size_t count);

/*
*** Main interface.
*/

// FIXME:?
void wd_restrain(void);
void wd_unleash(WD_STD_PARAMS);

#endif
