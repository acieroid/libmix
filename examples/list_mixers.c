#include <libmix.h>
#include <stdio.h>

void indent(int indentation_level)
{
  if (indentation_level > 0) {
    printf("\t");
    indent(indentation_level-1);
  }
}

void extension_print(MixExtension *ext, int indent_level)
{
  MixColor *color = mix_extension_get_color(ext);
  indent(indent_level);
  printf("Extension %s\n", mix_extension_get_name(ext));
  indent(indent_level+1);
  printf("Color: %d, %d, %d\n", color->red, color->green, color->blue);
  indent(indent_level+1);
  printf("Min value: %d\n", mix_extension_get_min_value(ext));
  indent(indent_level+1);
  printf("Max value: %d\n", mix_extension_get_max_value(ext));
  indent(indent_level+1);
  if (mix_extension_is_enum(ext)) {
    printf("Value: %s\n", mix_extension_get_enum_value(ext));
  }
  else if (mix_extension_is_stereo(ext)) {
    printf("Left value: %d\n", mix_extension_get_left_value(ext));
    indent(indent_level+1);
    printf("Right value: %d\n", mix_extension_get_right_value(ext));
  }
  else {
    printf("Value: %d\n", mix_extension_get_value(ext));
  }
}

void group_print(MixGroup *group, int indentation_level)
{
  MixList *iterator;

  indent(indentation_level);
  printf("Group %s\n", mix_group_get_name(group));

  mix_foreach(iterator, mix_group_get_groups(group)) {
    group_print(iterator->data, indentation_level+1);
  }
  mix_foreach(iterator, mix_group_get_extensions(group)) {
    extension_print(iterator->data, indentation_level+1);
  }
}

void mixer_print_extensions(MixMixer *mixer)
{
  MixList *iterator;
  mix_foreach(iterator, mix_mixer_get_extensions(mixer)) {
    extension_print(iterator->data, 1);
  }
}

void mixer_print_groups(MixMixer *mixer)
{
  MixList *iterator;
  mix_foreach(iterator, mix_mixer_get_groups(mixer)) {
    group_print(iterator->data, 1);
  }
}

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
    mixer_print_extensions(mixer);
    mixer_print_groups(mixer);
  }

  mix_list_free(mixers, (MixFreeFunc) mix_mixer_free);
  mix_close_dev(fd);
  return 0;
}
