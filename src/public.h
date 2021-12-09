/*
dogshed.h - watchdog
Modified 2021-12-09
*/

#ifndef WD_PUBLIC_H
#define WD_PUBLIC_H

/* Header-specific includes. */
#include <stdlib.h>

/*
*** Public interface.
*/

void wd_restrain(void);
int wd_bark(char *file, long line);
void wd_ignore(char *file, long line, char *addr_user, ptrdiff_t size_user);

#endif
