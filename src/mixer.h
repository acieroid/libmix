#ifndef MIX_MIXER_H
#define MIX_MIXER_H

#include "list.h"
#include "ossapi.h"

/**
 * A mixer correspond to a set of control of the same sound cards
 * (usually a sound card have one mixer)
 */
typedef struct {
  char *name;
  List *groups;
  int card_number;
} MixMixer;

/**
 * \return all the mixers available on the computer
 * \todo move that in another file
 */
List *mix_get_mixers();

/**
 * \return the name of a mixer
 */
char *mix_mixer_get_name(MixMixer *mixer);

/**
 * \return a list of all the groups included in that mixer
 */
List *mix_mixer_get_groups(MixMixer *mixer);

/**
 * \return The card number of the mixer
 */
int mix_mixer_get_card_number(MixMixer *mixer);

#endif /* MIX_MIXER_H */
