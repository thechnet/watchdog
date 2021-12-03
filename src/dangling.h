/*
dangling.h - watchdog
Modified 2021-12-03
*/

#ifndef WD_DANGLING_H
#define WD_DANGLING_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Dangling constants.
*/

#define WD_DANGLING_SIZE 32
#define WD_DANGLING_EMPTY_SPOT NULL

/*
*** Dangling globals.
*/

extern void **wd_dangling;
extern size_t wd_dangling_size;

/*
*** Dangling interface.
*/

void wd_dangling_open(void);
void wd_dangling_close(void);
void **wd_dangling_find(WD_STD_PARAMS, void *address);
void wd_dangling_record(WD_STD_PARAMS, void *address);
void wd_dangling_erase(void **pointer);

#endif /* !WD_DANGLING_H */
