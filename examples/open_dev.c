#include "libmix.h"

int main()
{
  MixAPIFD fd = mix_open_dev("/dev/mixer");
  mix_close_dev(fd);
  return 0;
}
