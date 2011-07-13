#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

#include "libmix.h"
#include "group.h"
#include "extension.h"

MixerAPIFD mix_open_dev(const char *dev)
{
  MixerAPIFD fd;
  if ((fd = open(dev, O_RDWR, 0)) == -1)
    perror("open");
  return fd;
}

void mix_close_dev(MixerAPIFD fd)
{
  assert(fd != -1);
  if (close(fd) == -1)
    perror("close");
}

int mix_get_number_of_mixers(MixerAPIFD fd)
{
  int n = 0;
  OSS_CALL(fd, SNDCTL_MIX_NRMIX, &n);
  return n;
}

MixMixer *mix_get_mixer(MixerAPIFD fd, int n)
{
  MixMixer *mixer = NULL;
  MixGroup *group = NULL;
  MixExtension *extension = NULL;
  oss_mixerinfo mixerinfo;
  oss_mixext mixext;
  int n_ext, i;

  mixer = malloc(sizeof(*mixer));
  assert(mixer != NULL);
  mixer->fd = fd;

  /* gather the mixer informations */
  mixerinfo.dev = n;
  OSS_CALL(fd, SNDCTL_MIXERINFO, &mixerinfo);
  mixer->name = strdup(mixerinfo.name);
  mixer->card_number = mixerinfo.card_number;
  /* get all the extensions and extract their informations (groups etc.) */
  n_ext = n;
  OSS_CALL(fd, SNDCTL_MIX_NREXT, &n_ext);
  for (i = 0; i < n_ext; i++) {
    mixext.dev = n;
    mixext.ctrl = i;
    OSS_CALL(fd, SNDCTL_MIX_EXTINFO, &mixext);

    switch (mixext.type) {
    case MIXT_DEVROOT:
      break;
    case MIXT_GROUP:
      if (group != NULL) {
        /* group->extensions = mix_list_reverse(group->extensions); */
        mixer->groups = mix_list_prepend(mixer->groups, (void *) group);
      }
      group = mix_group_new(NULL, mixext);
      break;
    default:
      assert (group != NULL);
      extension = mix_extension_new(group, mixext);
      group->extensions = mix_list_prepend(group->extensions, (void *) extension);
      break;
      
    }
  }
  
  /* mixer->groups = mix_list_reverse(mixer->groups); */
  return mixer;
}

MixList *mix_get_mixers(MixerAPIFD fd)
{
  MixList *result = NULL;
  int i;
  int n = mix_get_number_of_mixers(fd);
  
  for (i = 0; i < n; i++)
    result = mix_list_prepend(result, (void *) mix_get_mixer(fd, i));
  
  return result;
}
