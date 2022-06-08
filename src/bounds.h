/*
bounds.h - watchdog
Modified 2022-06-08
*/

#ifndef WD_BOUNDS_H
#define WD_BOUNDS_H

/* Header-specific includes. */
#include <stdlib.h>
#include <stddef.h>

/*
*** Bounds interface.
*/

ptrdiff_t wd_bounds_check(char *file, long line, char *array, size_t array_size, size_t item_size, ptrdiff_t index);

#endif /* !WD_BOUNDS_H */
