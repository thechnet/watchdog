/*
bounds.h - watchdog
Modified 2021-12-03
*/

#ifndef WD_BOUNDS_H
#define WD_BOUNDS_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Bounds interface.
*/

int wd_bounds_check(WD_STD_PARAMS, void *array, size_t array_size, size_t item_size, int index);

#endif /* !WD_BOUNDS_H */
