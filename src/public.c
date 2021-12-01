/*
public.c - watchdog
Modified 2021-12-01
*/

/* Header-specific includes. */
#include "public.h"

/* Implementation-specific includes. */
#include "dogshed.h"
#include "radar.h"
#include "reporter.h"

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
  
  wd_unleashed = false;
  out("restrain");
}

/*
Unleash the Watchdog.
*/
void wd_unleash(WD_STD_PARAMS)
{
  /* Assert that this function runs in the right circumstances. */
  assert(!wd_unleashed);
  
  out("unleash");
  
  wd_radar_enable();
  
  if (atexit(wd_restrain) != 0) {
    wd_alerts++;
    warn(WD_MSG_ATEXIT);
  }
  wd_unleashed = true;
}
