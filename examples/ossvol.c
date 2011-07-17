/**
 * Rewrite of Daniel J Griffiths's ossvol
 * (http://ghost1227.com/ossvol)
 *
 * The duo VOLSTORE/MUTSTORE is replaced by only a "mute store" that
 * contains the volume when we mute it
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <libmix.h>

#define OSS_DEVICE "/dev/mixer"
#define CHANNEL "vmix0-outvol"
#define MUTSTORE "/tmp/volume"

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

int mute_store_exists()
{
  FILE *f = fopen(MUTSTORE, "r");
  if (f == NULL)
    return 0;
  fclose(f);
  return 1;
}

int mute_store_get_and_rm()
{
  int value = 0;
  char str[16];
  /* open the file */
  FILE *f = fopen(MUTSTORE, "r");
  if (f == NULL) {
    perror("fopen");
    exit(1);
  }
  /* read the value */
  fscanf(f, "%d", &value);
  /* close the file */
  fclose(f);
  /* and delete the file */
  unlink(MUTSTORE);

  return value;
}

void mute_store_save(int value)
{
  FILE *f = fopen(MUTSTORE, "w");
  if (f == NULL) {
    perror("fopen");
    exit(1);
  }
  fprintf(f, "%d", value);
  fclose(f);
}

void increase(int step)
{
  /* open the device */
  MixAPIFD fd = mix_open_dev(OSS_DEVICE);
  /* get the first (and usually the only) mixer */
  MixMixer *mixer = mix_get_mixer(fd, 0);
  /* find the extension we want to change */
  MixExtension *ext = mix_mixer_find_extension(mixer, CHANNEL);
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
  MixAPIFD fd;
  MixMixer *mixer;
  MixExtension *ext;
  if (mute_store_exists() == 1) {
    /* if it is muted, then the actual value is 0 */
    increase(mute_store_get_and_rm());
  }
  else {
    fd = mix_open_dev(OSS_DEVICE);
    mixer = mix_get_mixer(fd, 0);
    ext = mix_mixer_find_extension(mixer, CHANNEL);
    mute_store_save(mix_extension_get_value(ext));
    mix_extension_set_value(ext, 0);
    mix_mixer_free(mixer);
    mix_close_dev(fd);
  }
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
