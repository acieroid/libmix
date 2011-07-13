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
  MixerAPIFD fd;        /**< File descriptor for communication with OSS */
  char *name;           /**< The name of this mixer */
  MixList *groups;      /**< Groups contained in this mixer */
  int card_number;      /**< Number of the sound card of this mixer */
} MixMixer;

/**
 * @return the name of a mixer
 */
char *mix_mixer_get_name(MixMixer *mixer);

/**
 * @return a list of all the groups included in that mixer
 */
MixList *mix_mixer_get_groups(MixMixer *mixer);

/**
 * @return The card number of the mixer
 */
int mix_mixer_get_card_number(MixMixer *mixer);

#endif /* MIX_MIXER_H */
