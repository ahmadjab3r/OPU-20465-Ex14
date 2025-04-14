CFLAGS = -ansi -Wall -pedantic -g
CC = gcc
DEPS = assembler.o
assembler: $(DEPS)
	$(CC) -g $(CFLAGS) -o $@ $(DEPS)




clean:
	rm