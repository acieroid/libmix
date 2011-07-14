/**
 * @file util.h
 * @brief Various utilities typedef and functions
 */
#ifndef MIX_UTILS_H
#define MIX_UTILS_H

#define MIX_WARN(...)                                   \
  fprintf(stderr, "[libmix] Warning: " __VA_ARGS__)

/**
 * Type representing a function used to free data
 */
typedef void (*MixFreeFunc) (void *data);

/**
 * @return the length of the number passed as argument
 */
int mix_number_length(int n);

#endif /* MIX_UTILS_H */
