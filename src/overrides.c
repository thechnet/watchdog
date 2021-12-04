/*
overrides.c - watchdog
Modified 2021-12-05
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
#include "usage.h"

/*
*** Overrides.
*/

/*
Override malloc.
*/
char *wd_override_malloc(WD_STD_PARAMS, size_t size)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_WARN_IF_SIZE_0(size);
  
  /* Allocate memory. */
  char *memory_real = malloc(WD_PADDING_SIZE+size+WD_PADDING_SIZE);
  
  /* Verify outgoing values. */
  WD_FAIL_IF_OUT_OF_MEMORY(memory_real, size, 2*WD_PADDING_SIZE);
  
  /* Add to radar. */
  wd_alloc *alloc = wd_radar_add(WD_STD_PARAMS_PASS,
    WD_TO_VIRTUAL(memory_real),
    size,
    true,
    true,
    true
  );
  
  return alloc->address;
}

/*
Override realloc.
*/
char *wd_override_realloc(WD_STD_PARAMS, char *memory_virtual, size_t new_size)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_WARN_IF_PTR_NULL(memory_virtual);
  WD_WARN_IF_PTR_DANGLING(memory_virtual);
  WD_WARN_IF_SIZE_0(new_size);
  
  wd_alloc *alloc = wd_radar_search(memory_virtual);
  
  WD_WARN_IF_RADAR_FINDS_PTR_UNTRACKED(alloc);
  
  /* If the incoming pointer is untracked, add it to the radar first. */
  // FIXME:
  // if (alloc == NULL) {
  //   wd_alloc *alloc = wd_radar_add(WD_STD_PARAMS_PASS,
  //   WD_TO_VIRTUAL(memory_real),
  //   size,
  //   true,
  //   true,
  //   true
  // );
  // }
  /*
  (We are not going to add padding after the fact because there is no way for us to know how many bytes we'd be allowed to copy into a new padded buffer.)
  */
  
  WD_WARN_IF_SIZE_REDUCED(alloc->size, new_size);
  
  /* Temporarily remove padding. */
  wd_padding_clear(alloc);
  
  /* Reallocate memory. */
  char *memory_new_real = realloc(WD_TO_REAL(memory_virtual), new_size+2*WD_PADDING_SIZE);
  
  /* Verify outgoing values. */
  WD_FAIL_IF_OUT_OF_MEMORY(memory_new_real, new_size, 2*WD_PADDING_SIZE);
  
  char *memory_new_virtual = WD_TO_VIRTUAL(memory_new_real);
  
  /* Confirm changes in radar. */
  size_t old_size = alloc->size;
  alloc->size = new_size;
  alloc->address = memory_new_virtual;
  
  /* Remove address from dangling pointer record if previously recorded. */
  wd_dangling_find_and_erase(alloc->address);
  
  /* Re-add padding. */
  wd_padding_write(alloc);
  
  /* Update snapshot. */
  wd_snapshot_realloc(alloc);
  wd_snapshot_capture(alloc);
  
  /* Update original. */
  wd_usage_original_update(alloc, old_size);
  
  return alloc->address;
}

/*
Override free.
*/
void wd_override_free(WD_STD_PARAMS, char *memory_virtual)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_WARN_IF_PTR_NULL(memory_virtual);
  WD_WARN_IF_PTR_DANGLING(memory_virtual);
  
  wd_alloc *alloc = wd_radar_search(memory_virtual);
  
  WD_WARN_IF_RADAR_FINDS_PTR_UNTRACKED(alloc);
  
  if (alloc != NULL) {
    /* Assess usage. */
    wd_usage_original_compare(alloc);
    
    /* Remove memory from radar. */
    wd_radar_drop(alloc);
  }
  
  /* Free memory. */
  free(WD_TO_REAL(memory_virtual));
  
  /* Record address in dangling pointer record. */
  wd_dangling_record(WD_STD_PARAMS_PASS, memory_virtual);
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
