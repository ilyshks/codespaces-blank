#include "threads_sum.h"

int Sum(const struct SumArgs *args) {
  int sum = 0;
  for (int i = args->begin; i < args->end; i++) {
    sum += args->array[i];
  }
  //printf("begin=%d, end=%d, sum=%d\n", args->begin, args->end, sum);
  return sum;
}
