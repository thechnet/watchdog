/*
reporter.h - watchdog
Modified 2021-12-08
*/

#ifndef WD_REPORTER_H
#define WD_REPORTER_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Reporter globals.
*/

extern size_t wd_alerts;

/*
*** Reporter interface.
*/

void wd_reporter_summary(void);

#endif /* !WD_REPORTER_H */
