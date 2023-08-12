#include <stdio.h>

void main() {
  int x = 1;

  int *px = &x;

  printf("%d", *px);
}