TARGET = bin/quikie
LIBS = -lm
CC = gcc -O2
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic

.PHONY: default all clean tests

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

toys:
	tests/toys/toys.sh ./$(TARGET) tests/toys

tests:
	tests/big/big.sh ./$(TARGET) tests/big

valgrind:
	cat tests/toys/in/toy_9_input
	valgrind --track-origins=yes --leak-resolution=high --leak-check=full --show-reachable=yes ./$(TARGET) < tests/toys/in/toy_9_input

massif:
	cat tests/toys/in/toy_9_input
	valgrind --massif-out-file=/tmp/massif --tool=massif ./$(TARGET) < tests/toys/in/toy_9_input
	ms_print /tmp/massif | less

clean:
	-rm -f *.o
