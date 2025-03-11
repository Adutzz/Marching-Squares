# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

# define targets
TARGETS=marching_squares

build: $(TARGETS)

marching_squares: tema2.c
	$(CC) $(CFLAGS) tema2.c -o marching_squares

pack:
	zip -FSr 315CA_RotaruFlorisAdrian_Tema2.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean