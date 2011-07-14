#include "util.h"

int mix_number_length(int n)
{
  if (n < 0)
    return 1 + mix_number_length(-n);
  if (n < 10)
    return 1;
  return 1 + mix_number_length(n/10);
}
