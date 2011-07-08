#include <stdio.h>
#include <fcntl.h>
#include <assert.h>

#include "libmix.h"

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

MixList *mix_get_mixers(MixerAPIFD fd)
{
  /* TODO */
  return NULL;
}
