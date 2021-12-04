/*
snapshots.h - watchdog
Modified 2021-12-04
*/

#ifndef WD_SNAPSHOTS_H
#define WD_SNAPSHOTS_H

/* Header-specific includes. */
#include "dogshed.h"
#include "radar.h"

/*
*** Snapshots interface.
*/

void wd_snapshot_alloc(wd_alloc *alloc);
void wd_snapshot_realloc(wd_alloc *alloc);
void wd_snapshot_capture(wd_alloc *alloc);
void wd_snapshot_compare(WD_STD_PARAMS, wd_alloc *alloc);

#endif /* !WD_SNAPSHOTS_H */
