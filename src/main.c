#include <stdio.h>
#include "chess.h"


int main(void)
{
    Board board = create_default_board();

    int evaluation = evaluate_board(&board);
    printf("eval: %d\n", evaluation);

    return 0;
}
