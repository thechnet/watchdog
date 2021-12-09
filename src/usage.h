/*
usage.h - watchdog
Modified 2021-12-09
*/

#ifndef WD_USAGE_H
#define WD_USAGE_H

/* Header-specific includes. */
#include <stdint.h>
#include "dogshed.h"
#include "radar.h"

/*
*** Usage globals.
*/

extern ptrdiff_t wd_usage_current_allocated;
extern ptrdiff_t wd_usage_max_allocated;
extern ptrdiff_t wd_usage_total_written;
extern ptrdiff_t wd_usage_total_allocated;

/*
*** Usage interface.
*/

void wd_usage_reset(void);
void wd_usage_add(ptrdiff_t amount);
void wd_usage_original_capture(wd_alloc *alloc);
void wd_usage_original_update(wd_alloc *alloc, ptrdiff_t growth);
void wd_usage_original_compare(wd_alloc *alloc);

#endif /* !WD_USAGE_H */
