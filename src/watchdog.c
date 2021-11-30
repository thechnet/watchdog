/*
watchdog.c - watchdog
Modified 2021-11-30
*/

#include "private.h"
#include "radar.h"

bool wd_unleashed = false;

/*
Restrain the Watchdog.
*/
void wd_restrain(void)
{
  wd_radar_stop(WD_STD_ARGS);
  wd_unleashed = false;
  out("restrain");
}

/*
Unleash the Watchdog.
*/
void wd_unleash(WD_STD_PARAMS)
{
  out("unleash");
  wd_radar_start(WD_STD_ARGS);
  atexit(&wd_restrain);
  wd_unleashed = true;
}
