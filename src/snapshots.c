/*
snapshots.c - watchdog
Modified 2021-12-04
*/

/* Header-specific includes. */
#include "snapshots.h"

/* Implementation-specific includes. */
#include <memory.h>
#include "reporter.h"

/*
*** Snapshots interface.
*/

/*
Allocate snapshot memory.
*/
void wd_snapshot_alloc(wd_alloc *alloc)
{
  alloc->snapshot = malloc(alloc->size);
  WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(alloc->snapshot, alloc->size, 0);
}

/*
Reallocate snapshot memory.
*/
void wd_snapshot_realloc(wd_alloc *alloc)
{
  alloc->snapshot = realloc(alloc->snapshot, alloc->size);
  WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(alloc->snapshot, alloc->size, 0);
}

/*
Take snapshot.
*/
void wd_snapshot_capture(wd_alloc *alloc)
{
  memcpy(alloc->snapshot, alloc->address, alloc->size);
}

/*
Check snapshot.
*/
void wd_snapshot_compare(WD_STD_PARAMS, wd_alloc *alloc)
{
  assert(alloc->snapshot != NULL);
  
  if (memcmp(alloc->address, alloc->snapshot, alloc->size) != 0) {
    warn_at(alloc->point.file, alloc->point.line, WD_MSG_SNAPSHOT, WD_STD_PARAMS_PASS);
    wd_snapshot_capture(alloc);
  }
}
