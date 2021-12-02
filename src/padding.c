/*
padding.c - watchdog
Modified 2021-11-30
*/

/* Header-specific includes. */
#include "padding.h"

/* Implementation-specific includes. */
#include <time.h>
#include <limits.h>

/*
*** Padding globals.
*/

char wd_padding[WD_PADDING_SIZE];
bool wd_padding_generated = false;

/*
*** Padding interface.
*/

/*
Get current time.
*/
void wd_time_get(void)
{
  time_t time_now = time(NULL);
  struct tm *time_local = localtime(&time_now);
  return (wd_time){
    .hour = time_local->tm_hour,
    .minute = time_local->tm_min,
    .second = time_local->tm_sec
  };
}

/*
Populate padding array.
*/
void wd_padding_generate(void)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  
  // TODO: Just use srand(time(NULL))? [https://www.cplusplus.com/reference/cstdlib/srand/]
  wd_time current_time = wd_time_get();
  uint seed = current_time.hour * current_time.minute * current_time.second;
  srand(seed);
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
  if (memcmp(alloc->memory+alloc->memory_size, wd_padding, WD_PADDING_SIZE) != 0) {
    alloc->check_padding = false;
    wd_alerts++;
    warn_at(alloc->origin.file, alloc->origin.line, WD_MSG_PADDING, WD_STD_ARGS);
  }
}

/*
Check padding of all allocations.
*/
void wd_padding_check_all(WD_STD_PARAMS)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_padding_generated);
  
  
}
