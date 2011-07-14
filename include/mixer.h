/**
 * @file mixer.h
 * @brief Implement mixer
 */
#ifndef MIX_MIXER_H
#define MIX_MIXER_H

#include "list.h"
#include "ossapi.h"

/**
 * A mixer correspond to a set of control of the same sound cards
 * (usually a sound card have one mixer)
 */
typedef struct {
  MixAPIFD fd;                /**< File descriptor for communication with OSS */
  MixList *groups;              /**< Groups contained in this mixer */
  oss_mixerinfo mixerinfo;      /**< OSS internal structure for this mixer */
} MixMixer;

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
 * @return the card number of the mixer
 */
int mix_mixer_get_card_number(MixMixer *mixer);

/**
 * @return the file descriptor associated with this mixer
 */
MixAPIFD mix_mixer_get_fd(MixMixer *mixer);

#endif /* MIX_MIXER_H */
