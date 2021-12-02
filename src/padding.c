/*
padding.c - watchdog
Modified 2021-12-02
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
  // TODO: Just use srand(time(NULL))? [https://www.cplusplus.com/reference/cstdlib/srand/]
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
  assert(alloc->check_padding);
  assert(alloc->memory != NULL);
  
  /* Check padding. */
  if (memcmp(alloc->memory+alloc->size, wd_padding, WD_PADDING_SIZE) != 0) {
    alloc->check_padding = false;
    wd_alerts++;
    warn_at(alloc->origin.file, alloc->origin.line, WD_MSG_PADDING, WD_STD_PARAMS_PASS);
  }
}
