#ifndef UTILS_H
#define UTILS_H
#define assert_file(f,path) if (f == NULL) {\
            printf("O arquivo %s não pôde ser aberto.\n", path);\
            exit(1);\
        }
#define add_slash(s) if (s[strlen(s)-1] != '/') strcat(s, "/");
#define debug(fmt, ...) if (VERBOSE) printf(fmt, __VA_ARGS__)

#endif
