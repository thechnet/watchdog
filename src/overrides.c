/*
overrides.c - watchdog
Modified 2021-12-01
*/

/* Header-specific includes. */
#include "overrides.h"

/* Implementation-specific includes. */
#include "radar.h"
#include "reporter.h"

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
  
  /* Verify incoming values. */
  if (size == 0)
    warn_at(file, line, WD_MSG_SIZE_0); // FIXME: Find solution to WD_STD_PARAMS_PASS.
  
  void *memory = malloc(size);
  
  /* Verify outgoing values. */
  if (memory == NULL) {
    wd_alerts++;
    fail_at(WD_STD_PARAMS_PASS, WD_MSG_OUT_OF_MEMORY " (malloc %zu b)", size);
  }
  
  wd_radar_watch(WD_STD_PARAMS_PASS, memory, size);
  return memory;
}

/*
Override free.
*/
void wd_override_free(WD_STD_PARAMS, void *memory)
{
  /* Assert that this function runs in the right circumstances. */
  WD_ENSURE_UNLEASHED();
  
  free(memory);
  
  /* Warn if freeing untracked memory. */
  if (!wd_radar_drop(WD_STD_ARGS, memory))
    warn_at(WD_STD_PARAMS_PASS, WD_MSG_FREE_UNTRACKED_MEMORY " (%p)", memory);
}
