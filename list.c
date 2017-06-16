#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

// Elem
int cmp_asc(Elem a, Elem b) {
    int cmp = strcmp(a.word, b.word);
    if (cmp != 0) {
        return cmp;
    } else if (a.doc != b.doc) {
        return a.doc > b.doc ? 1 : -1;
    } else if (a.pos != b.pos) {
        return a.pos > b.pos ? 1 : -1;
    } else {
        return 0;
    }
}

int cmp_desc(Elem a, Elem b) {
    return cmp_asc(b, a);
}

void swap_elem(Elem *a, Elem *b) {
    Elem aux = *a;
    *a = *b;
    *b = aux;
}

void copy_elem(Elem *dst, Elem src) {
    for (unsigned i = 0; i < WSIZE; i++)
        dst->word[i] = '\0';
    strcpy(dst->word, src.word);
    dst->doc = src.doc;
    dst->freq = src.freq;
    dst->pos = src.pos;
}

void print_elem(Elem e) {
    printf("(%s,%u,%u,%u)", e.word, e.doc, e.freq, e.pos);
}

void fread_elem_bin(FILE *input, Elem *current) {
    fread(current, sizeof(Elem), 1, input);
}

void fwrite_elem_bin(FILE **output, Elem current) {
    fwrite(&current, sizeof(Elem), 1, *output);
}

void fwrite_elem(FILE **output, Elem current) {
    fprintf(*output, "%s,%u,%u,%u\n", current.word,
            current.doc, current.freq, current.pos);
}

// List
List make_list(unsigned size) {
    List l;
    l.list = (Elem*)calloc(size, sizeof(Elem));
    l.last = 0;
    l.size = size;

    return l;
}

void append(List *l, Elem e) {
    l->list[l->last] = e;
    l->last++;
}

void merge_list(List *dst, List *src) {
    for (unsigned i = 0; i < src->last; i++) {
        append(dst, src->list[i]);
    }
}

void overwrite_list(List *dst, List *src) {
    dst->last = 0;
    merge_list(dst, src);
}

List copy_list(List a) {
    List b = make_list(a.size);
    merge_list(&b, &a);

    return b;
}

Elem *find_max(List *l, int (*cmp)(Elem, Elem)) {
    Elem *max = &l->list[0];

    for (unsigned i = 1; i < l->last; i++) {
        if (cmp(l->list[i], *max) > 0)
            max = &l->list[i];
    }

    return max;
}

Elem *find_min(List *l, int (*cmp)(Elem, Elem)) {
    Elem *min = &l->list[0];

    for (unsigned i = 1; i < l->last; i++) {
        if (cmp(l->list[i], *min) < 0)
            min = &l->list[i];
    }

    return min;
}

void print_list(List l) {
    printf("[");
    for (unsigned i = 0; i < l.last; i++) {
        print_elem(l.list[i]);
        if (i < l.last - 1)
            printf(",");
    }
    printf("]\n");
}

void destroy_list(List *l) {
    free(l->list);
}
