/*
padding.h - watchdog
Modified 2021-12-02
*/

#ifndef WD_PADDING_H
#define WD_PADDING_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Padding constants.
*/

#define WD_PADDING_SIZE 32

/*
*** Padding types.
*/

typedef struct _wd_time {
  int hour;
  int minute;
  int second;
} wd_time;

/*
*** Padding globals.
*/

extern char wd_padding[WD_PADDING_SIZE];
extern bool wd_padding_generated;

/*
*** Padding interface.
*/

void wd_time_get(void);
void wd_padding_generate(void);
void wd_padding_check(WD_STD_PARAMS, wd_alloc *alloc);
void wd_padding_check_all(WD_STD_PARAMS);

#endif /* !WD_PADDING_H */
