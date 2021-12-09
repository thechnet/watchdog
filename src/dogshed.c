/*
dogshed.c - watchdog
Modified 2021-12-09
*/

/* Header-specific includes. */
#include "dogshed.h"

/* Implementation-specific includes. */
#include <time.h>
#include <limits.h>

/*
*** Tools.
*/

/*
Write integer to stdout.
*/
void wd_int_out(int num)
{
  /* Get starting divisor. */
  int divisor = 1;
  for (int dividend=num; dividend>=10; dividend /= 10)
    divisor *= 10;
  
  /* Write integer. */
  while (divisor > 0) {
    int quotient = num/divisor;
    num -= quotient*divisor;
    divisor /= 10;
    write(STDOUT_FILENO, (char[1]){'0'+quotient}, 1);
  }
}

/*
Fill a section of memory with random bytes.
*/
void wd_fill_with_random_bytes(char *memory, size_t count)
{
  static bool initialized = false;
  if (!initialized) {
    srand(time(NULL));
    initialized = true;
  }
  
  for (size_t i=0; i<count; i++)
    memory[i] = rand() % CHAR_MAX;
}
