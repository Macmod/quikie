#ifndef SORTS_H
#define SORTS_H
#include "list.h"

/* Quick */
void partition(List*, unsigned, unsigned, int (*cmp)(Elem, Elem));
void quick(List*, int (*cmp)(Elem, Elem));

/* Insertion */
void insertion(List*, int (*cmp)(Elem, Elem));

/* Shell */
void shell(List*, int (*cmp)(Elem, Elem));

#endif
