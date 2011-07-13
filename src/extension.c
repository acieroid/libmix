#include <string.h>

#include "extension.h"

MixExtension *mix_extension_new(MixGroup *parent, oss_mixext ext)
{
  MixExtension *extension = malloc(sizeof(extension));
  assert(extension != NULL);
  
  extension->parent_group = parent;
  extension->name = strdup(ext.extname);
  #ifdef OSS_RGB_RED
  extension->color = mix_color_new_from_24bit(ext.rgbcolor);
  #endif
  extension->type = ext.type;
  extension->max_value = ext.maxvalue;
  extension->min_value = ext.minvalue;
  mix_extension_update_value(extension);
  return extension;
}

void mix_extension_free(MixExtension *extension)
{
  free(extension->name);
  free(extension);
}

void mix_extension_update_value(MixExtension *extension)
{
  /* TODO */
  oss_mixer_value val;
  switch (extension->type) {
  case MIXT_DEVROOT: /* already handled in mix_get_mixer */
  case MIXT_GROUP: /* already handled in mix_get_mixer */
    break;
  default:
    fprintf(stderr, "Unknown or not yet handled extension type: %d\n",
            extension->type);
    break;
  }
}
