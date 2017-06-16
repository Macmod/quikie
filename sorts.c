#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "sorts.h"

/* Quick */
void partition(List *list, unsigned left, unsigned right,
               int (*cmp)(Elem a, Elem b)) {
    if (left < right) {
        Elem pivot = list->list[(left+right)/2];

        unsigned i = left;
        unsigned j = right;
        while (true) {
            while (cmp(list->list[i], pivot) < 0)
                i++;
            while (cmp(list->list[j], pivot) > 0)
                j--;

            if (i >= j)
                break;

            swap_elem(&list->list[i], &list->list[j]);
        }

        partition(list, left, j, cmp);
        partition(list, j + 1, right, cmp);
    }
}

void quick(List *list, int (*cmp)(Elem a, Elem b)) {
    partition(list, 0, list->last - 1, cmp);
}

/* Insertion */
void insertion(List *l, int (*cmp)(Elem a, Elem b)) {
    int i, j;
    for (i = 1; i < (int)l->last; i++) {
        Elem current = l->list[i];
        for (j = i-1; j >= 0 && cmp(current, l->list[j]) < 0; j--)
            l->list[j+1] = l->list[j];
        l->list[j+1] = current;
    }
}

/* Shell */
void shell(List *list, int (*cmp)(Elem a, Elem b)) {
    // Initial gap
    unsigned gap;
    for (gap = 1; gap < list->last; gap = gap * 3 + 1);

    // Sequence of gaps
    int i, j;
    Elem ref;
    while (gap > 0) {
        gap /= 3;

        // Insertion
        for (i = gap; i < (int)list->last; i++) {
            j = i - gap;

            ref = list->list[i];
            while (j >= 0 && cmp(ref, list->list[j]) < 0) {
                list->list[j + gap] = list->list[j];
                j -= gap;
            }
            list->list[j + gap] = ref;
        }
    }
}
