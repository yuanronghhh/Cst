#include "glib.h"
#include <math.h>

int is_prime2(int n) {
  int i, tmp;
  if(n == 1) { return 0; }

  tmp = sqrt(n);
  for (i = 2; i <= tmp; i++) {
    if (n % i == 0) {
      return 0;
    }
  }

  return 1;
}

int is_prime3(int n) {
  int tmp;
  if(n < 2) { return 0; }

  if(n == 2 || n == 3) {
    return 1;
  }

  if(n % 6 != 1 && n % 6 != 5) {
    return 0;
  }

  tmp = sqrt(n);

  for(int i = 5; i <= tmp; i += 6) {
    if(n % i == 0 || n % (i + 2) == 0) {
      return 0;
    }
  }

  return 1;
}

int is_prime4(int n) {
  double root;

  if (n < 2) { return 0; }
  if (n % 2 == 0) { return (n == 2); }

  root = sqrt(n);

  for (int i = 3; i <= root; i += 2) {
    if(n % i  == 0) {
      return 0;
    }
  }

  return 1;
}

int is_prime5(int n) {
  if(n < 2) { return 0; }
  if(n == 2 || n == 3) { return 1; }

  if( n % 2 == 0 || n % 3 == 0) { return 0; }

  long root = sqrt(n) + 1;
  for(long i = 6L; i <= root; i += 6) {
    if( n % (i - 1) == 0 || n % (i + 1) == 0) {
      return 0;
    }
  }

  return 1;
}

void test_is_prime() {
  int i;
  int max = 1000000;
  int end, start;
  int c2 = 0, c3 = 0, c4 = 0, c5 = 0;

  start = g_get_monotonic_time();
  for(i = 1; i <= max; i++) {
    if(is_prime2(i)) {
      c2 += 1;
    }
  }
  end = g_get_monotonic_time();
  printf("[Prime 2]%d\t%d\n", end - start, c2);

  start = g_get_monotonic_time();
  for(i = 1; i <= max; i++) {
    if(is_prime3(i)) {
      c3 += 1;
    }
  }
  end = g_get_monotonic_time();
  printf("[Prime 3]%d\t%d\n", end - start, c3);


  start = g_get_monotonic_time();
  for(i = 1; i <= max; i++) {
    if(is_prime4(i)) {
      c4 += 1;
    }
  }
  end = g_get_monotonic_time();
  printf("[Prime 4]%d\t%d\n", end - start, c4);


  start = g_get_monotonic_time();
  for(i = 1; i <= max; i++) {
    if(is_prime5(i)) {
      c5 += 1;
    }
  }
  end = g_get_monotonic_time();
  printf("[Prime 5]%d\t%d\n", end - start, c5);
}
