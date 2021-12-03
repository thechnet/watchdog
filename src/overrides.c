/*
overrides.c - watchdog
Modified 2021-12-03
*/

/* Header-specific includes. */
#include "overrides.h"

/* Implementation-specific includes. */
#include <memory.h>
#include "radar.h"
#include "reporter.h"
#include "public.h"
#include "padding.h"
#include "dangling.h"

/*
*** Overrides.
*/

/*
Override malloc.
*/
void *wd_override_malloc(WD_STD_PARAMS, size_t size)
{
  /* Assert that this function runs in the right circumstances. */
  WD_ENSURE_UNLEASHED();
  assert(wd_padding_generated);
  
  /* Check state. */
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  if (size == 0)
    warn_at(file, line, WD_MSG_SIZE_0); // FIXME: Find solution to WD_STD_PARAMS_PASS.
  
  /* Allocate memory. */
  void *memory = malloc(size+WD_PADDING_SIZE);
  
  /* Remove address from dangling pointer record if previously recorded. */
  void **pointer = wd_dangling_find(WD_STD_PARAMS_PASS, memory);
  if (pointer != NULL)
    wd_dangling_erase(pointer);
  
  /* Add padding. */
  memcpy(memory+size, wd_padding, WD_PADDING_SIZE);
  
  /* Verify outgoing values. */
  if (memory == NULL) {
    wd_alerts++;
    fail_at(WD_STD_PARAMS_PASS, WD_MSG_OUT_OF_MEMORY " (malloc %zu b)", size);
  }
  
  /* Add to radar. */
  wd_alloc *alloc = wd_radar_watch(WD_STD_PARAMS_PASS, memory, size, true);
  
  /* Allocate snapshot memory. */
  alloc->snapshot = malloc(size);
  if (alloc->snapshot == NULL) {
    wd_alerts++;
    fail_at(WD_STD_ARGS, WD_MSG_OUT_OF_MEMORY " (malloc %zu b)", size);
  }
  
  return memory;
}

/*
Override realloc.
*/
void *wd_override_realloc(WD_STD_PARAMS, void *memory, size_t new_size)
{
  /* Assert that this function runs in the right circumstances. */
  WD_ENSURE_UNLEASHED();
  assert(wd_padding_generated);
  
  /* Check state. */
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  if (memory == NULL) {
    wd_alerts++;
    fail_at(file, line, WD_MSG_INCOMING_NULL);
  }
  if (new_size == 0) {
    wd_alerts++;
    warn_at(file, line, WD_MSG_SIZE_0); // FIXME: Find solution to WD_STD_PARAMS_PASS.
  }
  wd_alloc *alloc = wd_radar_find(WD_STD_PARAMS_PASS, memory);
  if (alloc == NULL) {
    wd_alerts++;
    fail_at(file, line, WD_MSG_UNTRACKED_MEMORY);
  }
  if (new_size <= alloc->size) {
    wd_alerts++;
    warn_at(file, line, WD_MSG_REALLOC_SIZE);
  }
  
  /* Temporarily remove padding. */
  memset(alloc->memory+alloc->size, WD_PADDING_CLEAR_CHAR, WD_PADDING_SIZE);
  
  /* Reallocate memory. */
  void *memory_new = realloc(alloc->memory, new_size+WD_PADDING_SIZE);
  
  /* Verify outgoing values. */
  if (memory_new == NULL) {
    wd_alerts++;
    fail_at(file, line, WD_MSG_OUT_OF_MEMORY);
  }
  
  /* Remove address from dangling pointer record if previously recorded. */
  void **pointer = wd_dangling_find(WD_STD_PARAMS_PASS, memory_new);
  if (pointer != NULL)
    wd_dangling_erase(pointer);
  
  /* Confirm changes in radar. */
  alloc->size = new_size;
  alloc->memory = memory_new;
  
  /* Re-add padding. */
  memcpy(alloc->memory+alloc->size, wd_padding, WD_PADDING_SIZE);
  
  /* Reallocate snapshot memory. */
  alloc->snapshot = realloc(alloc->snapshot, alloc->size);
  if (alloc->snapshot == NULL) {
    wd_alerts++;
    fail_at(WD_STD_ARGS, WD_MSG_OUT_OF_MEMORY " (realloc %zu b)", new_size);
  }
  
  return alloc->memory;
}

/*
Override free.
*/
void wd_override_free(WD_STD_PARAMS, void *memory)
{
  /* Assert that this function runs in the right circumstances. */
  WD_ENSURE_UNLEASHED();
  
  /* Check state. */
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Free memory. */
  free(memory);
  
  /* Record address in dangling pointer record. */
  wd_dangling_record(WD_STD_PARAMS_PASS, memory);
  
  /* Warn if freeing untracked memory. */
  if (!wd_radar_drop(WD_STD_ARGS, memory))
    warn_at(WD_STD_PARAMS_PASS, WD_MSG_UNTRACKED_MEMORY " (%p)", memory);
}
