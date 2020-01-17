#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "include/blist.h"

#define NUMBERS_MAX 50

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

static void printNumber(BListItem item, void *data) {
  printf("%zu\n", (size_t) item->data);
}

BList initNumbersList(size_t nItems) {
  BList list = blist_new(NULL);

  for (size_t i = 0; i < nItems; i++) {
    blist_add_tail(list, (void *) i);
  }

  return list;
}

int main(void) {
  printf("== Bidirectional linear list implementation. ==\n\n");

  BList orig = initNumbersList(NUMBERS_MAX);
  BList dst = blist_new(NULL);


  BListItem currentForward = orig->first;
  BListItem currentBack = orig->last;
  size_t forwardCounter = 0;
  size_t backCounter = NUMBERS_MAX;

  while (currentForward || currentBack) {
    backCounter--;

    if (currentForward  && !(forwardCounter % 2)) {
      blist_add_tail(dst, currentForward->data);
    }

    if (currentBack && backCounter % 2) {
      blist_add_after(dst->last, currentBack->data);
    }

    currentForward = currentForward->next;
    currentBack = currentBack->prev;

    forwardCounter++;
  }

  blist_each(dst, (EachCb) printNumber, false, NULL);

  blist_free(orig);
  blist_free(dst);

  exit(EXIT_SUCCESS);
}
