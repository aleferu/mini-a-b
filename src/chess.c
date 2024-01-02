#include <stdlib.h>
#include <stdio.h>

#include "chess.h"


Board create_default_board(void)
{
    uint64_t* pieces = malloc(sizeof(uint64_t) * N_PIECES);
    if (pieces == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }

    pieces[W_PAWN_I]   = W_PAWNS_S;
    pieces[W_ROOK_I]   = W_ROOKS_S;
    pieces[W_KNIGHT_I] = W_KNIGHTS_S;
    pieces[W_BISHOP_I] = W_BISHOPS_S;
    pieces[W_QUEEN_I]  = W_QUEEN_S;
    pieces[W_KING_I]   = W_KING_S;

    pieces[B_PAWN_I]   = B_PAWNS_S;
    pieces[B_ROOK_I]   = B_ROOKS_S;
    pieces[B_KNIGHT_I] = B_KNIGHTS_S;
    pieces[B_BISHOP_I] = B_BISHOPS_S;
    pieces[B_QUEEN_I]  = B_QUEEN_S;
    pieces[B_KING_I]   = B_KING_S;

    Board result;
    result.pieces = pieces;
    result.turn = WHITE_TURN;
    return result;
}


void destroy_board(Board* board)
{
    free(board->pieces);
    board->pieces = NULL;
}


// Can be uint
int count_bits(uint64_t number)
{
    int counter = 0;
    while (number) {
        number = number & (number - 1);
        counter += 1;
    }
    return counter;
}


int evaluate_board(Board* board)
{
    int result = 0;

    result += count_bits(board->pieces[W_PAWN_I])   * PAWN_V;
    result += count_bits(board->pieces[W_ROOK_I])   * ROOK_V;
    result += count_bits(board->pieces[W_KNIGHT_I]) * KNIGHT_V;
    result += count_bits(board->pieces[W_BISHOP_I]) * BISHOP_V;
    result += count_bits(board->pieces[W_QUEEN_I])  * QUEEN_V;

    result -= count_bits(board->pieces[B_PAWN_I])   * PAWN_V;
    result -= count_bits(board->pieces[B_ROOK_I])   * ROOK_V;
    result -= count_bits(board->pieces[B_KNIGHT_I]) * KNIGHT_V;
    result -= count_bits(board->pieces[B_BISHOP_I]) * BISHOP_V;
    result -= count_bits(board->pieces[B_QUEEN_I])  * QUEEN_V;

    return result;
}


uint64_t get_all_occupied_squares(Board* board)
{
    uint64_t result = 0;
    for (int i = 0; i < N_PIECES; ++i) {
        result |= board->pieces[i];
    }
    return result;
}


uint64_t* get_pieces_positions(uint64_t pieces)
{
    uint64_t piece_count = (uint64_t) count_bits(pieces);
    uint64_t* pieces_positions = malloc(sizeof(uint64_t) * (piece_count + 1));
    if (pieces_positions == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }

    size_t index = 0;
    uint64_t position = 1ULL;
    for (size_t i = 0; i < BOARD_SQUARES; ++i) {
        if ((pieces & position) != 0) {
            pieces_positions[index] = position;
            if (((uint64_t) ++index) == piece_count) {
                pieces_positions[index] = 0ULL;
                return pieces_positions;
            }
        }
        position = position << 1;
    }

    // Unreachable
    fprintf(stderr, "Error: get_pieces_positions has a bug\n");
    fprintf(stderr, "pieces: %zu\n", pieces);
    fprintf(stderr, "position: %zu\n", position);
    fprintf(stderr, "index: %zu\n", index);
    fprintf(stderr, "pieces_position:\n");
    for (size_t i = 0; i < piece_count; ++i) {
        fprintf(stderr, "\t%zu\n", pieces_positions[i]);
    }
    exit(1);
}
