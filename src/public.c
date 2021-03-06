/*
public.c - watchdog
Modified 2021-12-12
*/

/* Header-specific includes. */
#include "public.h"

/* Implementation-specific includes. */
#include "dogshed.h"
#include "radar.h"
#include "reporter.h"
#include "pulse.h"
#include "padding.h"
#include "snapshots.h"
#include "archive.h"
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
  wd_radar_terminate();
  wd_archive_terminate();
  wd_reporter_terminate();
  
  wd_unleashed = false;
}

/*
Unleash the Watchdog.
*/
void wd_unleash(WD_STD_PARAMS)
{
  /* Assert that this function runs in the right circumstances. */
  assert(!wd_unleashed);
  
  wd_reporter_initialize();
  wd_radar_initialize();
  wd_padding_generate();
  wd_pulse_reset();
  wd_archive_initialize();
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
  
  /* Check padding and snapshots. */
  for (size_t i=0; i<wd_radar_size; i++)
    if (wd_radar[i].addr_user != NULL) {
      if (wd_radar[i].padding_check_left || wd_radar[i].padding_check_right)
        wd_padding_check(WD_STD_PARAMS_PASS, wd_radar+i);
      if (wd_radar[i].snapshot != NULL)
        wd_snapshot_compare(WD_STD_PARAMS_PASS, wd_radar+i);
    }
  
  /* Pulse. */
  wd_pulse_update(WD_STD_PARAMS_PASS);
  
  return 1;
}

/*
Stop flagging an address as untracked.
FIXME: Not sure if using .dependent for this is the right solution.
Also, the entire implementation of this might need work.
*/
void wd_ignore(WD_STD_PARAMS, char *addr_user, ptrdiff_t size_user)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  wd_radar_add(NULL, 0, addr_user, size_user, false, false, true, false);
}
