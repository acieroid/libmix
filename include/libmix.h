/**
 * @file libmix.h
 * @brief Main include file of libmix
 */
#ifndef MIX_LIBMIX_H
#define MIX_LIBMIX_H

#include "mixer.h"
#include "group.h"
#include "extension.h"
#include "list.h"
#include "ossapi.h"

/**
 * Open the connection with OSS's API
 */
MixerAPIFD mix_open_dev(const char *dev);

/**
 * Close the connection with OSS's API
 */
void mix_close_dev(MixerAPIFD fd);

/**
 * @return the number of available mixers
 */
int mix_get_number_of_mixers(MixerAPIFD fd);

/**
 * @return the nth mixer (the first mixer is numbered 0)
 */
MixMixer *mix_get_mixer(MixerAPIFD fd, int n);

/**
 * @return all the mixers available on the computer
 */
MixList *mix_get_mixers(MixerAPIFD fd);

#endif /* MIX_LIBMIX_H */
