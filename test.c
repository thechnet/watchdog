#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <wchar.h>
#include "watchdog.h"
#define LOGGING_ID "#####"
#include "logging/logging.h"

int main(void)
{
  // char *a = malloc(5);
  // a = realloc(a, 10);
  // free(a+1);
  
  char *abc = "abc";
  wd_ignore(__FILE__, __LINE__, abc, strlen(abc)+1);
  char *def = "sdsdsd";
  wd_ignore(__FILE__, __LINE__, def, strlen(def)+1);
  char *a = malloc(4*sizeof(*a));
  char *b = strdup(abc+1);
  strcpy(b, def);
  b = realloc(b, 50);
  strcpy(b+1, def);
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
