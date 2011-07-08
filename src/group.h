#ifndef MIX_GROUP_H
#define MIX_GROUP_H

/**
 * A group contains multiple extensions of the same kind, that acts on
 * the same thing (ie. the on/off control for the volume and the
 * "potentiometer" associated with it)
 */
typedef struct {
  MixMixer *parent_mixer;
  char *name;
  List *extensions;
} MixGroup;


#endif /* MIX_GROUP_H */
