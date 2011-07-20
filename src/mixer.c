#include "mixer.h"
#include "group.h"
#include "extension.h"

MixMixer *mix_mixer_new(MixAPIFD fd)
{
  MIX_DBG("Creating mixer");
  MixMixer *mixer = malloc(sizeof(*mixer));
  assert(mixer != NULL);

  mixer->fd = fd;
  mixer->groups = NULL;
  mixer->extensions = NULL;

  return mixer;
}

void mix_mixer_free(MixMixer *mixer)
{
  MIX_DBG("Freeing mixer %s", mix_mixer_get_name(mixer));
  assert(mixer != NULL);
  mix_list_free(mixer->groups, (MixFreeFunc) mix_group_free);
  mix_list_free(mixer->extensions, (MixFreeFunc) mix_extension_free);
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

MixList *mix_mixer_get_extensions(MixMixer *mixer)
{
  assert(mixer != NULL);
  return mixer->extensions;
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

void mix_mixer_update(MixMixer *mixer)
{
  mix_list_iter(mixer->groups, (MixIterFunc) mix_group_update);
  mix_list_iter(mixer->extensions, (MixIterFunc) mix_extension_update_value);
}

void mix_mixer_finish_add(MixMixer *mixer)
{
  mixer->groups = mix_list_reverse(mixer->groups);
  mix_list_iter(mixer->groups, (MixIterFunc) mix_group_finish_add);
  mixer->extensions = mix_list_reverse(mixer->extensions);
  mix_list_iter(mixer->extensions, (MixIterFunc) mix_extension_update_value);
}

void mix_mixer_add_child_group(MixMixer *mixer, MixGroup *group)
{
  MIX_DBG("Mixer %s: adding group %s",
          mix_mixer_get_name(mixer), mix_group_get_name(group));
  mixer->groups = mix_list_prepend(mixer->groups, group);
  mix_group_set_parent_mixer(group, mixer);
}

void mix_mixer_add_child_extension(MixMixer *mixer, MixExtension *ext)
{
  MIX_DBG("Mixer %s: adding extension %s",
          mix_mixer_get_name(mixer), mix_extension_get_name(ext));
  mixer->extensions = mix_list_prepend(mixer->extensions, ext);
  mix_extension_set_parent_mixer(ext, mixer);
}

MixExtension *mix_mixer_find_extension(MixMixer *mixer, const char *name)
{
  MixList *iterator;
  MixExtension *ext;
  MixGroup *group;
  mix_foreach(iterator, mix_mixer_get_extensions(mixer)) {
    ext = iterator->data;
    if (strcmp(mix_extension_get_name(ext), name) == 0)
      return ext;
  }

  mix_foreach(iterator, mix_mixer_get_groups(mixer)) {
    group = iterator->data;
    ext = mix_group_find_extension(group, name);
    if (ext != NULL)
      return ext;
  }
  MIX_WARN("No extensions found when searching for %s", name);
  return NULL;
}

MixGroup *mix_mixer_find_group(MixMixer *mixer, const char *name)
{
  MixList *iterator;
  MixGroup *group, *child_group;

  mix_foreach(iterator, mix_mixer_get_groups(mixer)) {
    group = iterator->data;
    if (strcmp(mix_group_get_name(group), name) == 0)
      return group;

    child_group = mix_group_find_group(group, name);
    if (child_group != NULL)
      return child_group;
  }
  MIX_WARN("No group found when searching for %s", name);
  return NULL;
}
