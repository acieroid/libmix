#include "libmix.h"

int main()
{
  MixerAPIFD fd = mix_open_dev("/dev/mixer");
  mix_close_dev(fd);
  return 0;
}
