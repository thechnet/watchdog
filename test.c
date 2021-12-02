#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "watchdog.h"

int main(void)
{
  char *a = malloc(5*sizeof(*a));
  strcpy(a, "abcdefdsf");
  a = realloc(a, 10);
  free(a);
  
  printf("--- Summary:\n");
}
