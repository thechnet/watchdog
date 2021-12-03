#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "watchdog.h"

int main(void)
{
  int b[4] = {1,2,3,4};
  printf("%zu, %zu, %zu, %zu\n",
    sizeof(b), sizeof(b[0]),
    sizeof(b+2), sizeof((b+1)[0])
  );
  
  char *a = malloc(5*sizeof(*a));
  strcpy(a, "abcdefdsf");
  a = realloc(a, 10);
  free(a);
  
  printf("--- Summary:\n");
}
