/*
reporter.c - watchdog
Modified 2021-12-12
*/

/* Header-specific includes. */
#include "reporter.h"

/* Implementation-specific includes. */
#include "radar.h"
#include "pulse.h"
#include "usage.h"

/*
*** Reporter globals.
*/

size_t wd_alerts = 0;
FILE *wd_reporter_log = NULL;
size_t wd_reporter_log_count = 0;

/*
*** Reporter interface.
*/

/*
Initialize the reporter.
*/
void wd_reporter_initialize(void)
{
  #ifndef WD_REPORTER_ENABLE
  return;
  #endif
  assert(wd_reporter_log == NULL);
  wd_reporter_log_count = 0;
  wd_reporter_log = fopen(WD_REPORTER_FILE_LOG, "w");
  if (wd_reporter_log == NULL)
    warn(WD_MSG_REPORTER_LOG);
  wd_report("(Begin report.)");
}

/*
Deinitialize the reporter.
*/
void wd_reporter_terminate(void)
{
  if (wd_reporter_log == NULL)
    return;
  wd_report("(End report.)");
  fclose(wd_reporter_log);
  wd_reporter_log = NULL;
}

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
    if (wd_radar[i].addr_user != NULL && !wd_radar[i].dependent) {
      radar_empty = false;
      wd_alerts++;
      warn_at(wd_radar[i].point.file, wd_radar[i].point.line, WD_MSG_NOT_FREED);
    }
  if (radar_empty)
    success(WD_MSG_ALL_FREED);
  
  printf(
    "\nusage_current_allocated: %zu b\n"
    "usage_max_allocated: %zu b\n"
    "usage_total_written: %zu b\n"
    "usage_total_allocated: %zu b\n",
    wd_usage_current_allocated,
    wd_usage_max_allocated,
    wd_usage_total_written,
    wd_usage_total_allocated
  );
}
