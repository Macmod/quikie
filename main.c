#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "list.h"
#include "quickext.h"
#include "invindex.h"
#define BINDEX "tmp/bin_index"
#define MAXP 256
#define ORDER 1
#define VERBOSE 1

int main() {
    /* Variables */
    unsigned size, wc = 0;
    unsigned d, m;
    char e[MAXP], s[MAXP];
    char path[MAXP];
    FILE *doc;
    FILE *index;
    FILE *bin_index;

    /* Input */
    scanf("%u %u %[^ ] %[^ \n]", &d, &m, e, s);
    if (m < sizeof(Elem)) {
        printf("A memória não pode ser menor que %ldB!\n", sizeof(Elem));
        exit(1);
    }
    add_slash(e);
    add_slash(s);
    strcat(s, "index");
    size = m/sizeof(Elem);

    /* Header (info) */
    debug(
        "[Input Path] %s\n"
        "[Output Path] %s\n"
        "Record Size: %lu\n"
        "Used Records: %u\n"
        "Used Memory: %lu (<= %u)\n"
        "Order: %s\n",
        e, s, sizeof(Elem), size, size * sizeof(Elem),
        m, ORDER ? "ASC" : "DESC"
    );

    /* Write binary file containing all documents */
    bin_index = fopen(BINDEX, "w+b");
    assert_file(bin_index, BINDEX);
    for (unsigned i = 1; i <= d; i++) {
        sprintf(path, "%s%d", e, i);
        debug("[Input] %s\n", path);

        doc = fopen(path, "r+");
        assert_file(doc, path);
        wc += write_pseudo_index(&bin_index, doc, i);
        fclose(doc);
    }
    fclose(bin_index);
    debug("Word Count: %u\n", wc);

    /* External sort */
    List mem = make_list(size);
    Handlers p = make_handlers(BINDEX);
    ext_quicksort(
        &p, wc, &mem,
        ORDER ? cmp_asc : cmp_desc
    );
    handlers_free(&p);
    debug("%s\n", "[Sorted]");

    /* Build index in output file */
    bin_index = fopen(BINDEX, "r+b");
    assert_file(bin_index, BINDEX);
    index = fopen(s, "w+");
    assert_file(index, s);
    build_index(bin_index, index, &mem, wc);
    debug("%s\n", "[Indexed]");

    /* Cleanup */
    fclose(index);
    fclose(bin_index);
    destroy_list(&mem);
    remove(BINDEX);

    return 0;
}
