/*
snapshots.c - watchdog
Modified 2021-12-09
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
  assert(alloc->snapshot == NULL);
  alloc->snapshot = malloc(alloc->size_user);
  WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(alloc->snapshot, 0, alloc->size_user);
}

/*
Reallocate snapshot memory.
*/
void wd_snapshot_realloc(wd_alloc *alloc)
{
  assert(alloc->snapshot != NULL);
  alloc->snapshot = realloc(alloc->snapshot, alloc->size_user);
  WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(alloc->snapshot, 0, alloc->size_user);
}

/*
Take snapshot.
*/
void wd_snapshot_capture(wd_alloc *alloc)
{
  assert(alloc->snapshot != NULL);
  memcpy(alloc->snapshot, alloc->addr_user, alloc->size_user);
}

/*
Check snapshot.
*/
void wd_snapshot_compare(WD_STD_PARAMS, wd_alloc *alloc)
{
  assert(alloc->is_protected);
  assert(alloc->snapshot != NULL);
  
  if (memcmp(alloc->addr_user, alloc->snapshot, alloc->size_user) != 0) {
    info_at(file, line, WD_MSG_SNAPSHOT, alloc->point.file, alloc->point.line);
    wd_snapshot_capture(alloc);
  }
}
