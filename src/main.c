#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STR_SIZE 64

typedef void (*EachCb) (void *data, void *context);

typedef struct __BList BList_t;
typedef struct __BListItem BListItem_t;
typedef BListItem_t* BListItem;
typedef BList_t* BList;

typedef char* String;

struct __BListItem {
    BListItem next;
    BListItem prev;
    BList list;
    void *data;
};

struct __BList {
    BListItem first;
    BListItem last;
    EachCb freeCb;
};



BList blist_new(EachCb freeCb) {
    BList list = (BList) malloc(sizeof(BList_t));
    
    list->first = (void *) NULL;
    list->last = (void *) NULL;
    list->freeCb = freeCb;

    return list;
}

static BListItem blist_item_new(void *data) {
    BListItem item = (BListItem) malloc(sizeof(BListItem_t));

    item->next = (void *) NULL;
    item->prev = (void *) NULL;
    item->data = data;
    
    return item;
}

void blist_add_tail(BList list, void *data) {
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
}

void blist_add_head(BList list, void *data) {
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
}

void blist_each(BList list, EachCb cb, void *data) {
    BListItem current = list->first;
    
    while (current) {
	cb(current, data);
	current = current->next;
    }
}

static void inline blist_free_item(BListItem item, bool freeData) {
    if (freeData) {
	item->list->freeCb(item->data, (void *) NULL);
    }
    free(item);
}

void blist_free(BList list, bool freeData) {
    blist_each(list, (EachCb) blist_free_item, &freeData);
}

String allocStr(String src) {
    size_t strSize = strlen(src);
    String dst = (String) calloc(strSize + 1, sizeof(char));
    strncpy(dst, src, strSize);
    
    return dst;
}

void printStr(BListItem item, void *data) {
    printf("%s\n", (String) item->data);
}

int main(void) {
    printf("== Bidirectional linear list implementation. ==\n\n");
    
    BList blist = blist_new(NULL);

    String str1 = allocStr("str 1");
    String str2 = allocStr("str 2");
    String str3 = allocStr("str 3");
    
    blist_add_head(blist, str1);
    blist_add_tail(blist, str2);
    blist_add_head(blist, str3);
    
    blist_each(blist, (EachCb) printStr, NULL);
    
    exit(EXIT_SUCCESS);
}
