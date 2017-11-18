#include <iostream>
#include <stdio.h>
#include "Puzzle.h"

int main()
{
    SeekPath PUZZLE;
    char c;
    do{
        system("cls");
        PUZZLE.puzzle_initialize();
        PUZZLE.seekQueue();
        PUZZLE.seek_shortest(PUZZLE.in);
        PUZZLE.tail();
        PUZZLE.show();
        c=getchar();
    }while(c !='s' && c!='S');

    return 0;
}
