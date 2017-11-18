#include <iostream>
#include <stdio.h>
#include "Puzzle.h"

int main()
{
    SeekPath PUZZLE;
    do{
    system("cls");
    PUZZLE.puzzle_initialize();
    PUZZLE.seekQueue();
    PUZZLE.seek_shortest(PUZZLE.in);
    PUZZLE.tail();
    PUZZLE.show();
    }while(getchar()!='N');

    return 0;
}
