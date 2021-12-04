/*
signals.h - watchdog
Modified 2021-12-04
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
void wd_int_out(int num);

#endif /* !WD_SIGNALS_H */
