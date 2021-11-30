#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "watchdog.h"

int main(void)
{
  int *a = malloc(1);
  int *b = malloc(1);
  free(a);
  free(b);
  
  wprintf(L"Done.\n");
}
