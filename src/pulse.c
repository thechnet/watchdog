/*
pulse.c - watchdog
Modified 2021-12-10
*/

/* Header-specific includes. */
#include "pulse.h"

/*
Pulse globals.
*/

wd_point wd_pulse;

/*
*** Pulse interface.
*/

/*
Reset pulse.
*/
void wd_pulse_reset(void)
{
  wd_pulse = (wd_point){
    .file = NULL,
    .line = 0
  };
}

/*
Update pulse.
*/
void wd_pulse_update(WD_STD_PARAMS)
{
  wd_pulse = (wd_point){
    .file = file,
    .line = line
  };
}
