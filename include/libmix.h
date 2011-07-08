/**
 * @file libmix.h
 * @brief Main include file of libmix
 */
#ifndef MIX_LIBMIX_H
#define MIX_LIBMIX_H

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
 * @return all the mixers available on the computer
 */
MixList *mix_get_mixers(MixerAPIFD fd);

#endif /* MIX_LIBMIX_H */
