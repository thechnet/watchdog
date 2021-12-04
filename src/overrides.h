/*
overrides.h - watchdog
Modified 2021-12-04
*/

#ifndef WD_OVERRIDES_H
#define WD_OVERRIDES_H

/* Header-specific includes. */
#include <stdlib.h>

/*
*** Overrides.
*/

void *wd_override_malloc(char *file, size_t line, size_t size);
void *wd_override_realloc(char *file, size_t line, void *memory, size_t new_size);
void wd_override_free(char *file, size_t line, void *memory);
void wd_override_assert(char *file, size_t line, char *assertion_string, int assertion);

#endif /* !WD_OVERRIDES_H */
