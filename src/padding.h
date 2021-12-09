/*
padding.h - watchdog
Modified 2021-12-09
*/

#ifndef WD_PADDING_H
#define WD_PADDING_H

/* Header-specific includes. */
#include "dogshed.h"
#include "radar.h"

/*
*** Padding constants.
*/

#define WD_PADDING_SIZE 32
#define WD_SIZE_REAL_MARKUP (2*WD_PADDING_SIZE)
#define WD_PADDING_CLEAR_CHAR 0

/*
*** Padding globals.
*/

extern char wd_padding[WD_PADDING_SIZE];
extern bool wd_padding_generated;

/*
*** Padding preprocessor functions.
*/

// FIXME: Do we need these?
#define WD_ADDR_USER_MAKE_REAL(addr_user) (addr_user-WD_PADDING_SIZE)
#define WD_ADDR_REAL_MAKE_USER(addr_real) (addr_real+WD_PADDING_SIZE)
#define WD_SIZE_USER_MAKE_REAL(size_user) (WD_PADDING_SIZE+size_user+WD_PADDING_SIZE)

/*
*** Padding interface.
*/

void wd_padding_generate(void);
void wd_padding_check(WD_STD_PARAMS, wd_alloc *alloc);
void wd_padding_write_left(wd_alloc *alloc);
void wd_padding_write_right(wd_alloc *alloc);
void wd_padding_write(wd_alloc *alloc);
void wd_padding_clear_left(wd_alloc *alloc);
void wd_padding_clear_right(wd_alloc *alloc);
void wd_padding_clear(wd_alloc *alloc);

#endif /* !WD_PADDING_H */
