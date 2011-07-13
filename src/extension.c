#include <string.h>

#include "extension.h"

MixExtension *mix_extension_new(MixGroup *parent, oss_mixext ext)
{
  MixExtension *extension = malloc(sizeof(*extension));
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
  assert(extension != NULL);
  if (extension->name != NULL)
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
    MIX_WARN("Unknown or not yet handled extension type: %d\n",
             extension->type);
    break;
  }
}

char *mix_extension_get_name(MixExtension *extension)
{
  assert(extension != NULL);
  return extension->name;
}
