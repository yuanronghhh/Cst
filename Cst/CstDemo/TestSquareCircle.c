#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void run(void) {
  int i,
      loopc = 198, /* 循环次数 */
      sg_sum,
      n = 8,
      max = 0,
      sum = 0;

  for (i = 0; i < loopc; i++) {
    while(n!=0) {
      /* 从个位开始取，得到每一位的值 */
      sg_sum = n % 10;

      /* 得到每一位的平方 */
      sum += sg_sum * sg_sum;

      /* 去掉尾部后的值 */
      n /= 10;
    }

    if (max < sum) {
      max = sum;
    }

    n = sum;
    sum = 0;
  }

  printf("[ans] %d\n", max);
}
