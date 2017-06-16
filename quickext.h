#ifndef EXT_QUICKSORT_H
#define EXT_QUICKSORT_H

/* Handlers (auxiliary data type) */
typedef struct Handlers {
    FILE *readL;
    FILE *readR;
    FILE *writeL;
    FILE *writeR;
} Handlers;

Handlers make_handlers(char[]);
void handlers_flush(Handlers*);
void handlers_free(Handlers*);

/* External Quicksort */
void ext_quicksort_rec(Handlers*, List*, int, int, int (*)(Elem,Elem));
void ext_quicksort(Handlers*, unsigned, List*, int (*)(Elem,Elem));

#endif
