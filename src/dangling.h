/*
dangling.h - watchdog
Modified 2021-12-04
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
*** Dangling types.
*/

typedef struct _wd_dangling_pointer {
  char *address;
  wd_point freed_at;
} wd_dangling_pointer;

/*
*** Dangling globals.
*/

extern size_t wd_dangling_size;
extern wd_dangling_pointer *wd_dangling;

/*
*** Dangling interface.
*/

void wd_dangling_open(void);
void wd_dangling_close(void);
wd_dangling_pointer *wd_dangling_find(WD_STD_PARAMS, char *address);
void wd_dangling_record(WD_STD_PARAMS, char *address);
void wd_dangling_erase(wd_dangling_pointer *pointer);
bool wd_dangling_find_and_erase(char *address);

#endif /* !WD_DANGLING_H */
