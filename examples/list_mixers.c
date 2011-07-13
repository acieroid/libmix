#include <libmix.h>
#include <stdio.h>

int main()
{
  int i;
  MixMixer *mixer;
  MixGroup *group;
  MixExtension *ext;
  MixList *mixer_iterator, *group_iterator, *ext_iterator;
  MixerAPIFD fd = mix_open_dev("/dev/mixer");
  int n = mix_get_number_of_mixers(fd);
  MixList *mixers = mix_get_mixers(fd);

  printf("Number of mixers: %d\n", n);
  mix_foreach(mixer_iterator, mixers) {
    mixer = mixer_iterator->data;
    printf("Mixer %s on card %d\n", mixer->name, mixer->card_number);
    mix_foreach(group_iterator, mixer->groups) {
      group = group_iterator->data;
      printf("Group %s\n", group->name);
      mix_foreach(ext_iterator, group->extensions) {
        ext = ext_iterator->data;
        printf("Extension %s\n", ext->name);
      }
    }
  }

  mix_close_dev(fd);
  return 0;
}
