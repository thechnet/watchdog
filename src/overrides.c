/*
overrides.c - watchdog
Modified 2021-12-04
*/

/* Header-specific includes. */
#include "overrides.h"

/* Implementation-specific includes. */
#include <memory.h>
#include "dogshed.h"
#include "radar.h"
#include "reporter.h"
#include "public.h"
#include "padding.h"
#include "dangling.h"
#include "snapshots.h"

/*
*** Overrides.
*/

/*
Override malloc.
*/
void *wd_override_malloc(WD_STD_PARAMS, size_t size)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_WARN_IF_SIZE_0(size);
  
  /* Allocate memory. */
  void *memory = malloc(size+WD_PADDING_SIZE);
  
  /* Verify outgoing values. */
  WD_FAIL_IF_OUT_OF_MEMORY(memory, size, WD_PADDING_SIZE);
  
  /* Add to radar. */
  wd_alloc *alloc = wd_radar_add(WD_STD_PARAMS_PASS, memory, size, true);
  wd_padding_write(alloc);
  wd_dangling_find_and_erase(alloc->memory);
  wd_snapshot_alloc(alloc);
  wd_snapshot_take(alloc);
  
  return alloc->memory;
}

/*
Override realloc.
*/
void *wd_override_realloc(WD_STD_PARAMS, void *memory, size_t new_size)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_WARN_IF_PTR_NULL(memory);
  WD_WARN_IF_PTR_DANGLING(memory);
  WD_WARN_IF_SIZE_0(new_size);
  
  wd_alloc *alloc = wd_radar_search(memory);
  
  WD_WARN_IF_RADAR_FINDS_PTR_UNTRACKED(alloc);
  WD_WARN_IF_SIZE_REDUCED(alloc->size, new_size);
  
  /* Temporarily remove padding. */
  wd_padding_clear(alloc);
  
  /* Reallocate memory. */
  void *memory_new = realloc(alloc->memory, new_size+WD_PADDING_SIZE);
  
  /* Verify outgoing values. */
  WD_FAIL_IF_OUT_OF_MEMORY(memory_new, new_size, WD_PADDING_SIZE);
  
  /* Confirm changes in radar. */
  alloc->size = new_size;
  alloc->memory = memory_new;
  
  /* Remove address from dangling pointer record if previously recorded. */
  wd_dangling_find_and_erase(memory_new);
  
  /* Re-add padding. */
  wd_padding_write(alloc);
  
  /* Update snapshot. */
  wd_snapshot_realloc(alloc);
  wd_snapshot_take(alloc);
  
  return alloc->memory;
}

/*
Override free.
*/
void wd_override_free(WD_STD_PARAMS, void *memory)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_WARN_IF_PTR_NULL(memory);
  WD_WARN_IF_PTR_DANGLING(memory);
  
  /* Free memory. */
  free(memory);
  
  /* Record address in dangling pointer record. */
  wd_dangling_record(WD_STD_PARAMS_PASS, memory);
  
  wd_alloc *alloc = wd_radar_search(memory);
  
  /* Warn if freeing untracked memory, otherwise remove memory from radar. */
  WD_WARN_IF_RADAR_FINDS_PTR_UNTRACKED(alloc);
  if (alloc != NULL)
    wd_radar_drop(alloc);
}

/*
Override assert.
*/
void wd_override_assert(WD_STD_PARAMS, char *assertion_string, int assertion)
{
  if (assertion)
    return;
  fail_at(file, line, WD_MSG_ASSERT, assertion_string);
}
