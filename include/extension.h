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
void mix_extension_free(MixExtension *extension);

/**
 * Update the value of an extension
 */
void mix_extension_update_value(MixExtension *extension);

#endif /* MIX_EXTENSION_H */
