/*
pulse.h - watchdog
Modified 2021-12-10
*/

#ifndef WD_PULSE_H
#define WD_PULSE_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Pulse globals.
*/

extern wd_point wd_pulse;

/*
*** Pulse interface.
*/

void wd_pulse_reset(void);
void wd_pulse_update(WD_STD_PARAMS);

#endif /* !WD_PULSE_H */
