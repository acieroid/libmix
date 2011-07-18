/**
 * @file mixer.h
 * @brief Implement mixer
 */
#ifndef MIX_MIXER_H
#define MIX_MIXER_H

#include "list.h"
#include "ossapi.h"
#include "datastructures.h"
/**
 * Allocate and fill a new mixer object
 */
MixMixer *mix_mixer_new(MixAPIFD fd);

/**
 * Free a mixer object
 */
void mix_mixer_free(MixMixer *mixer);

/**
 * @return the name of a mixer. The string returned is a pointer to the
 * 'name' field of the group, so it will be freed when mix_group_free
 * is called.
 */
char *mix_mixer_get_name(MixMixer *mixer);

/**
 * @return a list of all the groups included in that mixer
 */
MixList *mix_mixer_get_groups(MixMixer *mixer);

/**
 * @return a list of all the extensions included in that mixer
 */
MixList *mix_mixer_get_extensions(MixMixer *mixer);

/**
 * @return the card number of the mixer
 */
int mix_mixer_get_card_number(MixMixer *mixer);

/**
 * @return the file descriptor associated with this mixer
 */
MixAPIFD mix_mixer_get_fd(MixMixer *mixer);

/**
 * update all the extensions and groups of this mixer
 */
void mix_mixer_update(MixMixer *mixer);

/**
 * Function to call when all groups and extensions have been added. It
 * takes care of reversing them to have them in order.
 */
void mix_mixer_finish_add(MixMixer *mixer);

/**
 * Add a child group to this mixer
 */
void mix_mixer_add_child_group(MixMixer *mixer, MixGroup *group);

/**
 * Add a child extension to this mixer
 */
void mix_mixer_add_child_extension(MixMixer *mixer, MixExtension *ext);

/**
 * Set the parent mixer of this extension
 */
void mix_extension_set_parent_mixer(MixExtension *ext, MixMixer *mixer);

/**
 * Set the parent group of this extension
 */
void mix_extension_set_parent_group(MixExtension *ext, MixGroup *group);
  
/**
 * @return the extension that match the name given as argument, or
 * NULL
 */
MixExtension *mix_mixer_find_extension(MixMixer *mixer, const char *name);

#endif /* MIX_MIXER_H */
