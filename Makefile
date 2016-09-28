all: life

life: life.c
	gcc -Wall life.c -o GameOfLife

clean:
	rm -rf GameOfLife
