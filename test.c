#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <signal.h>
#include "watchdog.h"

int main(void)
{
  char *a = malloc(5*sizeof(*a));
  strcpy(a, "abcd");
  #define a(i) WD_ARRAY(a, i)
  printf("'%c'\n", a(3));
  a = realloc(a, 10);
  free(a);
  (void)WD_ARRAY(a, 0);
  
  // assert(1==2);
  // raise(SIGFPE);
  // raise(SIGSEGV);
  // raise(SIGILL);
  
  printf("--- Summary:\n");
}
