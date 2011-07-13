/**
 * @file extension.h
 * @brief Implements extensions
 */
#ifndef MIX_EXTENSION_H
#define MIX_EXTENSION_H

#include "group.h"
#include "color.h"
#include "ossapi.h"

/**
 * Type of an extension. Values are the same as OSS's MIXT_* defines
 */
typedef int MixExtensionType;

/**
 * An extension correspond to some "movable" or "chosable" control,
 * ie. a on/off control, a "potentiometer" control, etc.
 */
typedef struct {
  MixGroup *parent_group;       /**< The group that contains this extension */
  char *name;                   /**< The name of this extension */
  MixExtensionType type;        /**< The type of this extension */
  MixColor *color;              /**< The color of this extension */
  int value;                    /**< The current value */
  int max_value;                /**< The maximum possible value */
  int min_value;                /**< The minimum possible value */
} MixExtension;

/**
 * Allocate and fill a new extension
 */
MixExtension *mix_extension_new(MixGroup *parent, oss_mixext ext);

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
 * @return the current value of the mixer
 */
int mix_extension_get_value(MixExtension *ext);

/**
 * @return the maximum value of the mixer
 */
int mix_extension_get_max_value(MixExtension *ext);

/**
 * @return the minimum value of the mixer. See OSS's documentation for
 * more information about what this minimal value means.
 */
int mix_extension_get_min_value(MixExtension *ext);

#endif /* MIX_EXTENSION_H */
