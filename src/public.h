/*
dogshed.h - watchdog
Modified 2021-12-02
*/

#ifndef WD_PUBLIC_H
#define WD_PUBLIC_H

/* Header-specific includes. */
#include <stdlib.h>

/*
*** Public interface.
*/

void wd_restrain(void);
int wd_bark(char *file, size_t line); // FIXME: Cannot use WD_STD_PARAMS because we do not want to expose that macro to the public.

#endif
