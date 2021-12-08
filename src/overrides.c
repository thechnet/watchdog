/*
overrides.c - watchdog
Modified 2021-12-08
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
char *wd_override_malloc(WD_STD_PARAMS, size_t size_virtual)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_WARN_IF_SIZE_0(size_virtual);
  
  /* Allocate memory. */
  size_t size_real = WD_REAL_SIZE(size_virtual);
  char *memory_real = malloc(size_real);
  
  /* Verify outgoing values. */
  WD_FAIL_IF_OUT_OF_MEMORY(memory_real, size_virtual, size_real-size_virtual);
  
  /* Add new allocation to radar. */
  wd_alloc *alloc = wd_radar_add(WD_STD_PARAMS_PASS,
    memory_real, size_virtual,
    true, true, true
  );
  
  return alloc->address;
}

/*
Override realloc.
*/
char *wd_override_realloc(WD_STD_PARAMS, char *memory_virtual, size_t size_virtual_new)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_WARN_IF_PTR_NULL(memory_virtual);
  WD_WARN_IF_PTR_DANGLING(memory_virtual);
  WD_WARN_IF_SIZE_0(size_virtual_new);
  wd_alloc *alloc = wd_radar_search(memory_virtual);
  WD_WARN_IF_RADAR_FINDS_PTR_UNTRACKED(alloc);
  /* If the incoming pointer is untracked, we add it to the radar. */
  if (alloc == NULL)
    alloc = wd_radar_add(WD_STD_PARAMS_PASS,
      memory_virtual, 0,
      false, false, false
    );
  WD_WARN_IF_SIZE_REDUCED(alloc->size, size_virtual_new);
  
  /* Unlock allocation. */
  wd_radar_unlock(alloc);
  
  /* Reallocate memory. */
  char *memory_real = wd_radar_real_address_get(alloc);
  size_t size_real = wd_radar_real_size_get(alloc);
  size_t size_new_real = size_real+(size_virtual_new-alloc->size);
  char *memory_new_real = realloc(memory_real, size_new_real);
  
  /* If the memory was previously not padded (because it was untracked), we can now add the padding. */
  if (!alloc->is_padded) {
    char *memory_new_real_padded = malloc(WD_PADDING_SIZE+size_new_real+WD_PADDING_SIZE);
    WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(memory_new_real_padded, size_new_real, 2*WD_PADDING_SIZE);
    memcpy(memory_new_real_padded+WD_PADDING_SIZE, memory_new_real, size_new_real);
    free(memory_new_real);
    memory_new_real = memory_new_real_padded;
    alloc->is_padded = true;
    alloc->padding_check_left = true;
    alloc->padding_check_right = true;
  }
  
  /* If there were previously no snapshots of this memory (because it was untracked), we can now take one. */
  if (alloc->snapshot == NULL) {
    wd_snapshot_alloc(alloc);
    wd_snapshot_capture(alloc);
  }
  
  /* Record old pointer as dangling pointer if the real pointer has changed. */
  if (memory_new_real != memory_real)
    wd_dangling_record(WD_STD_PARAMS_PASS, memory_real);
  
  /* Verify outgoing values. */
  WD_FAIL_IF_OUT_OF_MEMORY(memory_new_real, size_virtual_new, size_new_real-size_virtual_new);
  
  /* Lock allocation. */
  wd_radar_lock(alloc, size_virtual_new, memory_new_real);
  
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
  
  /* Determine real address. */
  char *memory_real;
  if (alloc != NULL && alloc->is_padded)
    memory_real = WD_TO_REAL(memory_virtual);
  else
    memory_real = memory_virtual;
  
  /* Finalize allocation if tracked. */
  if (alloc != NULL) {
    if (alloc->original != NULL)
      wd_usage_original_compare(alloc); /* Assess usage. */
    wd_radar_drop(alloc); /* Remove memory from radar. */
  }
  
  /* Free memory. */
  free(memory_real);
  
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
