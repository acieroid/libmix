#include "mixer.h"

void mix_mixer_free(MixMixer *mixer)
{
  assert(mixer != NULL);
  if (mixer->name != NULL)
    free(mixer->name);
  free(mixer);
}
char *mix_mixer_get_name(MixMixer *mixer)
{
  assert(mixer != NULL);
  return mixer->name;
}

MixList *mix_mixer_get_groups(MixMixer *mixer)
{
  assert(mixer != NULL);
  return mixer->groups;
}

int mix_mixer_get_card_number(MixMixer *mixer)
{
  assert(mixer != NULL);
  return mixer->card_number;
}
