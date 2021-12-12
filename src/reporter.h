/*
reporter.h - watchdog
Modified 2021-12-12
*/

#ifndef WD_REPORTER_H
#define WD_REPORTER_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Report constants.
*/

#define WD_REPORTER_ENABLE
#define WD_REPORTER_FILE_LOG "watchdog.log"
#define WD_REPORTER_SIZE_LOG ((size_t)4096)

/*
*** Reporter globals.
*/

extern size_t wd_alerts;
extern FILE *wd_reporter_log;
extern size_t wd_reporter_log_count;

/*
*** Report preprocessor functions.
*/

#define wd_report(__msg, ...)\
  {\
    if (wd_reporter_log != NULL) {\
      if (wd_reporter_log_count++ == WD_REPORTER_SIZE_LOG) {\
        fprintf(wd_reporter_log, "(Maximum log count reached.)\n");\
        wd_reporter_terminate();\
      } else {\
        fprintf(wd_reporter_log, __msg "\n", ##__VA_ARGS__);\
      }\
    }\
  }

/*
*** Reporter interface.
*/

void wd_reporter_initialize(void);
void wd_reporter_terminate(void);
void wd_reporter_summary(void);

#endif /* !WD_REPORTER_H */
