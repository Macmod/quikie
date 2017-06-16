#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#include "quickext.h"
#include "invindex.h"

unsigned write_pseudo_index(FILE **output, FILE *input, unsigned i) {
    Elem el;
    for (unsigned i = 0; i < WSIZE; i++)
        el.word[i] = '\0';
    el.doc = i;
    el.freq = 1;

    unsigned wc;
    unsigned pos = 0;
    for (wc = 0; fscanf(input, "%s", el.word) != EOF; wc++) {
        el.pos = pos;
        fwrite_elem_bin(output, el);
        pos = ftell(input)+1;
    }

    return wc;
}

void bin_to_text(FILE *input, FILE *output, List *mem, unsigned wc) {
    for (unsigned i = 0; i < wc; i++) {
        fread_elem_bin(input, &mem->list[0]);
        fwrite_elem(&output, mem->list[0]);
    }
}

void write_back(FILE *input, FILE **output, Elem *elem,
                unsigned pos, unsigned freq) {
    fseek(input, pos, SEEK_SET);
    for (unsigned i = 0; i < freq; i++) {
        fread_elem_bin(input, elem);
        elem->freq = freq;
        fwrite_elem(output, *elem);
    }
}

void build_index(FILE *input, FILE *output, List *mem, unsigned wc) {
    char aux[WSIZE] = "\0";
    unsigned count = 0;
    unsigned pos = 0;
    unsigned doc = 0;

    for (unsigned i = 0; i < wc; i++) {
        fread_elem_bin(input, &mem->list[0]);

        if (i == 0 || (mem->list[0].doc == doc &&
                strcmp(mem->list[0].word, aux) == 0)) {
            count++;
        } else {
            write_back(input, &output, &mem->list[0], pos, count);

            count = 1;
            pos = ftell(input);
            fread_elem_bin(input, &mem->list[0]);
        }

        strcpy(aux, mem->list[0].word);
        doc = mem->list[0].doc;
    }

    write_back(input, &output, &mem->list[0], pos, count);
}
