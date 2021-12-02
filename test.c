#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "watchdog.h"

int main(void)
{
  int *a = malloc(1);
  a = realloc(a, 0);
  free(a);
  
  wprintf(L"Done.\n");
}
