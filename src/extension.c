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
  int i;
  assert(ext != NULL);

  if (ext->color != NULL)
    mix_color_free(ext->color);
  if (ext->enum_values != NULL) {
    for (i = 0; i < mix_extension_get_max_value(ext); i++) {
      if (ext->enum_values[i] != NULL)
        free(ext->enum_values[i]);
    }
    free(ext->enum_values);
  }
  if (ext->enum_values_available != NULL)
    free(ext->enum_values);

  free(ext);
}

void mix_extension_update_value(MixExtension *ext)
{
  int i, mask;
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
  case MIXT_ENUM:
    ext->enum_values = malloc(mix_extension_get_max_value(ext)*sizeof(char *));
    ext->enum_values_available =
      malloc(mix_extension_get_max_value(ext)*sizeof(int));
    enuminfo.dev = ext->mixext.dev;
    enuminfo.ctrl = ext->mixext.ctrl;
    /* "It is possible that some enum controls don't have any name
       list available. In this case the application should
       automatically generate list of numbers (0 to N-1)." (OSS
       documentation) */
    if (ioctl(mix_extension_get_fd(ext), SNDCTL_MIX_ENUMINFO, &enuminfo) != -1) {
      for (i = 0; i < mix_extension_get_max_value(ext); i++) {
        ext->enum_values[i] = malloc((strlen(enuminfo.strings +
                                             enuminfo.strindex[i])+1) *
                                     sizeof(char));
        strcpy(ext->enum_values[i], enuminfo.strings + enuminfo.strindex[i]);
        /* See http://manuals.opensound.com/developer/SNDCTL_MIX_EXTINFO.html */
        ext->enum_values_available[i] = ext->mixext.enum_present[i/8] & (1 << (i%8));
      }
    }
    else {
      for (i = 0; i < mix_extension_get_max_value(ext); i++) {
        ext->enum_values[i] = malloc(mix_number_length(i)*sizeof(char));
        sprintf(ext->enum_values[i], "%d", i);
        ext->enum_values_available[i] = ext->mixext.enum_present[i/8] & (1 << (i%8));
      }
    }
    /* no break because we also want the current value */
  case MIXT_ONOFF: /* 0 -> ON, 1 -> OFF */
  case MIXT_MUTE:  /* 0 -> Muted, 1 -> Not muted */
    OSS_CALL(mix_extension_get_fd(ext), SNDCTL_MIX_READ, &val);
    ext->value = val.value;
    break;
  case MIXT_MONODB:
  /* "This mixer control type is not used by OSS drivers any
     more. Mixer applications can handle it just like
     MIXT_MONOSLIDER" (OSS documentation) */
  case MIXT_MONOSLIDER:
    OSS_CALL(mix_extension_get_fd(ext), SNDCTL_MIX_READ, &val);
    /* "The 8 least significant bits of the value field are used for
       the volume level (0xVV)" (OSS documentation) */
    ext->value = val.value & 0xFF;
    break;
  case MIXT_MONOSLIDER16:
    OSS_CALL(mix_extension_get_fd(ext), SNDCTL_MIX_READ, &val);
    /* "The actual value is stored in the lowest 16 bits of the value
       field (0xVVVV)" (OSS documentation) */
    ext->value = val.value & 0xFFFF;
    break;
  case MIXT_SLIDER:
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
  //  if (mix_extension_get_value() > mix_extension_get_max_value())
  return ext->enum_values[ext->value];
}
