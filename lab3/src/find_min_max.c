#include "find_min_max.h"
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <limits.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (unsigned int i=begin; i<end; ++i){
    min_max.min = MIN(min_max.min, array[i]);
    min_max.max = MAX(min_max.max, array[i]);
  }

  return min_max;
}
