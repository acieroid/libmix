/**
 * @file libmix.h
 * @brief Main include file of libmix
 */
#ifndef MIX_LIBMIX_H
#define MIX_LIBMIX_H

#include "list.h"

/**
 * @return all the mixers available on the computer
 */
List *mix_get_mixers();

#endif /* MIX_LIBMIX_H */
