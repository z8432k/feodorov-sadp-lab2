#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <math.h>
#include "include/blist.h"

#define NUMBERS_MAX 50

typedef char *String;

static String allocStr(String src) {
  size_t strSize = strlen(src);
  String dst = (String) calloc(strSize + 1, sizeof(char));
  strncpy(dst, src, strSize);

  return dst;
}

static void freeData(void * data, void *d) {
  printf("\t==> Free memory for data at %p\n", (void *) data);
  free(data);
}

static void printStr(BListItem item, void *data) {
  printf("%s\n", (String) item->data);
}

static void printNumber(BListItem item, void *data) {
  printf("%zu\n", (size_t) item->data);
}

static void printDouble(BListItem item, void *data) {
  printf("%f\n",  *((double *) item->data));
}

BList initNumbersList(size_t nItems) {
  BList list = blist_new(NULL);

  for (size_t i = 0; i < nItems; i++) {
    blist_add_tail(list, (void *) i);
  }

  return list;
}

static void fillList(BList src, BList dst) {
  BListItem currentForward = src->first;
  BListItem currentBack = src->last;
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
}

static ssize_t readNumber(double *element) {
  char *line = NULL;
  ssize_t lineSize;
  size_t bufLen = 0;

  lineSize = getline(&line, &bufLen, stdin);
  *element = strtod(line, NULL);

  return lineSize;
}

BList inputList() {
  BList list = blist_new(freeData);

  double number;

  printf("Input numbers one by one. Press enter after each. Press enter for finish: \n");

  do {
    ssize_t lineSize = readNumber(&number);

    if (errno > 0) {
      fprintf(stderr, "\tError: %s\nTry again:\n", strerror(errno));
      errno = 0;
      continue;
    }

    if (!isfinite(number)) {
      fprintf(stderr, "\tBad number %lf. Try again:\n", number);
      continue;
    }

    if (lineSize > 1) {
      double *numberData = malloc(sizeof(double));
      *numberData = number;
      blist_add_tail(list, (void *) numberData);
    }
    else {
      break;
    }
  } while (true);

  return list;
}

int main(void) {
  printf("== Bidirectional linear list implementation. ==\n\n");

  BList orig = initNumbersList(NUMBERS_MAX);
  BList dst = blist_new(NULL);

  fillList(orig, dst);

  blist_each(dst, (EachCb) printNumber, false, NULL);

  blist_free(orig);
  blist_free(dst);

  BList doubleList = inputList();
  blist_each(doubleList, (EachCb) printDouble, false, NULL);

  blist_free(doubleList);

  exit(EXIT_SUCCESS);
}
