#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ROWS (48+2)
#define COLS (98+2)

#define DEAD 0
#define LIVE 1

#define INTERVAL (100000)   // in microseconds

void clearscreen(void);
int zero_one(void);
void printlife( int matrix[ROWS][COLS] );


int main(int argc, char *argv[])
{
    int now[ROWS][COLS], next[ROWS][COLS];
    int l, d, r, c, g=0;
    FILE *initstate;
    char cell;


    // zero the initial matrix:
    memset(now,0,ROWS*COLS*sizeof(int));
    

    if (argc==1)
    {
        srand(time(NULL));

        for ( r=1; r<ROWS-1; r++ )
            for ( c=1; c<COLS-1; c++ )
                now[r][c] = zero_one();
    }
    else if (argc==2)
    {
        initstate = fopen(argv[1],"r");

        if (!initstate)
        {
            printf( "\n couldn't open file '%s', terminating.\n\n", argv[1] );
            exit(EXIT_FAILURE);
        }
        else
        {
            for (r=1; r<ROWS-1; r++)
            {
                for (c=1; c<COLS; c++)  // not COLS-1 because we expect the newline character!
                {
                    fscanf(initstate,"%1c",&cell);

                    if (cell=='.')
                        now[r][c] = 0;
                    else if (cell=='+')
                        now[r][c] = 1;
                }
            }
        }

        fclose(initstate);
    }

    clearscreen();
    printf(" generation: %d\n", g);
    printlife(now);
    sleep(2);


    while(1)
    {
        g++;

        // zero the next generation matrix:
        memset(next,0,ROWS*COLS*sizeof(int));

        for ( r=1; r<ROWS-1; r++ )
        {
            for ( c=1; c<COLS-1; c++ )
            {
                // count neighbours
                l=0; d=0;

                if (now[r+1][c-1]) l++; else d++;
                if (now[r+1][c+0]) l++; else d++;
                if (now[r+1][c+1]) l++; else d++;
                if (now[r+0][c-1]) l++; else d++;
                if (now[r+0][c+1]) l++; else d++;
                if (now[r-1][c-1]) l++; else d++;
                if (now[r-1][c+0]) l++; else d++;
                if (now[r-1][c+1]) l++; else d++;

                /*
                  rules:
                  1. live cell with fewer than two live neighbours dies
                  2. live cell with more than three live neighbours dies
                  3. live cell with two or three live neighbours lives on
                  4. dead cell with exactly three live neighbours becomes live
                */

                     if (  now[r][c] && l<2 ) next[r][c] = DEAD;
                else if (  now[r][c] && l>3 ) next[r][c] = DEAD;
                else if (  now[r][c] && l>=2 && l<=3 ) next[r][c] = LIVE;
                else if ( !now[r][c] && (l==3) ) next[r][c] = LIVE;
            }
        }

        // copy matrix next -> now:
        for (r=0; r<ROWS; r++)
            for (c=0; c<COLS; c++)
                now[r][c] = next[r][c];


        clearscreen();
        printf(" generation: %d\n", g);
        printlife(now);
        usleep(INTERVAL);
    }

    return(0);
}



int zero_one(void)
{
    return( rand()%2 );
}



void clearscreen(void)  // works only on "ANSI terminals", whatever that means
{
    printf( "\033[2J" );
    printf( "\033[1;1H" );

    return;
}



void printlife( int matrix[ROWS][COLS] )
{
    int r, c;

    for ( r=0; r<ROWS; r++)
    {
        for ( c=0; c<COLS; c++)
        {
            if (matrix[r][c])
                printf("+");
            else
                printf(" ");
        }

        printf("\n");
    }

    printf("\n");

    return;
}
