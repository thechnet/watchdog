/*
reporter.h - watchdog
Modified 2021-11-30
*/

/* Header-specific includes. */
#include "private.h"

/*
*** Reporter globals.
*/

extern size_t wd_alerts; // FIXME: Still unsure about the use of this.

/*
*** Reporter interface.
*/

void wd_reporter_summary(void);
