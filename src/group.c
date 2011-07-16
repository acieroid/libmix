#include <string.h>

#include "group.h"
#include "extension.h"

MixGroup *mix_group_new(MixMixer *parent, MixGroup *parent_group, oss_mixext ext)
{
  MixGroup *group = malloc(sizeof(*group));
  assert(group != NULL);
  group->parent_mixer = parent;
  group->parent_group = parent_group;
  group->mixext = ext;
  group->extensions = NULL;
  return group;

}

void mix_group_free(MixGroup *group)
{
  assert(group != NULL);
  mix_list_free(group->extensions, (MixFreeFunc) mix_extension_free);
  mix_list_free(group->groups, (MixFreeFunc) mix_group_free);
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
