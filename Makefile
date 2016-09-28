all: life

life: life.c
	gcc -Wall -Werror -ansi -pedantic life.c -o GameOfLife

clean:
	rm -rf GameOfLife
