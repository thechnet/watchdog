/*
padding.c - watchdog
Modified 2021-12-09
*/

/* Header-specific includes. */
#include "padding.h"

/* Implementation-specific includes. */
#include <memory.h>
#include "reporter.h"
#include "radar.h"

/*
*** Padding globals.
*/

char wd_padding[WD_PADDING_SIZE];
bool wd_padding_generated = false;

/*
*** Padding interface.
*/

/*
Populate padding array.
*/
void wd_padding_generate(void)
{
  wd_fill_with_random_bytes(wd_padding, WD_PADDING_SIZE);
  wd_padding_generated = true;
}

/*
Check padding of an allocation.
*/
void wd_padding_check(WD_STD_PARAMS, wd_alloc *alloc)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_padding_generated);
  assert(alloc->padding_check_left || alloc->padding_check_right);
  assert(alloc->addr_user != NULL);
  
  /*
  Check padding.
  */
  
  /* Left. */
  if (alloc->padding_check_left && memcmp(alloc->addr_user-WD_PADDING_SIZE, wd_padding, WD_PADDING_SIZE) != 0) {
    alloc->padding_check_left = false;
    wd_alerts++;
    warn_at(file, line, WD_MSG_PADDING_LEFT, alloc->point.file, alloc->point.line);
  }
  
  /* Right. */
  if (alloc->padding_check_right && memcmp(alloc->addr_user+alloc->size_user, wd_padding, WD_PADDING_SIZE) != 0) {
    alloc->padding_check_right = false;
    wd_alerts++;
    warn_at(file, line, WD_MSG_PADDING_RIGHT, alloc->point.file, alloc->point.line);
  }
}

/*
Write left padding to the end of an allocation.
*/
void wd_padding_write_left(wd_alloc *alloc)
{
  assert(wd_padding_generated);
  assert(alloc != NULL);
  assert(alloc->addr_user != NULL);
  assert(alloc->is_protected);
  memcpy(alloc->addr_user-WD_PADDING_SIZE, wd_padding, WD_PADDING_SIZE);
}

/*
Write right padding to the end of an allocation.
*/
void wd_padding_write_right(wd_alloc *alloc)
{
  assert(wd_padding_generated);
  assert(alloc != NULL);
  assert(alloc->addr_user != NULL);
  assert(alloc->is_protected);
  memcpy(alloc->addr_user+alloc->size_user, wd_padding, WD_PADDING_SIZE);
}

/*
Write padding to the end of an allocation.
*/
void wd_padding_write(wd_alloc *alloc)
{
  wd_padding_write_left(alloc);
  wd_padding_write_right(alloc);
}

/*
Clear left padding from the end of an allocation.
*/
void wd_padding_clear_left(wd_alloc *alloc)
{
  assert(wd_padding_generated);
  assert(alloc != NULL);
  assert(alloc->addr_user != NULL);
  assert(alloc->is_protected);
  memset(alloc->addr_user-WD_PADDING_SIZE, WD_PADDING_CLEAR_CHAR, WD_PADDING_SIZE);
}

/*
Clear right padding from the end of an allocation.
*/
void wd_padding_clear_right(wd_alloc *alloc)
{
  assert(wd_padding_generated);
  assert(alloc != NULL);
  assert(alloc->addr_user != NULL);
  assert(alloc->is_protected);
  memset(alloc->addr_user+alloc->size_user, WD_PADDING_CLEAR_CHAR, WD_PADDING_SIZE);
}

/*
Clear padding from the end of an allocation.
*/
void wd_padding_clear(wd_alloc *alloc)
{
  wd_padding_clear_left(alloc);
  wd_padding_clear_right(alloc);
}
