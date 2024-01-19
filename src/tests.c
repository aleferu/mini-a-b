#include <stdio.h>
#include <assert.h>

#include "chess.h"


#define ASSERT(result) \
    do { \
        if (!(result)) { \
            fprintf(stderr, "Assertion failed!\nFile: %s\nLine: %d\n", __FILE__, __LINE__); \
            assert(result); \
        } \
    } while (0)


void test_evaluate_board(void)
{
    Board* board = create_default_board();

    int evaluation = evaluate_board(board);
    ASSERT(evaluation == 0);
}


void test_count_bits(void)
{
    uint64_t foo = 4582762ULL;
    size_t bits_foo = 13;
    size_t bits_counted = count_bits(foo);
    ASSERT(bits_counted == bits_foo);
    ASSERT(count_bits(0ULL) == 0);
}


int main(void)
{
    printf("Nothing more should be printed\n");

    test_evaluate_board();

    return 0;
}
