#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include "watchdog.h"
#define LOGGING_ID "#####"
#include "logging/logging.h"

int main(void)
{
  char *abc = "abc";
  char *a = malloc(4*sizeof(*a));
  char *b = strdup(abc);
  strcpy(b, "sdsdsd");
  b = realloc(b, 50);
  strcpy(b-1, "sdsdsd");
  free(b);
  free(a);
  
  // char *_ = "abc";
  // char *a = strdup(_);
  // assert(_!=a);
  // a = realloc(a, 100);
  // free(a);
  
  // a[0] = 'a';
  // #define a(i) WD_ARRAY(a, i)
  // (void)a(3);
  // free(a);
  // (void)WD_ARRAY(a, 0);
  
  // assert(1==2);
  // raise(SIGFPE);
  // raise(SIGSEGV);
  // raise(SIGILL);
  
  printf("--- Summary:\n");
}
