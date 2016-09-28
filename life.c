#define _GNU_SOURCE

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MARGIN (1)
#define ROWS (48 + 2*MARGIN)
#define COLS (98 + 2*MARGIN)
#define DEAD (0)
#define ALIVE (1)
#define CELLCHAR '*'
#define INTERVAL (100000)   /* usec, 0.1 second */


void clearscreen(void);
int zero_one(void);
void printlife(char matrix[ROWS][COLS]);


int main(int argc, char *argv[])
{
    char now[ROWS][COLS], next[ROWS][COLS];
    int l, r, c, g=0;
    FILE *initstate;
    char cell;


    /* zero the initial matrix: */
    memset(now, 0, ROWS*COLS*sizeof(char));

    if (argc == 1) {
        srand(time(NULL));

        for (r=1; r<ROWS-1; ++r) {
            for (c=1; c<COLS-1; ++c) {
                now[r][c] = zero_one();
            }
        }
    } else if (argc == 2) {
        if ((initstate = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr, "couldn't open file \"%s\", terminating.\n", argv[1]);
            return 1;
        } else {
            /* FIXME unsafe, no bounds checking */
            r = c = MARGIN;
            while ((cell = fgetc(initstate)) != EOF) {
                 if (cell == '\n') {
                    ++r;
                    c = MARGIN;
                } else if (cell == ' ') {
                    now[r][c] = DEAD;
                    ++c;
                } else {
                    now[r][c] = ALIVE;
                    ++c;
                }
            }
        }
        fclose(initstate);
    }

    clearscreen();
    printf(" generation: %d\n", g);
    printlife(now);
    sleep(2);


    while (1) {
        ++g;

        /* zero the next generation matrix: */
        memset(next, 0, ROWS*COLS*sizeof(char));

        for ( r=1; r<ROWS-1; ++r ) {
            for ( c=1; c<COLS-1; ++c ) {
                /* count living neighbours */
                l=0;

                if (now[r+1][c-1] == ALIVE) ++l;
                if (now[r+1][c+0] == ALIVE) ++l;
                if (now[r+1][c+1] == ALIVE) ++l;
                if (now[r+0][c-1] == ALIVE) ++l;
                if (now[r+0][c+1] == ALIVE) ++l;
                if (now[r-1][c-1] == ALIVE) ++l;
                if (now[r-1][c+0] == ALIVE) ++l;
                if (now[r-1][c+1] == ALIVE) ++l;

                /*
                 * rules:
                 * 1. alive cell with fewer than two living neighbours dies (starvation)
                 * 2. alive cell with more than three living neighbours dies (overpopulation)
                 * 3. alive cell with two or three living neighbours lives on
                 * 4. dead cell with exactly three living neighbours becomes alive (reproduction)
                 */

                     if (now[r][c] == ALIVE && l < 2) next[r][c] = DEAD;
                else if (now[r][c] == ALIVE && l > 3) next[r][c] = DEAD;
                else if (now[r][c] == ALIVE && l >= 2 && l <= 3) next[r][c] = ALIVE;
                else if (now[r][c] == DEAD && l == 3) next[r][c] = ALIVE;
            }
        }

        /* copy matrix next -> now: */
        memcpy(now, next, ROWS*COLS*sizeof(char));

        clearscreen();
        printf(" generation: %d\n", g);
        printlife(now);
        usleep(INTERVAL);
    }

    return 0;
}


int zero_one(void)
{
    return( rand()%2 );
}


void clearscreen(void)  /* works only on "ANSI terminals", whatever that means */
{
    printf( "\033[2J" );
    printf( "\033[1;1H" );
}


void printlife(char matrix[ROWS][COLS])
{
    int r, c;

    for (r=0; r<ROWS; ++r) {
        for (c=0; c<COLS; ++c) {
            if (matrix[r][c] == ALIVE) {
                putchar(CELLCHAR);
            } else {
                putchar(' ');
            }
        }
        putchar('\n');
    }
    putchar('\n');
}
