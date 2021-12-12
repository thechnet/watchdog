/*
archive.h - watchdog
Modified 2021-12-12
*/

#ifndef WD_ARCHIVE_H
#define WD_ARCHIVE_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Archive constants.
*/

#define WD_ARCHIVE_SIZE ((size_t)1) // FIXME: 4096

/*
*** Archive types.
*/

typedef struct _wd_archived {
  char *addr_user;
  wd_point freed_at;
} wd_archived;

/*
*** Archive globals.
*/

extern size_t wd_archive_size;
extern wd_archived *wd_archive;

/*
*** Archive interface.
*/

void wd_archive_initialize(void);
void wd_archive_terminate(void);
void wd_archive_record(WD_STD_PARAMS, char *addr_user);
void wd_archive_erase(wd_archived *pointer);
wd_archived *wd_archive_query(char *addr_user);

#endif /* !WD_ARCHIVE_H */
