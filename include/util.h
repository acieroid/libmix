/**
 * @file util.h
 * @brief Various utilities typedef and functions
 */
#ifndef MIX_UTILS_H
#define MIX_UTILS_H

/**
 * Print a warning
 */
#define MIX_WARN(...)                                   \
  fprintf(stderr, "[libmix] Warning: " __VA_ARGS__);    \
  fprintf(stderr, "\n");

/**
 * Print a debug message
 */
#define MIX_DBG(...)                                    \
  fprintf(stderr, "[libmix] Debug: " __VA_ARGS__);      \
  fprintf(stderr, "\n");

/**
 * A function used to free data
 */
typedef void (*MixFreeFunc) (void *data);

/**
 * A function meant to be iterated over something
 */
typedef void (*MixIterFunc) (void *data);

/**
 * @return the length of the number passed as argument
 */
int mix_number_length(int n);

#endif /* MIX_UTILS_H */
