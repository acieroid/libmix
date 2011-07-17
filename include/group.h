/**
 * @file group.h
 * @brief Implements extension groups
 */
#ifndef MIX_GROUP_H
#define MIX_GROUP_H

#include "datastructures.h"
#include "ossapi.h"

/**
 * Allocate and fill a new group
 */
MixGroup *mix_group_new(oss_mixext ext);

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

/**
 * @sa mix_mixer_finish_add
 */
void mix_group_finish_add(MixGroup *group);

/**
 * Add a child group to this group
 * @sa mix_mixer_add_child_group
 */
void mix_group_add_child_group(MixGroup *parent, MixGroup *group);

/**
 * Add a child extension to this group
 * @sa mix_mixer_add_child_extension
 */
void mix_group_add_child_extension(MixGroup *group, MixExtension *ext);

/**
 * Set the parent mixer of this group
 */
void mix_group_set_parent_mixer(MixGroup *group, MixMixer *mixer);

/**
 * Set the parent group of this group
 */
void mix_group_set_parent_group(MixGroup *group, MixGroup *parent);

#endif /* MIX_GROUP_H */
