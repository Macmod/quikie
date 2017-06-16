#ifndef LIST_H
#define LIST_H
#include <stdlib.h>
#include <stdio.h>
#define WSIZE 20

typedef struct Elem {
    char word[WSIZE];
    unsigned doc;
    unsigned freq;
    unsigned pos;
} Elem;

typedef struct {
    Elem *list;
    unsigned last,
             size;
} List;

int cmp_asc(Elem, Elem);
int cmp_desc(Elem, Elem);
void swap_elem(Elem*, Elem*);
void copy_elem(Elem*, Elem);
void print_elem(Elem);
void fread_elem_bin(FILE*, Elem*);
void fwrite_elem_bin(FILE**, Elem);
void fwrite_elem(FILE**, Elem);

List make_list(unsigned);
void append(List*, Elem);
void merge_list(List*, List*);
void overwrite_list(List*, List*);
List copy_list(List);
Elem *find_max(List*, int (*)(Elem, Elem));
Elem *find_min(List*, int (*)(Elem, Elem));
void print_list(List);
void destroy_list(List*);

#endif
