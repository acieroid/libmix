/**
 * @file group.h
 * @brief Implements extension groups
 */
#ifndef MIX_GROUP_H
#define MIX_GROUP_H

#include "mixer.h"
#include "ossapi.h"

/**
 * A group contains multiple extensions of the same kind, that acts on
 * the same thing (ie. the on/off control for the volume and the
 * "potentiometer" associated with it)
 */
typedef struct {
  MixMixer *parent_mixer;       /**< The mixer that contains this group */
  char *name;                   /**< The name of this group */
  MixList *extensions;          /**< Extensions contained in this group */
} MixGroup;

/**
 * Allocate and fill a new group
 */
MixGroup *mix_group_new(MixMixer *parent, oss_mixext ext);

/**
 * Free a group
 */
void mix_group_free(MixGroup *group);

#endif /* MIX_GROUP_H */
