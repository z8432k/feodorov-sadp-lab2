#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "include/blist.h"

typedef char *String;


static String allocStr(String src) {
  size_t strSize = strlen(src);
  String dst = (String) calloc(strSize + 1, sizeof(char));
  strncpy(dst, src, strSize);

  return dst;
}

static void freeStr(void * str, void *data) {
  printf("\t==> Free memory for string at %p\n", (void *) str);
  free(str);
}

static void printStr(BListItem item, void *data) {
  printf("%s\n", (String) item->data);
}

int main(void) {
  printf("== Bidirectional linear list implementation. ==\n\n");

  BList blist = blist_new(freeStr);

  String str1 = allocStr("str 1");
  String str2 = allocStr("str 2");
  String str3 = allocStr("str 3");

  BListItem i1 = blist_add_head(blist, str1);
  BListItem i2 = blist_add_tail(blist, str2);
  BListItem i3 = blist_add_head(blist, str3);

  blist_each(blist, (EachCb) printStr, NULL);

  blist_remove(i3);
  blist_free_item(i3, NULL);
  blist_each(blist, (EachCb) printStr, NULL);

  blist_remove(i2);
  blist_free_item(i2, NULL);
  blist_each(blist, (EachCb) printStr, NULL);

  blist_remove(i1);
  blist_free_item(i1, NULL);
  blist_each(blist, (EachCb) printStr, NULL);

  blist_free(blist);

  exit(EXIT_SUCCESS);
}
