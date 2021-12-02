/*
snapshots.h - watchdog
Modified 2021-12-02
*/

#ifndef WD_SNAPSHOTS_H
#define WD_SNAPSHOTS_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Snapshots interface.
*/

void wd_snapshot_take(wd_alloc *alloc);
void wd_snapshot_check(WD_STD_PARAMS, wd_alloc *alloc);

#endif /* !WD_SNAPSHOTS_H */
