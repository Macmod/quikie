#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "sorts.h"
#include "quickext.h"

/*
 * Sort options: insertion, shell, quick
 * Insertion performs better because
 * the array is nearly sorted.
 */
#define sort insertion

/* Handlers */
Handlers make_handlers(char path[]) {
    FILE *readL = fopen(path, "r+b");
    assert_file(readL, path);
    FILE *readR = fopen(path, "r+b");
    assert_file(readR, path);
    FILE *writeL = fopen(path, "r+b");
    assert_file(writeL, path);
    FILE *writeR = fopen(path, "r+b");
    assert_file(writeR, path);

    Handlers p;
    p.readL = readL;
    p.readR = readR;
    p.writeL = writeL;
    p.writeR = writeR;

    return p;
}

void handlers_flush(Handlers *p) {
    fflush(p->readL);
    fflush(p->readR);
    fflush(p->writeL);
    fflush(p->writeR);
}

void handlers_free(Handlers *p) {
    fclose(p->readL);
    fclose(p->readR);
    fclose(p->writeL);
    fclose(p->writeR);
}

/* External Quicksort */
void read_right(FILE **readR, int *ls, Elem *elem) {
    fseek(*readR, *ls * sizeof(Elem), SEEK_SET);
    fread_elem_bin(*readR, elem);
    (*ls)--;
}

void read_left(FILE **readL, int *li, Elem *elem) {
    fread_elem_bin(*readL, elem);
    (*li)++;
}

void write_right(FILE **writeR, int *es, Elem *elem) {
    fseek(*writeR, *es * sizeof(Elem), SEEK_SET);
    fwrite_elem_bin(writeR, *elem);
    (*es)--;
}

void write_left(FILE **writeL, int *ei, Elem *elem) {
    fwrite_elem_bin(writeL, *elem);
    (*ei)++;
}

void swap_elems(FILE **file, unsigned pos, int *e, Elem current, List *mem){
    if (pos == 0)
        write_left(file, e, &mem->list[pos]);
    else
        write_right(file, e, &mem->list[pos]);

    copy_elem(&mem->list[pos], current);
}

void fill_mem(Handlers *p, List *mem, unsigned *pswitch,
              int *li, int *ls) {
    mem->last = 0;
    while (mem->last < mem->size && *ls >= *li){
        if (!*pswitch)
            read_left(&p->readL, li, &mem->list[mem->last]);
        else
            read_right(&p->readR, ls, &mem->list[mem->last]);

        *pswitch = !(*pswitch);
        mem->last++;
    }
}

void ext_partition(Handlers *p, List *mem, int left, int right,
                   int *i, int *j, int (*cmp)(Elem, Elem)) {
    int li = left, ls = right;
    int ei = left, es = right;
    unsigned pswitch = 0;

    fseek(p->readL, li * sizeof(Elem), SEEK_SET);
    fseek(p->writeL, ei * sizeof(Elem), SEEK_SET);

    Elem current;
    *i = left - 1;
    *j = right + 1;

    fill_mem(p, mem, &pswitch, &li, &ls);
    quick(mem, cmp);

    while (ls >= li) {
        if (ls == es) {
            read_right(&p->readR, &ls, &current);
            pswitch = 0;
        } else if (li == ei) {
            read_left(&p->readL, &li, &current);
            pswitch = 1;
        } else {
            if (pswitch) {
                read_right(&p->readR, &ls, &current);
            } else {
                read_left(&p->readL, &li, &current);
            }

            pswitch = !pswitch;
        }

        if (cmp(current, mem->list[0]) < 0) {
            *i = ei;
            write_left(&p->writeL, &ei, &current);
        } else if (cmp(current, mem->list[mem->last-1]) > 0) {
            *j = es;
            write_right(&p->writeR, &es, &current);
        } else {
            if (ei - left < right - es)
                swap_elems(&p->writeL, 0, &ei, current, mem);
            else
                swap_elems(&p->writeR, mem->size - 1, &es, current, mem);

            sort(mem, cmp);
        }
    }

    for (unsigned k = 0; k < mem->last; k++)
        write_left(&p->writeL, &ei, &mem->list[k]);

    handlers_flush(p);
}

void ext_quicksort_rec(Handlers *p, List *mem, int left, int right,
                       int (*cmp)(Elem, Elem)) {
    if (left >= right)
        return;

    int i, j;
    ext_partition(p, mem, left, right, &i, &j, cmp);

    ext_quicksort_rec(p, mem, left, i, cmp);
    ext_quicksort_rec(p, mem, j, right, cmp);
}

void ext_quicksort(Handlers *p, unsigned count, List *mem,
                   int (*cmp)(Elem, Elem)) {
    ext_quicksort_rec(p, mem, 0, count-1, cmp);
}
