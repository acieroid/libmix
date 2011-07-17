#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "list.h"
#include "ossapi.h"

/**
 * Simply a RGB color
 */
typedef struct {
  unsigned char red;            /**< The red value of the color */
  unsigned char green;          /**< The green value */
  unsigned char blue;           /**< The blue value */
} MixColor;

/**
 * A mixer correspond to a set of control of the same sound cards
 * (usually a sound card have one mixer)
 */
typedef struct {
  MixAPIFD fd;                  /**< File descriptor for communication with OSS */
  MixList *groups;              /**< Groups contained in this mixer */
  MixList *extensions;          /**< Extensions contained in this mixer */
  oss_mixerinfo mixerinfo;      /**< OSS internal structure for this mixer */
} MixMixer;

/**
 * An extension correspond to some "movable" or "chosable" control,
 * ie. a on/off control, a "potentiometer" control, etc.
 */
typedef struct {
  MixMixer *parent_mixer;        /**< The mixer that contains this extension */
  struct MixGroup *parent_group; /**< The group that contains this extension */
  MixColor *color;               /**< The color of this extension */
  int value;                     /**< The current integer value */
  char **enum_values;            /**< Possible values for a MIXT_ENUM */
  int *enum_values_available;    /**< Which values in enum_values are available now */
  oss_mixext mixext;             /**< OSS internal structure for this extension */
} MixExtension;

/**
 * A group contains multiple extensions of the same kind, that acts on
 * the same thing (ie. the on/off control for the volume and the
 * "potentiometer" associated with it)
 */
typedef struct MixGroup {
  MixMixer *parent_mixer;        /**< The mixer that contains this group */
  struct MixGroup *parent_group; /**< The group that contains this group (if there is) */
  MixList *groups;               /**< Groups contained in this group */
  MixList *extensions;           /**< Extensions contained in this group */
  oss_mixext mixext;             /**< OSS internal structure for this group */
} MixGroup;


#endif /* DATASTRUCTURES_H */
