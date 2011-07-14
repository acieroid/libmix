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
  MixList *extensions;          /**< Extensions contained in this group */
  oss_mixext mixext;            /**< OSS internal structure for this group */
} MixGroup;

/**
 * Allocate and fill a new group
 */
MixGroup *mix_group_new(MixMixer *parent, oss_mixext ext);

/**
 * Free a group
 */
void mix_group_free(MixGroup *group);

/**
 * @return the group name.
 * @sa mix_mixer_get_name for details about the returned string
 */
char *mix_group_get_name(MixGroup *group);

/**
 * @return the list of extensions contained by this group
 */
MixList *mix_group_get_extensions(MixGroup *group);

/**
 * @return a pointer to the parent mixer
 */
MixMixer *mix_group_get_mixer(MixGroup *group);

/**
 * @return the file descriptor associated with this group
 */
MixAPIFD mix_group_get_fd(MixGroup *group);

#endif /* MIX_GROUP_H */
