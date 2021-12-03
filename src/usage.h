/*
usage.h - watchdog
Modified 2021-12-02
*/

#ifndef WD_USAGE_H
#define WD_USAGE_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Usage globals.
*/

extern size_t wd_usage_current = 0;
extern size_t wd_usage_current_internal = 0;
extern size_t wd_usage_max = 0;
extern size_t wd_usage_max_internal = 0;

/*
*** Usage interface.
*/

void wd_usage_reset(void);
void wd_usage_add(size_t amount);
void wd_usage_subtract(size_t amount);

#endif /* !WD_USAGE_H */
