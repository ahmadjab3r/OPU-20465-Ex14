CFLAGS = -ansi -Wall -pedantic
CC = gcc

main.o: main.c
	$(CC) $(CFLAGS) -c main.c



clean:
	rm -f letters.o letters xy_bits xy_bits.o