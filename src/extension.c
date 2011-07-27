#include <string.h>

#include "extension.h"

MixExtension *mix_extension_new(oss_mixext ext)
{
  MixExtension *extension = malloc(sizeof(*extension));
  MIX_DBG("Creating extension %s", ext.extname);
  assert(extension != NULL);

  extension->parent_group = NULL;
  extension->parent_mixer = NULL;
  #ifdef OSS_RGB_RED
  extension->color = mix_color_new_from_24bit(ext.rgbcolor);
  #else
  extension->color = mix_color_new_from_24bit(0);
  #endif
  extension->mixext = ext;
  extension->enum_values = NULL;
  extension->enum_values_available = NULL;
  extension->value = 0;
  return extension;
}

void mix_extension_free(MixExtension *ext)
{
  int i;
  assert(ext != NULL);
  MIX_DBG("Freeing extension %s", mix_extension_get_name(ext));

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
    free(ext->enum_values_available);

  free(ext);
}

void mix_extension_update_value(MixExtension *ext)
{
  int i, mask;
  oss_mixer_value val;
  oss_mixer_enuminfo enuminfo;

  MIX_DBG("Extension %s: updating value",
          mix_extension_get_name(ext));

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
  case MIXT_MONOPEAK:
    OSS_CALL(mix_extension_get_fd(ext), SNDCTL_MIX_READ, &val);
    /* "The 8 least significant bits of the value field are used for
       the volume level (0xVV)" (OSS documentation) */
    ext->value = val.value & 0xFF;
    break;
  case MIXT_MONOSLIDER16:
    /* "The actual value is stored in the lowest 16 bits of the value
       field (0xVVVV)" (OSS documentation) */
  case MIXT_STEREOSLIDER:
    /* "The left channel level is stored in the lowest 8 bits of the
       value field and the right channel level is stored in the next 8
       bit byte (0xRRLL)." (OSS documentation) */
  case MIXT_STEREOPEAK:
    OSS_CALL(mix_extension_get_fd(ext), SNDCTL_MIX_READ, &val);
    ext->value = val.value & 0xFFFF; /* In both case we want these four bytes */
    break;
  case MIXT_SLIDER:
  case MIXT_VALUE:
  case MIXT_STEREOSLIDER16:
    OSS_CALL(mix_extension_get_fd(ext), SNDCTL_MIX_READ, &val);
    ext->value = val.value;
    break;
  default:
    MIX_WARN("Unknown or not yet handled extension type: %d for extension %s",
             mix_extension_get_type(ext), mix_extension_get_name(ext));
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

MixMixer *mix_extension_get_mixer(MixExtension *ext)
{
  assert(ext != NULL);
  return ext->parent_mixer;
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

int mix_extension_get_left_value(MixExtension *ext)
{
  int mask;
  switch (mix_extension_get_type(ext)) {
  case MIXT_STEREOSLIDER: /* 0xRRLL */
    mask = 0x00FF;
    break;
  case MIXT_STEREOSLIDER16: /* 0xRRRRLLLL */
    mask = 0x0000FFFF;
    break;
  default:
    MIX_WARN("Extension %s isn't stereo", mix_extension_get_name(ext));
    mask = ~0; /* We simply return its raw value */
    break;
  }
  return mix_extension_get_value(ext) & mask;
}

int mix_extension_get_right_value(MixExtension *ext)
{
  int mask, shift;
  switch (mix_extension_get_type(ext)) {
  case MIXT_STEREOSLIDER: /* 0xRRLL */
    mask = 0xFF00;
    shift = 8;
    break;
  case MIXT_STEREOSLIDER16: /* 0xRRRRLLLL */
    mask = 0xFFFF0000;
    shift = 16;
    break;
  default:
    MIX_WARN("Extension %s isn't stereo", mix_extension_get_name(ext));
    mask = ~0; /* We simply return its raw value */
    shift = 0;
  }
  return (mix_extension_get_value(ext) & mask) >> shift;
}

int mix_extension_muted(MixExtension *ext)
{
  return mix_extension_get_value(ext);
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
  MixMixer *parent_mixer;
  MixGroup *parent_group;
  parent_mixer = mix_extension_get_mixer(ext);
  parent_group  = mix_extension_get_group(ext);

  assert(parent_group != NULL || parent_mixer != NULL);
  if (parent_mixer != NULL)
    return mix_mixer_get_fd(parent_mixer);
  else
    return mix_group_get_fd(parent_group);
}

char *mix_extension_get_enum_value(MixExtension *ext)
{
  int val;
  assert(ext != NULL);
  assert(mix_extension_get_type(ext) == MIXT_ENUM);
  val = mix_extension_get_value(ext);
  if (val > mix_extension_get_max_value(ext))
    val = mix_extension_get_max_value(ext)-1;
  return ext->enum_values[val];
}

char **mix_extension_get_enum_values(MixExtension *ext)
{
  assert(ext != NULL);
  assert(mix_extension_get_type(ext) == MIXT_ENUM);
  return ext->enum_values;
}

void mix_extension_set_parent_mixer(MixExtension *ext, MixMixer *mixer)
{
  assert(ext != NULL);
  MIX_DBG("Extension %s: setting parent mixer %s",
          mix_extension_get_name(ext), mix_mixer_get_name(mixer));

  ext->parent_mixer = mixer;
}

void mix_extension_set_parent_group(MixExtension *ext, MixGroup *group)
{
  assert(ext != NULL);
  MIX_DBG("Extension %s: setting parent group %s",
          mix_extension_get_name(ext), mix_group_get_name(group));

  ext->parent_group = group;
}

int mix_extension_is_enum(MixExtension *ext)
{
  return mix_extension_get_type(ext) == MIXT_ENUM;
}

int mix_extension_is_stereo(MixExtension *ext)
{
  switch (mix_extension_get_type(ext)) {
  case MIXT_STEREOSLIDER:
  case MIXT_STEREOSLIDER16:
    return 1;
  default:
    return 0;
  }
}

int mix_extension_is_mute(MixExtension *ext)
{
  switch (mix_extension_get_type(ext)) {
  case MIXT_MUTE:
  case MIXT_ONOFF:
    return 1;
  default:
    return 0;
  }
}

int mix_extension_is_slider(MixExtension *ext)
{
  switch (mix_extension_get_type(ext)) {
  case MIXT_STEREOSLIDER16:
  case MIXT_SLIDER:
  case MIXT_MONOSLIDER16:
  case MIXT_STEREOSLIDER:
  case MIXT_MONOSLIDER:
    return 1;
  default:
    return 0;
  }
}

int mix_extension_is_writeable(MixExtension *ext)
{
  assert(ext != NULL);
  return ext->mixext.flags & MIXF_WRITEABLE;
}

int mix_extension_enum_value_is_available(MixExtension *ext, int val)
{
  assert(ext != NULL);
  if (val < mix_extension_get_max_value(ext))
    return ext->enum_values_available[val];
  else
    return 0;
}

void mix_extension_set_value(MixExtension *ext, int value)
{
  oss_mixer_value val;
  assert(ext != NULL);
  MIX_DBG("Setting the value of extension %s to %d",
          mix_extension_get_name(ext), value);

  if (!mix_extension_is_writeable(ext)) {
    MIX_WARN("Trying to modify a read-only extension: %s",
             mix_extension_get_name(ext));
    return;
  }
  
  val.dev = ext->mixext.dev;
  val.ctrl = ext->mixext.ctrl;
  val.timestamp = ext->mixext.timestamp;
  if (value < mix_extension_get_max_value(ext))
    val.value = value;
  else
    val.value = mix_extension_get_max_value(ext)-1;
  OSS_CALL(mix_extension_get_fd(ext), SNDCTL_MIX_WRITE, &val);

  /* we now have to update the value since it was changed */
  mix_extension_update_value(ext);
}

void mix_extension_set_stereo_value(MixExtension *ext, int left, int right)
{
  int shift;
  switch (mix_extension_get_type(ext)) {
  case MIXT_STEREOSLIDER16:
    shift = 16;
    break;
  default:
    shift = 8;
    break;
  }
  mix_extension_set_value(ext, left | (right << shift));
}

void mix_extension_set_muted(MixExtension *ext, int muted)
{
  assert(muted == 0 || muted == 1);
  mix_extension_set_value(ext, muted);
}
