#include <string.h>

#include "group.h"
#include "extension.h"

MixGroup *mix_group_new(oss_mixext ext)
{
  MixGroup *group;
  MIX_DBG("Creating group %s", ext.extname);

  group = malloc(sizeof(*group));
  assert(group != NULL);
  group->parent_mixer = NULL;
  group->parent_group = NULL;
  group->mixext = ext;
  group->extensions = NULL;
  group->groups = NULL;
  return group;
}

void mix_group_free(MixGroup *group)
{
  assert(group != NULL);
  MIX_DBG("Freeing group %s", mix_group_get_name(group));

  mix_list_free(group->extensions, (MixFreeFunc) mix_extension_free);
  mix_list_free(group->groups, (MixFreeFunc) mix_group_free);
  free(group);
}

char *mix_group_get_name(MixGroup *group)
{
  assert(group != NULL);
  return group->mixext.extname;
}

MixList *mix_group_get_groups(MixGroup *group)
{
  assert(group != NULL);
  return group->groups;
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

MixGroup *mix_group_get_group(MixGroup *group)
{
  assert(group != NULL);
  return group->parent_group;
}

MixAPIFD mix_group_get_fd(MixGroup *group)
{
  MixMixer *parent_mixer;
  MixGroup *parent_group;
  parent_mixer = mix_group_get_mixer(group);
  parent_group = mix_group_get_group(group);

  assert(parent_group != NULL || parent_mixer != NULL);
  if (parent_mixer != NULL)
    return mix_mixer_get_fd(parent_mixer);
  else
    return mix_group_get_fd(parent_group);
}

void mix_group_update(MixGroup *group)
{
  mix_list_iter(group->groups, (MixIterFunc) mix_group_update);
  mix_list_iter(group->extensions, (MixIterFunc) mix_extension_update_value);
}

void mix_group_finish_add(MixGroup *group)
{
  assert(group != NULL);
  group->groups = mix_list_reverse(group->groups);
  mix_list_iter(group->groups, (MixIterFunc) mix_group_finish_add);
  mix_list_iter(group->extensions, (MixIterFunc) mix_extension_update_value);
  group->extensions = mix_list_reverse(group->extensions);
}

void mix_group_add_child_group(MixGroup *group, MixGroup *child)
{
  MIX_DBG("Group %s: adding group %s",
          mix_group_get_name(group), mix_group_get_name(child));
  assert(group != NULL);

  group->groups = mix_list_prepend(group->groups, child);
  mix_group_set_parent_group(child, group);
}

void mix_group_add_child_extension(MixGroup *group, MixExtension *ext)
{
  MIX_DBG("Group %s: adding extension %s",
          mix_group_get_name(group), mix_extension_get_name(ext));
  assert(group != NULL);

  group->extensions = mix_list_prepend(group->extensions, ext);
  mix_extension_set_parent_group(ext, group);
}

void mix_group_set_parent_mixer(MixGroup *group, MixMixer *mixer)
{
  MIX_DBG("Group %s: setting parent mixer %s",
          mix_group_get_name(group), mix_mixer_get_name(mixer));
  assert(group != NULL);

  group->parent_mixer = mixer;
}

void mix_group_set_parent_group(MixGroup *group, MixGroup *parent)
{
  MIX_DBG("Group %s: setting parent group %s",
          mix_group_get_name(group), mix_group_get_name(parent));
  assert(group != NULL);

  group->parent_group = parent;
}

MixExtension *mix_group_find_extension(MixGroup *group, const char *name)
{
  MixList *iterator;
  MixExtension *ext;
  MixGroup *child_group;
  mix_foreach(iterator, mix_group_get_extensions(group)) {
    ext = iterator->data;
    if (strcmp(mix_extension_get_name(ext), name) == 0)
      return ext;
  }

  mix_foreach(iterator, mix_group_get_groups(group)) {
    child_group = iterator->data;
    ext = mix_group_find_extension(child_group, name);
    if (ext != NULL)
      return ext;
  }
  return NULL;
}

MixGroup *mix_group_find_group(MixGroup *group, const char *name)
{
  MixList *iterator;
  MixGroup *child_group, *child_child_group;

  mix_foreach(iterator, mix_group_get_groups(group)) {
    child_group = iterator->data;
    if (strcmp(mix_group_get_name(child_group), name) == 0)
      return child_group;

    child_child_group = mix_group_find_group(child_group, name);
    if (child_child_group != NULL)
      return child_child_group;
  }
  return NULL;
}
