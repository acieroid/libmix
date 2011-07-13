#include <libmix.h>
#include <stdio.h>

int main()
{
  int i;
  MixMixer *mixer;
  MixList *iterator;
  MixerAPIFD fd = mix_open_dev("/dev/mixer");
  int n = mix_get_number_of_mixers(fd);
  MixList *mixers = mix_get_mixers(fd);

  printf("Number of mixers: %d\n", n);
  mix_foreach(iterator, mixers) {
    mixer = iterator->data;
    printf("Mixer %s on card %d\n", mixer->name, mixer->card_number);
  }

  mix_close_dev(fd);
  return 0;
}
