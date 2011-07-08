#include "list.h"

MixList *mix_list_next(MixList *list)
{
  assert(list != NULL);
  return list->next;
}

MixList *mix_list_prepend(MixList *list, void *data)
{
  MixList *new_list = malloc(sizeof(new_list));;
  assert(new_list != NULL);

  new_list->data = data;
  new_list->next = list;

  return new_list;
}

void mix_list_free(MixList *list, MixFreeFunc freefunc)
{
  MixList *prev, *cur;
  assert(list != NULL);

  for (prev = list, cur = list->next; cur != NULL;
       prev = cur, cur = cur->next) {
    freefunc(prev->data);
    prev->data = NULL;
    prev->next = NULL;
    free(prev);
  }

  freefunc(cur->data);
  cur->data = NULL;
  free(cur);
}
