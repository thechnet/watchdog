/*
snapshots.c - watchdog
Modified 2021-12-02
*/

/* Header-specific includes. */
#include "snapshots.h"

/* Implementation-specific includes. */
#include <memory.h>

/*
*** Snapshots interface.
*/

/*
Take snapshot.
*/
void wd_snapshot_take(wd_alloc *alloc)
{
  memcpy(alloc->snapshot, alloc->memory, alloc->size);
}

/*
Check snapshot.
*/
void wd_snapshot_check(WD_STD_PARAMS, wd_alloc *alloc)
{
  assert(alloc->snapshot != NULL);
  
  if (memcmp(alloc->memory, alloc->snapshot, alloc->size) != 0) {
    warn_at(alloc->origin.file, alloc->origin.line, WD_MSG_SNAPSHOT, WD_STD_PARAMS_PASS);
    wd_snapshot_take(alloc);
  }
}
