/*
usage.c - watchdog
Modified 2021-12-02
*/

/* Header-specific includes. */
#include "usage.h"

/*
*** Usage globals.
*/

size_t wd_usage_current = 0;
size_t wd_usage_current_internal = 0;
size_t wd_usage_max = 0;
size_t wd_usage_max_internal = 0;

/*
*** Usage interface.
*/

/*
Reset usage metrics.
*/
void wd_usage_reset(void)
{
  wd_usage_current = 0;
  wd_usage_current_internal = 0;
  wd_usage_max = 0;
  wd_usage_max_internal = 0;
}

/*
Add amount to current memory usage.
*/
void wd_usage_add(size_t amount)
{
  wd_usage_current += amount;
  if (wd_usage_current > wd_usage_max)
    wd_usage_max = wd_usage_current;
}

/*
Subtract amount from current memory usage.
*/
void wd_usage_subtract(size_t amount)
{
  assert((int64_t)wd_usage_current - amount >= 0); // FIXME:?
  wd_usage_current -= amount;
}
