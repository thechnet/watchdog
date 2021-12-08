/*
reporter.c - watchdog
Modified 2021-12-08
*/

/* Header-specific includes. */
#include "reporter.h"

/* Implementation-specific includes. */
#include "radar.h"
#include "tracks.h"
#include "usage.h"

/*
*** Reporter globals.
*/

size_t wd_alerts = 0;

/*
*** Reporter interface.
*/

/*
Print a summary to the console.
*/
void wd_reporter_summary(void)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  
  /* Present unfreed allocations on radar. */
  assert(wd_radar != NULL);
  bool radar_empty = true;
  for (size_t i=0; i<wd_radar_size; i++)
    if (wd_radar[i].address != NULL) {
      radar_empty = false;
      wd_alerts++;
      warn_at(wd_radar[i].point.file, wd_radar[i].point.line, WD_MSG_NOT_FREED);
    }
  if (radar_empty)
    success(WD_MSG_ALL_FREED);
  
  // printf(
  //   "\nusage_current_allocated: %zu b\n"
  //   "usage_current_allocated_internal: %zu b\n"
  //   "usage_max_allocated: %zu b\n"
  //   "usage_max_internal_allocated: %zu b\n"
  //   "usage_total_written: %zu b\n"
  //   "usage_total_allocated: %zu b\n",
  //   wd_usage_current_allocated,
  //   wd_usage_current_allocated_internal,
  //   wd_usage_max_allocated,
  //   wd_usage_max_allocated_internal,
  //   wd_usage_total_written,
  //   wd_usage_total_allocated
  // );
}
