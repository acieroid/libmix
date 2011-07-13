#include <libmix.h>

int main()
{
  MixerAPIFD fd = mix_open_dev("/dev/mixer");
  MixList *mixers = mix_get_mixers(fd);

  mix_close_dev(fd);
  return 0;
}
