/*
overrides.c - watchdog
Modified 2021-11-30
*/

#include "overrides.h"
#include "reporter.h"

/*
Override malloc.
*/
void *wd_override_malloc(WD_STD_PARAMS, size_t size)
{
  WD_ENSURE_UNLEASHED();
  void *memory = malloc(size);
  if (memory == NULL) {
    wd_alerts++;
    fail_at(WD_STD_PARAMS_PASS, WD_MSG_OUT_OF_MEMORY " (malloc %zu b)", size);
  }
  wd_radar_track(WD_STD_ARGS, memory, size);
  return memory;
}

/*
Override free.
*/
void wd_override_free(WD_STD_PARAMS, void *memory)
{
  WD_ENSURE_UNLEASHED();
  free(memory);
  if (!wd_radar_drop(WD_STD_ARGS, memory))
    warn_at(WD_STD_PARAMS_PASS, WD_MSG_FREE_UNKNOWN_MEMORY " (%p)", memory);
}
