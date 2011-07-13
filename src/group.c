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
  free(group->name);
  free(group);
}
