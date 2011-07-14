#include <libmix.h>
#include <stdio.h>

int main()
{
  int i;
  MixMixer *mixer;
  MixGroup *group;
  MixExtension *ext;
  MixList *mixer_iterator, *group_iterator, *ext_iterator;
  MixAPIFD fd = mix_open_dev("/dev/mixer");
  int n = mix_get_number_of_mixers(fd);
  MixList *mixers = mix_get_mixers(fd);

  printf("Number of mixers: %d\n", n);
  mix_foreach(mixer_iterator, mixers) {
    mixer = mixer_iterator->data;
    printf("Mixer %s on card %d\n",
           mix_mixer_get_name(mixer),
           mix_mixer_get_card_number(mixer));
    mix_foreach(group_iterator, mix_mixer_get_groups(mixer)) {
      group = group_iterator->data;
      printf("\tGroup %s\n", mix_group_get_name(group));
      mix_foreach(ext_iterator, mix_group_get_extensions(group)) {
        ext = ext_iterator->data;
        MixColor *color = mix_extension_get_color(ext);
        printf("\t\tExtension %s\n", mix_extension_get_name(ext));
        printf("\t\t\tColor: %d, %d, %d\n", color->red, color->green, color->blue);
        if (mix_extension_get_type(ext) == MIXT_ENUM) {
          printf("\t\t\tValue: %s\n", mix_extension_get_enum_value(ext));
        }
        else {
          printf("\t\t\tValue: %d\n", mix_extension_get_value(ext));
        }
      }
    }
  }

  mix_list_free(mixers, (MixFreeFunc) mix_mixer_free);
  mix_close_dev(fd);
  return 0;
}
