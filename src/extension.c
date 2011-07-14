#include <string.h>

#include "extension.h"

MixExtension *mix_extension_new(MixGroup *parent, oss_mixext ext)
{
  MixExtension *extension = malloc(sizeof(*extension));
  assert(extension != NULL);
  
  extension->parent_group = parent;
  #ifdef OSS_RGB_RED
  extension->color = mix_color_new_from_24bit(ext.rgbcolor);
  #endif
  extension->mixext = ext;
  mix_extension_update_value(extension);
  return extension;
}

void mix_extension_free(MixExtension *ext)
{
  assert(ext != NULL);
  if (ext->color != NULL)
    mix_color_free(ext->color);
  free(ext);
}

void mix_extension_update_value(MixExtension *ext)
{
  int i;
  oss_mixer_value val;
  oss_mixer_enuminfo enuminfo;
  /* The dev, ctrl and timestamp fields must be initialized to the
     values returned by SNDCTL_MIX_EXTINFO before making the
     call. (OSS documentation) */
  val.dev = ext->mixext.dev;
  val.ctrl = ext->mixext.ctrl;
  val.timestamp = ext->mixext.timestamp;

  switch (mix_extension_get_type(ext)) {
  case MIXT_DEVROOT:
  case MIXT_GROUP:
  case MIXT_MARKER:
    /* already handled in mix_get_mixer */
    break;
#if 0
  case MIXT_ENUM:
    ext->enum_values = malloc(mix_extension_get_max_value(ext)*sizeof(char *));
    enuminfo.dev = ext->extinfo->dev;
    enuminfo.ctrl = ext->extinfo->ctrl;
    /* "It is possible that some enum controls don't have any name
       list available. In this case the application should
       automatically generate list of numbers (0 to N-1)." (from OSS
       documentation) */
    if (ioctl(mix_extension_get_fd(ext), SNDCTL_MIX_ENUMINFO, &enuminfo) != -1) {
      for (i = 0; i < mix_extension_get_max_value(ext); i++) {
        /* See http://manuals.opensound.com/developer/SNDCTL_MIX_EXTINFO.html */
        if (ext->extinfo->enum_present[i/8] & (1 << (i%8))) {
          ext->enum_values[i] = malloc(strlen(enuminfo.strings +
                                              enuminfo.strindex[i]) *
                                       sizeof(char));
          strcpy(ext->enum_values[i], enuminfo.strings + enuminfo.strindex[i]);
        }
      }
    }
    else {
      for (i = 0; i < mix_extension_get_max_value(ext); i++) {
        ext->enum_values[i] = malloc(mix_number_length(i)*sizeof(char));
        sprintf(ext->enum_values[i], "%d", i);
      }
    }
    /* no break because we also want the current value */
#endif
  case MIXT_ONOFF: /* 0 -> ON, 1 -> OFF */
  case MIXT_MUTE:  /* 0 -> Muted, 1 -> Not muted */
    OSS_CALL(mix_extension_get_fd(ext), SNDCTL_MIX_READ, &val);
    ext->value = val.value;
    break;
  default:
    MIX_WARN("Unknown or not yet handled extension type: %d\n",
             mix_extension_get_type(ext));
    break;
  }
}

char *mix_extension_get_name(MixExtension *ext)
{
  assert(ext != NULL);
  return ext->mixext.extname;
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
  return ext->mixext.maxvalue;
}

int mix_extension_get_min_value(MixExtension *ext)
{
  assert(ext != NULL);
  return ext->mixext.minvalue;
}

int mix_extension_get_type(MixExtension *ext)
{
  assert(ext != NULL);
  return ext->mixext.type;
}

MixAPIFD mix_extension_get_fd(MixExtension *ext)
{
  return mix_group_get_fd(mix_extension_get_group(ext));
}

char *mix_extension_get_enum_value(MixExtension *ext)
{
  assert(ext != NULL);
  assert(mix_extension_get_type(ext) == MIXT_ENUM);
  return ext->enum_values[ext->value];
}
