/**
 * @file color.h
 * @brief Implements colors
 */
#ifndef MIX_COLOR_H
#define MIX_COLOR_H

#include <stdlib.h>

/**
 * Simply a RGB color
 */
typedef struct {
  unsigned char red;            /**< The red value of the color */
  unsigned char green;          /**< The green value */
  unsigned char blue;           /**< The blue value */
} MixColor;

/**
 * Allocate data for a new color and fill its members
 */
MixColor *mix_color_new(unsigned char red,
                        unsigned char green,
                        unsigned char blue);

/**
 * Free memory used by @p color using @p free(3)
 */
void mix_color_delete(MixColor *color);

/**
 * Create a color (using @p mix_color_new) from a 24 bit color
 * representation
 */
MixColor *mix_color_new_from_24bit(unsigned int color);

#endif /* MIX_COLOR_H */
