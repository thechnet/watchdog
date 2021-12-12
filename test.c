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
  char *a = malloc(10);
  free(a);
  a = realloc(a, 20);
  
  printf("--- Summary:\n");
}
