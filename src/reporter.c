/*
reporter.c - watchdog
Modified 2021-12-04
*/

/* Header-specific includes. */
#include "reporter.h"

/* Implementation-specific includes. */
#include "radar.h"
#include "tracks.h"

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
    if (wd_radar[i].memory != NULL) {
      radar_empty = false;
      wd_alerts++;
      warn_at(wd_radar[i].origin.file, wd_radar[i].origin.line, WD_MSG_NOT_FREED);
    }
  if (radar_empty)
    success(WD_MSG_ALL_FREED);
}
