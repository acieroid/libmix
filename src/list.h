#ifndef MIX_LIST_H
#define MIX_LIST_H

#include <stdlib.h>
#include <assert.h>

#include "util.h"

/**
 * Simply linked list
 */
typedef struct MixList {
  void *data;                   /**< The data contained in this list node */
  struct MixList *next;         /**< The next list node */
} MixList;

/**
 * \return A pointer to the next element of the list
 */
MixList *mix_list_next(MixList *list);

/**
 * Prepend the list by a new allocated list node containing @data
 */
MixList *mix_list_prepend(MixList *list, void *data);

/**
 * Free all the element of a list by calling @freefunc on the data
 * field of each element of the list, and then calling free(3)
 */
void mix_list_free(MixList *list, MixFreeFunc freefunc);

#endif /* MIX_LIST_H */
