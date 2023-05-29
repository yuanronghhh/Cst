#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void run(void) {
  for (float y = 1.5f; y > -1.5f; y -= 0.1f) {
    for (float x = -1.5f; x < 1.5f; x += 0.05f) {
      float a = x * x + y * y - 1;
      printf("\e[0;49;95m%c", a * a * a - x * x * y * y * y < 0.0f ? '*' : ' ');
    }

    printf("\n");
  }
}
