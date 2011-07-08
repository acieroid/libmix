#include <assert.h>

#include "color.h"

MixColor *mix_color_new(unsigned char red,
                        unsigned char green,
                        unsigned char blue)
{
  MixColor *color = malloc(sizeof(color));
  assert(color != NULL);

  color->red = red;
  color->green = green;
  color->blue = blue;
  return color;
}

void mix_color_delete(MixColor *color)
{
  assert(color != NULL);
  free(color);
}

MixColor *mix_color_new_from_24bit(unsigned int color)
{
  return mix_color_new(color & 0xFF,
                       color & 0xFF00 >> 2,
                       color & 0xFF0000 >> 4);
}
