#include <stdlib.h>
#include <stdio.h>
#include "include/blist.h"

BList blist_new(EachCb freeCb) {
  BList list = (BList) malloc(sizeof(BList_t));

  list->first = (void *) NULL;
  list->last = (void *) NULL;
  list->freeCb = freeCb;

  return list;
}

BListItem blist_item_new(void *data) {
  BListItem item = (BListItem) malloc(sizeof(BListItem_t));

  item->next = (void *) NULL;
  item->prev = (void *) NULL;
  item->data = data;

  return item;
}

BListItem blist_add_tail(BList list, void *data) {
  BListItem item = blist_item_new(data);

  item->list = list;

  if (list->last) {
    item->prev = list->last;
    list->last->next = item;
  }

  list->last = item;

  if (!list->first) {
    list->first = item;
    list->first->next = item;
  }

  return item;
}

BListItem blist_add_head(BList list, void *data) {
  BListItem item = blist_item_new(data);

  item->list = list;

  if (list->first) {
    item->next = list->first;
    list->first->prev = item;
  }

  list->first = item;

  if (!list->last) {
    list->last = item;
    list->last->prev = item;
  }

  return item;
}

BListItem blist_add_after(BListItem item, void *data) {
  BListItem newItem = blist_item_new(data);

  if (item->next) {
    newItem->next = item->next;
    item->next->prev = newItem;
  }

  if (item->list->last == item) {
    item->list->last = newItem;
  }

  item->next = newItem;
  newItem->prev = item;

  return newItem;
}

BListItem blist_remove(BListItem item) {
  if (item->prev) {
    if (item->next) {
      item->prev->next = item->next;
    }
    else {
      item->prev->next = NULL;
    }
  }

  if (item->next) {
    if (item->prev) {
      item->next->prev = item->prev;
    }
    else {
      item->next->prev = NULL;
    }
  }

  if (item->list->last == item) {
    if (item->prev) {
      item->list->last = item->prev;
    }
    else {
      item->list->last = NULL;
    }
  }

  if (item->list->first == item) {
    if (item->next) {
      item->list->first = item->next;
    }
    else {
      item->list->first = NULL;
    }
  }

  return item;
}

void blist_each(BList list, EachCb cb, bool reverse, void *data) {
  BListItem current = list->first;

  while (current) {
    BListItem target = current;

    if (reverse) {
      current = current->prev;
    }
    else {
      current = current->next;
    }

    cb(target, data);
  }
}

void blist_free(BList list) {
  blist_each(list, (EachCb) blist_free_item, false, NULL);
  printf("\t==> Free memory for list at %p\n", (void *) list);
  free(list);
}
