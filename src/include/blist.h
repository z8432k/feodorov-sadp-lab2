#include <stdlib.h>
#include <stdio.h>

typedef struct _BList BList_t;
typedef struct _BListItem BListItem_t;
typedef BListItem_t* BListItem;
typedef BList_t* BList;

typedef void (*EachCb) (void *data, void *context);

struct _BListItem {
  BListItem next;
  BListItem prev;
  BList list;
  void *data;
};

struct _BList {
  BListItem first;
  BListItem last;
  EachCb freeCb;
};


BList blist_new(EachCb freeCb);

BListItem blist_item_new(void *data);

BListItem blist_add_tail(BList list, void *data);

BListItem blist_add_head(BList list, void *data);

BListItem blist_add_after(BListItem item, void *data);

BListItem blist_remove(BListItem item);

void blist_each(BList list, EachCb cb, void *data);

inline static void blist_free_item(BListItem item, void *arg) {
  if (item->list->freeCb) {
    item->list->freeCb(item->data, (void *) NULL);
  }
  printf("\t==> Free memory for list item at %p\n", (void *) item);
  free(item);
}

void blist_free(BList list);
