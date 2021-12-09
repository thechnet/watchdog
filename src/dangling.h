/*
dangling.h - watchdog
Modified 2021-12-09
*/

#ifndef WD_DANGLING_H
#define WD_DANGLING_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Dangling constants.
*/

#define WD_DANGLING_SIZE ((size_t)1) // FIXME: 4096

/*
*** Dangling types.
*/

typedef struct _wd_dangling_pointer {
  char *addr_user;
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
void wd_dangling_record(WD_STD_PARAMS, char *addr_user);
void wd_dangling_clear(wd_dangling_pointer *pointer);
wd_dangling_pointer *wd_dangling_search(char *addr_user);
wd_dangling_pointer *wd_dangling_find_spot(void);

#endif /* !WD_DANGLING_H */
