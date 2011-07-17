/**
 * Rewrite of Daniel J Griffiths's ossvol
 * (http://ghost1227.com/ossvol)
 */
#include <stdio.h>
#include <stdlib.h>
#include <libmix.h>

static const char channel[] = "vmix0-outvol";

void print_usage()
{
  printf("usage: ossvol [option] [argument]\n\n");
  printf("Options:\n");
  printf("     -i, --increase - increase volume by [argument]\n");
  printf("     -d, --decrease - decrease volume by [argument]\n");
  printf("     -t, --toggle   - toggle mute on and off\n");
  printf("     -h, --help     - display this\n");
  exit(0);
}

void increase(int step)
{
  /* open the device */
  MixAPIFD fd = mix_open_dev("/dev/mixer");
  /* get the first (and usually the only) mixer */
  MixMixer *mixer = mix_get_mixer(fd, 0);
  /* find the extension we want to change */
  MixExtension *ext = mix_mixer_find_extension(mixer, channel);
  /* change its value */
  mix_extension_set_value(ext, mix_extension_get_value(ext) + step);
  /* free the mixer */
  mix_mixer_free(mixer);
  /* close the device */
  mix_close_dev(fd);
}

void decrease(int n)
{
  increase(-n);
}

void toggle()
{
  printf("TODO\n");
}

int main(int argc, char *argv[])
{
  int step = 1;
  if (argc >= 3)
    step = strtol(argv[2], NULL, 10);

  if (argc == 1 || strcmp(argv[1], "-h") == 0 ||
      strcmp(argv[1], "--help") == 0) {
    print_usage();
  }
  else if (strcmp(argv[1], "-i") == 0 ||
           strcmp(argv[1], "--increase") == 0) {
    increase(step);
  }
  else if (strcmp(argv[1], "-d") == 0 ||
           strcmp(argv[1], "--decrease") == 0) {
    decrease(step);
  }
  else if (strcmp(argv[1], "-t") == 0 ||
           strcmp(argv[1], "--toggle") == 0) {
    toggle();
  }
  else {
    printf("Unrecognized option `%s', see ossvol --help\n", argv[1]);
    return 1;
  }

  return 0;
}
