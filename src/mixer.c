#include "mixer.h"
#include "group.h"

MixMixer *mix_mixer_new(MixAPIFD fd)
{
  MixMixer *mixer = malloc(sizeof(*mixer));
  assert(mixer != NULL);

  mixer->fd = fd;
  mixer->groups = NULL;

  return mixer;
}

void mix_mixer_free(MixMixer *mixer)
{
  assert(mixer != NULL);
  mix_list_free(mixer->groups, (MixFreeFunc) mix_group_free);
  free(mixer);
}
char *mix_mixer_get_name(MixMixer *mixer)
{
  assert(mixer != NULL);
  return mixer->mixerinfo.name;
}

MixList *mix_mixer_get_groups(MixMixer *mixer)
{
  assert(mixer != NULL);
  return mixer->groups;
}

int mix_mixer_get_card_number(MixMixer *mixer)
{
  assert(mixer != NULL);
  return mixer->mixerinfo.card_number;
}

MixAPIFD mix_mixer_get_fd(MixMixer *mixer)
{
  assert(mixer != NULL);
  return mixer->fd;
}
