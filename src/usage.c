/*
usage.c - watchdog
Modified 2021-12-04
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

size_t wd_usage_current_allocated;
size_t wd_usage_current_allocated_internal;
size_t wd_usage_max_allocated;
size_t wd_usage_max_allocated_internal;
size_t wd_usage_total_allocated;
size_t wd_usage_total_written;

/*
*** Usage interface.
*/

/*
Reset usage metrics.
*/
void wd_usage_reset(void)
{
  wd_usage_current_allocated = 0;
  wd_usage_current_allocated_internal = 0;
  wd_usage_max_allocated = 0;
  wd_usage_max_allocated_internal = 0;
  wd_usage_total_written = 0;
  wd_usage_total_allocated = 0;
}

/*
Add amount to current memory usage.
*/
void wd_usage_add(size_t amount)
{
  wd_usage_current_allocated += amount;
  if (wd_usage_current_allocated > wd_usage_max_allocated)
    wd_usage_max_allocated = wd_usage_current_allocated;
}

/*
Subtract amount from current memory usage.
*/
void wd_usage_subtract(size_t amount)
{
  assert((int64_t)wd_usage_current_allocated - amount >= 0); // FIXME:?
  wd_usage_current_allocated -= amount;
}

/*
Capture the original memory.
*/
void wd_usage_original_capture(wd_alloc *alloc)
{
  assert(alloc != NULL);
  assert(alloc->original == NULL);
  alloc->original = malloc(alloc->size);
  WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(alloc->original, alloc->size, 0);
  memcpy(alloc->original, alloc->address, alloc->size);
}

/*
Update the original memory after reallocation.
*/
void wd_usage_original_update(wd_alloc *alloc, size_t old_size)
{
  assert(alloc != NULL);
  assert(alloc->original != NULL);
  alloc->original = realloc(alloc->original, alloc->size);
  WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(alloc->original, alloc->size, 0);
  if (alloc->size > old_size)
    memcpy(alloc->original+old_size, alloc->address+old_size, alloc->size-old_size);
}

/*
Compare the final state of the memory to the original.
*/
void wd_usage_original_compare(wd_alloc *alloc)
{
  assert(alloc != NULL);
  size_t bytes_unchanged = 0;
  size_t bytes_changed = 0;
  for (size_t i=0; i<alloc->size; i++)
    if (alloc->address[0] == alloc->original[0])
      bytes_unchanged++;
    else
      bytes_changed++;
  assert(bytes_unchanged+bytes_changed==alloc->size);
  wd_usage_total_allocated += alloc->size;
  wd_usage_total_written += bytes_changed;
}
