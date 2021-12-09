/*
signals.h - watchdog
Modified 2021-12-09
*/

#ifndef WD_SIGNALS_H
#define WD_SIGNALS_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Signals interface.
*/

void wd_signals_register(void);
void wd_signals_handle(int signum);

#endif /* !WD_SIGNALS_H */
