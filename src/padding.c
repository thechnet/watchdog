/*
padding.c - watchdog
Modified 2021-12-08
*/

/* Header-specific includes. */
#include "padding.h"

/* Implementation-specific includes. */
#include <time.h>
#include <limits.h>
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
  srand(time(NULL));
  for (int i=0; i<WD_PADDING_SIZE; i++)
    wd_padding[i] = rand() % CHAR_MAX;
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
  assert(alloc->address != NULL);
  
  /*
  Check padding.
  */
  
  /* Left. */
  if (alloc->padding_check_left && memcmp(alloc->address-WD_PADDING_SIZE, wd_padding, WD_PADDING_SIZE) != 0) {
    alloc->padding_check_left = false;
    wd_alerts++;
    warn_at(file, line, WD_MSG_PADDING_LEFT, alloc->point.file, alloc->point.line);
  }
  
  /* Right. */
  if (alloc->padding_check_right && memcmp(alloc->address+alloc->size, wd_padding, WD_PADDING_SIZE) != 0) {
    alloc->padding_check_right = false;
    wd_alerts++;
    warn_at(file, line, WD_MSG_PADDING_RIGHT, alloc->point.file, alloc->point.line);
  }
}

/*
Write padding to the end of an allocation.
*/
void wd_padding_write(wd_alloc *alloc)
{
  assert(wd_padding_generated);
  memcpy(alloc->address-WD_PADDING_SIZE, wd_padding, WD_PADDING_SIZE);
  memcpy(alloc->address+alloc->size, wd_padding, WD_PADDING_SIZE);
}

/*
Clear padding from the end of an allocation.
*/
void wd_padding_clear(wd_alloc *alloc)
{
  assert(wd_padding_generated);
  memset(alloc->address-WD_PADDING_SIZE, WD_PADDING_CLEAR_CHAR, WD_PADDING_SIZE);
  memset(alloc->address+alloc->size, WD_PADDING_CLEAR_CHAR, WD_PADDING_SIZE);
}
