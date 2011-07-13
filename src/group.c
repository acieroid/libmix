#include <string.h>

#include "group.h"

MixGroup *mix_group_new(MixMixer *parent, oss_mixext ext)
{
  MixGroup *group = malloc(sizeof(*group));
  assert(group != NULL);
  group->parent_mixer = parent;
  group->name = strdup(ext.extname);
  return group;

}

void mix_group_free(MixGroup *group)
{
  assert(group != NULL);
  if (group->name != NULL)
    free(group->name);
  free(group);
}

char *mix_group_get_name(MixGroup *group)
{
  assert(group != NULL);
  return group->name;
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
