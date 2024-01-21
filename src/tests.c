#include <stdio.h>
#include <assert.h>

#include "chess.h"


void test_evaluate_board(Board* board)
{
    int evaluation = evaluate_board(board);
    assert(evaluation == 0);
}


void test_count_bits(void)
{
    uint64_t foo = 4582762ULL;
    size_t bits_foo = 13;
    size_t bits_counted = count_bits(foo);
    assert(bits_counted == bits_foo);
    assert(count_bits(0ULL) == 0);
}


void test_get_occupied_squares(Board* board)
{
    uint64_t result_all = get_all_occupied_squares(board);
    uint64_t result_white = get_white_occupied_squares(board);
    uint64_t result_black = get_black_occupied_squares(board);

    assert(result_white == 0x000000000000FFFFULL);
    assert(result_black == 0xFFFF000000000000ULL);
    assert(result_all   == 0xFFFF00000000FFFFULL);
}


void test_get_pieces_positions(Board* board)
{
    PositionArray* arr = get_pieces_positions(board->pieces[W_BISHOP_I]);

    assert(arr->count == 2);
    assert(arr->positions[0] == 0x0000000000000004ULL);
    assert(arr->positions[1] == 0x0000000000000020ULL);

    destroy_position_array(arr);
}


void test_is_piece_in(void)
{
    uint64_t piece_position = 0x0000000000000004ULL;
    // row
    bool result = is_piece_in_row(piece_position, 3);
    assert(!result);
    result = is_piece_in_row(piece_position, get_piece_row(piece_position));
    assert(result);
    // column
    result = is_piece_in_column(piece_position, 3);
    assert(!result);
    result = is_piece_in_column(piece_position, get_piece_column(piece_position));
    assert(result);
}


int main(void)
{
    printf("Nothing more should be printed\n");

    Board* default_board = create_default_board();

    test_evaluate_board(default_board);
    test_count_bits();
    test_get_occupied_squares(default_board);
    test_get_pieces_positions(default_board);
    test_is_piece_in();

    destroy_board(default_board);

    return 0;
}
