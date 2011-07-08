/**
 * @file extension.h
 * @brief Implements extensions
 */
#ifndef MIX_EXTENSION_H
#define MIX_EXTENSION_H

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

#endif /* MIX_EXTENSION_H */
