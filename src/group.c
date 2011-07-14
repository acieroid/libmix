#include <string.h>

#include "group.h"

MixGroup *mix_group_new(MixMixer *parent, oss_mixext ext)
{
  MixGroup *group = malloc(sizeof(*group));
  assert(group != NULL);
  group->parent_mixer = parent;
  group->mixext = ext;
  return group;

}

void mix_group_free(MixGroup *group)
{
  assert(group != NULL);
  free(group);
}

char *mix_group_get_name(MixGroup *group)
{
  assert(group != NULL);
  return group->mixext.extname;
}

MixList *mix_group_get_extensions(MixGroup *group)
{
  assert(group != NULL);
  return group->extensions;
}

MixMixer *mix_group_get_mixer(MixGroup *group)
{
  assert(group != NULL);
  return group->parent_mixer;
}

MixAPIFD mix_group_get_fd(MixGroup *group)
{
  return mix_mixer_get_fd(mix_group_get_mixer(group));
}
