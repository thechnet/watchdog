/*
public.c - watchdog
Modified 2021-12-04
*/

/* Header-specific includes. */
#include "public.h"

/* Implementation-specific includes. */
#include "dogshed.h"
#include "radar.h"
#include "reporter.h"
#include "tracks.h"
#include "padding.h"
#include "snapshots.h"
#include "dangling.h"
#include "usage.h"
#include "signals.h"

/*
*** Globals.
*/

bool wd_unleashed = false;

/*
*** Watchdog interface.
*/

/*
Restrain the Watchdog.
*/
void wd_restrain(void)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  
  wd_reporter_summary();
  wd_radar_disable();
  wd_dangling_close();
  
  wd_unleashed = false;
}

/*
Unleash the Watchdog.
*/
void wd_unleash(WD_STD_PARAMS)
{
  /* Assert that this function runs in the right circumstances. */
  assert(!wd_unleashed);
  
  wd_radar_enable();
  wd_padding_generate();
  wd_tracks_reset();
  wd_dangling_open();
  wd_usage_reset();
  wd_signals_register();
  
  if (atexit(wd_restrain) != 0) {
    wd_alerts++;
    warn(WD_MSG_ATEXIT);
  }
  wd_unleashed = true;
}

/*
Check state.
*/
int wd_bark(WD_STD_PARAMS)
{
  /* Assert that this function runs in the right circumstances. */
  WD_ENSURE_UNLEASHED();
  
  for (size_t i=0; i<wd_radar_size; i++)
    if (wd_radar[i].address != NULL) {
      if (wd_radar[i].padding_check_left || wd_radar[i].padding_check_right)
        wd_padding_check(WD_STD_PARAMS_PASS, wd_radar+i);
      if (wd_radar[i].snapshot != NULL)
        wd_snapshot_compare(WD_STD_PARAMS_PASS, wd_radar+i);
    }
  
  wd_tracks_update(WD_STD_PARAMS_PASS);
  return 1;
}
