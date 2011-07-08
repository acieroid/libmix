#ifndef MIX_EXTENSION_H
#define MIX_EXTENSION_H

/**
 * An extension correspond to some "movable" or "chosable" control,
 * ie. a on/off control, a "potentiometer" control, etc.
 */
typedef struct {
  MixGroup *parent_group;
  char *name;
  MixExtensionType type;
  MixColor *color;
  int value;
  int max_value;
  int min_value;
} MixExtension;

#endif /* MIX_EXTENSION_H */
