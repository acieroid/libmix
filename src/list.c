#include <assert.h>

#include "list.h"

MixList *mix_list_next(MixList *list)
{
  assert(list != NULL);
  return list->next;
}

MixList *mix_list_prepend(MixList *list, void *data)
{
  MixList *new_list = malloc(sizeof(*new_list));;
  assert(new_list != NULL);

  new_list->data = data;
  new_list->next = list;

  return new_list;
}

MixList *mix_list_reverse(MixList *list)
{
  MixList *new_list, *cur, *next;

  if (list == NULL)
    return list;

  new_list = NULL;
  for (cur = list; cur != NULL; cur = next) {
    next = cur->next;
    cur->next = new_list;
    new_list = cur;
  }
  return new_list;
}

void mix_list_free(MixList *list, MixFreeFunc freefunc)
{
  MixList *prev, *cur;
  if (list == NULL)
    return;

  for (prev = list, cur = list->next; cur != NULL;
       prev = cur, cur = cur->next) {
    freefunc(prev->data);
    prev->data = NULL;
    prev->next = NULL;
    free(prev);
  }

  freefunc(prev->data);
  prev->data = NULL;
  free(prev);
}

void mix_list_iter(MixList *list, MixIterFunc iterfunc)
{
  MixList *iterator;
  mix_foreach(iterator, list) {
    iterfunc(iterator->data);
  }
}

void *mix_list_select_left(MixList *list, void *selected)
{
  MixList *iterator;
  void *prev;
  mix_foreach(iterator, list) {
    if (iterator->data == selected) {
      if (prev == NULL)
        return selected; /* already at the left-most element */
      else
        return prev;
    }
    prev = iterator->data;
  }
  MIX_WARN("No elements found when selecting left");
  return NULL;
}

void *mix_list_select_right(MixList *list, void *selected)
{
  MixList *iterator;
  mix_foreach(iterator, list) {
    if (iterator->data == selected) {
      if (iterator->next == NULL)
        return selected; /* already at the right-most element */
      else
        return iterator->next->data;
    }
  }
  MIX_WARN("No elements found when selecting right");
  return NULL;
}
