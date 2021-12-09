/*
usage.c - watchdog
Modified 2021-12-09
*/

/* Header-specific includes. */
#include "usage.h"

/* Implementation-specific includes. */
#include <stdint.h>
#include <memory.h>
#include "reporter.h"

/*
*** Usage globals.
*/

ptrdiff_t wd_usage_current_allocated;
ptrdiff_t wd_usage_max_allocated;
ptrdiff_t wd_usage_total_allocated;
ptrdiff_t wd_usage_total_written;

/*
*** Usage interface.
*/

/*
Reset usage metrics.
*/
void wd_usage_reset(void)
{
  wd_usage_current_allocated = 0;
  wd_usage_max_allocated = 0;
  wd_usage_total_written = 0;
  wd_usage_total_allocated = 0;
}

/*
Add amount to current memory usage.
*/
void wd_usage_add(ptrdiff_t amount)
{
  assert(wd_usage_current_allocated+amount >= 0);
  wd_usage_current_allocated += amount;
  if (wd_usage_current_allocated > wd_usage_max_allocated)
    wd_usage_max_allocated = wd_usage_current_allocated;
}

/*
Capture the original memory.
*/
void wd_usage_original_capture(wd_alloc *alloc)
{
  assert(alloc != NULL);
  assert(alloc->original == NULL);
  alloc->original = malloc(alloc->size_user);
  WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(alloc->original, 0, alloc->size_user);
  memcpy(alloc->original, alloc->addr_user, alloc->size_user);
}

/*
Update the original memory after reallocation.
*/
void wd_usage_original_update(wd_alloc *alloc, ptrdiff_t growth)
{
  assert(alloc != NULL);
  assert(alloc->is_native);
  assert(alloc->original != NULL);
  alloc->original = realloc(alloc->original, alloc->size_user);
  WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(alloc->original, 0, alloc->size_user);
  if (growth <= 0)
    return;
  ptrdiff_t size_before = alloc->size_user-growth;
  assert(size_before >= 0);
  memcpy(alloc->original+size_before, alloc->addr_user+size_before, growth);
}

/*
Compare the final state of the memory to the original.
*/
void wd_usage_original_compare(wd_alloc *alloc)
{
  assert(alloc != NULL);
  size_t bytes_unchanged = 0;
  size_t bytes_changed = 0;
  assert(alloc->is_native);
  assert(alloc->original != NULL);
  for (size_t i=0; i<alloc->size_user; i++)
    if (alloc->addr_user[i] == alloc->original[i])
      bytes_unchanged++;
    else
      bytes_changed++;
  assert(bytes_unchanged+bytes_changed==alloc->size_user);
  wd_usage_total_allocated += alloc->size_user;
  wd_usage_total_written += bytes_changed;
}
