/*
usage.h - watchdog
Modified 2021-12-12
*/

#ifndef WD_USAGE_H
#define WD_USAGE_H

/* Header-specific includes. */
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
void wd_usage_offset(ptrdiff_t amount);

void wd_original_frame_create(wd_alloc *alloc);
void wd_original_frame_resize(wd_alloc *alloc);
void wd_original_frame_destroy(wd_alloc *alloc);
void wd_original_capture(wd_alloc *alloc);
void wd_original_adjust(wd_alloc *alloc, ptrdiff_t growth);
void wd_original_compare(wd_alloc *alloc);

#endif /* !WD_USAGE_H */
