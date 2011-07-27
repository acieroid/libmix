/**
 * @file extension.h
 * @brief Implements extensions
 */
#ifndef MIX_EXTENSION_H
#define MIX_EXTENSION_H

#include "ossapi.h"
#include "datastructures.h"

/**
 * Type of an extension. Values are the same as OSS's MIXT_* defines
 */
typedef int MixExtensionType;

/**
 * Allocate and fill a new extension
 */
MixExtension *mix_extension_new(oss_mixext ext);

/**
 * Free an extension
 */
void mix_extension_free(MixExtension *ext);

/**
 * Update the value of an extension
 */
void mix_extension_update_value(MixExtension *ext);

/**
 * @return the name of this extension
 * @sa mix_mixer_get_name for details about the string returned
 */
char *mix_extension_get_name(MixExtension *ext);

/**
 * @return a pointer to the parent group of this extension
 */
MixGroup *mix_extension_get_group(MixExtension *ext);

/**
 * @return the color of this mixer. Mixer colors are supported since
 * OSS 4.1, if the version of OSS on this computer is older, the
 * default color will be returned.
 */
MixColor *mix_extension_get_color(MixExtension *ext);

/**
 * @return the current value of this extension
 */
int mix_extension_get_value(MixExtension *ext);

/**
 * @return the left value of this extension
 */
int mix_extension_get_left_value(MixExtension *ext);

/**
 * @return the right value of this extension
 */
int mix_extension_get_right_value(MixExtension *ext);

/**
 * @return 1 is the extension is muted and 0 if not
 */
int mix_extension_muted(MixExtension *ext);

/**
 * @return the maximum value of the mixer
 */
int mix_extension_get_max_value(MixExtension *ext);

/**
 * @return the minimum value of the mixer. See OSS's documentation for
 * more information about what this minimal value means.
 */
int mix_extension_get_min_value(MixExtension *ext);

/**
 * @return the type of this extension
 */
int mix_extension_get_type(MixExtension *ext);

/**
 * @return the file descriptor of this extension's device
 */
MixAPIFD mix_extension_get_fd(MixExtension *ext);

/**
 * @return the string corresponding to the current value of a
 * MIXT_ENUM extension
 */
char *mix_extension_get_enum_value(MixExtension *ext);

/**
 * @return the array of the enum values
 */
char **mix_extension_get_enum_values(MixExtension *ext);

/**
 * @return 1 if this extension is an enum (MIXT_ENUM)
 */
int mix_extension_is_enum(MixExtension *ext);

/**
 * @return 1 if this extension's values are stereo
 */
int mix_extension_is_stereo(MixExtension *ext);

/**
 * @return 1 if this extension is a mute or on/off extension
 */
int mix_extension_is_mute(MixExtension *ext);

/**
 * @return 1 if this extension is a slider
 */
int mix_extension_is_slider(MixExtension *ext);

/**
 * @return 1 if this extension is writeable
 */
int mix_extension_is_writeable(MixExtension *ext);

/**
 * @return 1 if the enum value @p val is available
 */
int mix_extension_enum_value_is_available(MixExtension *ext, int val);

/**
 * Set the value of this extension
 */
void mix_extension_set_value(MixExtension *ext, int value);

/**
 * Set the stereo value of this extension
 */
void mix_extension_set_stereo_value(MixExtension *ext, int left, int right);

/**
 * Set the muted value of this extension (1 = muted, 0 = unmuted)
 */
void mix_extension_set_muted(MixExtension *ext, int muted);

#endif /* MIX_EXTENSION_H */
