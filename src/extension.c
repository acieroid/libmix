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

void mix_extension_free(MixExtension *ext)
{
  assert(ext != NULL);
  if (ext->name != NULL)
    free(ext->name);
  if (ext->color != NULL)
    mix_color_free(ext->color);
  free(ext);
}

void mix_extension_update_value(MixExtension *ext)
{
  oss_mixer_value val;
  val.dev = ext->id;
  /* TODO */
  oss_mixer_value val;
  switch (ext->type) {
  case MIXT_DEVROOT:
  case MIXT_GROUP:
  case MIXT_MARKER:
    /* already handled in mix_get_mixer */
    break;
  case MIXT_ONOFF: /* 0 -> ON, 1 -> OFF */
  case MIXT_MUTE:  /* 0 -> Muted, 1 -> Not muted */
    OSS_CALL(SNDCTL_MIX_READ, val);
    ext->value = val.value;
    break;
  default:
    MIX_WARN("Unknown or not yet handled extension type: %d\n",
             ext->type);
    break;
  }
}

char *mix_extension_get_name(MixExtension *ext)
{
  assert(ext != NULL);
  return ext->name;
}

MixGroup *mix_extension_get_group(MixExtension *ext)
{
  assert(ext != NULL);
  return ext->parent_group;
}

MixColor *mix_extension_get_color(MixExtension *ext)
{
  assert(ext != NULL);
  return ext->color;
}

int mix_extension_get_value(MixExtension *ext)
{
  assert(ext != NULL);
  return ext->value;
}

int mix_extension_get_max_value(MixExtension *ext)
{
  assert(ext != NULL);
  return ext->max_value;
}

int mix_extension_get_min_value(MixExtension *ext)
{
  assert(ext != NULL);
  return ext->min_value;
}
