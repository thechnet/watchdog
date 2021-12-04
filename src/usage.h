/*
usage.h - watchdog
Modified 2021-12-04
*/

#ifndef WD_USAGE_H
#define WD_USAGE_H

/* Header-specific includes. */
#include "dogshed.h"
#include "radar.h"

/*
*** Usage globals.
*/

extern size_t wd_usage_current_allocated;
extern size_t wd_usage_current_allocated_internal;
extern size_t wd_usage_max_allocated;
extern size_t wd_usage_max_allocated_internal;
extern size_t wd_usage_total_written;
extern size_t wd_usage_total_allocated;

/*
*** Usage interface.
*/

void wd_usage_reset(void);
void wd_usage_add(size_t amount);
void wd_usage_subtract(size_t amount);
void wd_usage_original_capture(wd_alloc *alloc);
void wd_usage_original_update(wd_alloc *alloc, size_t old_size);
void wd_usage_original_compare(wd_alloc *alloc);

#endif /* !WD_USAGE_H */
