#ifndef INVINDEX_H
#define INVINDEX_H
#include "list.h"

/* Inv. Index */
unsigned write_pseudo_index(FILE**, FILE*, unsigned);
void bin_to_text(FILE*, FILE*, List*, unsigned);
void build_index(FILE*, FILE*, List*, unsigned);

#endif
