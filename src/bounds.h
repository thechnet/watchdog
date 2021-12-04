/*
bounds.h - watchdog
Modified 2021-12-04
*/

#ifndef WD_BOUNDS_H
#define WD_BOUNDS_H

/* Header-specific includes. */
#include <stdlib.h>

/*
*** Bounds interface.
*/

int wd_bounds_check(char *file, size_t line, char *array, size_t array_size, size_t item_size, int index);

#endif /* !WD_BOUNDS_H */
