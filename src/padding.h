/*
padding.h - watchdog
Modified 2021-12-04
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
#define WD_PADDING_CLEAR_CHAR 0

/*
*** Padding globals.
*/

extern char wd_padding[WD_PADDING_SIZE];
extern bool wd_padding_generated;

/*
*** Padding preprocessor functions.
*/

#define WD_TO_REAL(address_virtual) (address_virtual-WD_PADDING_SIZE)
#define WD_TO_VIRTUAL(address_real) (address_real+WD_PADDING_SIZE)

/*
*** Padding interface.
*/

void wd_padding_generate(void);
void wd_padding_check(WD_STD_PARAMS, wd_alloc *alloc);
void wd_padding_write(wd_alloc *alloc);
void wd_padding_clear(wd_alloc *alloc);

#endif /* !WD_PADDING_H */
