#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


#define N_PIECES (12)
#define BOARD_SQUARES (64)

#define WHITE_TURN (true)
#define BLACK_TURN (false)


/*
Sadly, enums should only have values in the range of ints

typedef enum {
    W_PAWNS_S   = 0x000000000000ff00,
    W_ROOKS_S   = 0x0000000000000081,
    W_KNIGHTS_S = 0x0000000000000042,
    W_BISHOPS_S = 0x0000000000000024,
    W_QUEEN_S   = 0x0000000000000010,
    W_KING_S    = 0x0000000000000008,
    B_PAWNS_S   = 0x00ff000000000000,
    B_ROOKS_S   = 0x8100000000000000,
    B_KNIGHTS_S = 0x4200000000000000,
    B_BISHOPS_S = 0x2400000000000000,
    B_QUEEN_S   = 0x1000000000000000,
    B_KING_S    = 0x0800000000000000,
} PIECE_STARTING_POSITION;
*/
// Parenthesis are not necessary
#define W_PAWNS_S   (0x000000000000ff00)
#define W_ROOKS_S   (0x0000000000000081)
#define W_KNIGHTS_S (0x0000000000000042)
#define W_BISHOPS_S (0x0000000000000024)
#define W_QUEEN_S   (0x0000000000000010)
#define W_KING_S    (0x0000000000000008)
#define B_PAWNS_S   (0x00ff000000000000)
#define B_ROOKS_S   (0x8100000000000000)
#define B_KNIGHTS_S (0x4200000000000000)
#define B_BISHOPS_S (0x2400000000000000)
#define B_QUEEN_S   (0x1000000000000000)
#define B_KING_S    (0x0800000000000000)


typedef enum {
    W_PAWN_I   = 0,
    W_ROOK_I   = 1,
    W_KNIGHT_I = 2,
    W_BISHOP_I = 3,
    W_QUEEN_I  = 4,
    W_KING_I   = 5,
    B_PAWN_I   = 6,
    B_ROOK_I   = 7,
    B_KNIGHT_I = 8,
    B_BISHOP_I = 9,
    B_QUEEN_I  = 10,
    B_KING_I   = 11,
} PIECE_INDEX;


typedef enum {
    PAWN_V   = 1,
    ROOK_V   = 5,
    KNIGHT_V = 3,
    BISHOP_V = 3,
    QUEEN_V  = 9
} PIECE_VALUE;


typedef struct {
    uint64_t* pieces;
    bool turn;
} Board;


Board create_default_board()
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


typedef struct {
    PIECE_INDEX piece_type;
    uint64_t previous_position;
    uint64_t next_position;
} Move;


uint64_t* get_pieces_positions(uint64_t pieces)
{
    int piece_count = count_bits(pieces);
    uint64_t* pieces_positions = malloc(sizeof(uint64_t) * (piece_count + 1));
    if (pieces_positions == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    uint64_t position = 1ULL;
    for (int i = 0; i < BOARD_SQUARES; ++i) {
        if ((pieces & position) != 0) {
            pieces_positions[index] = position;
            if ((++index) == piece_count) {
                pieces_positions[index] = 0ULL;
                return pieces_positions;
            }
        }
        position = position << 1;
    }

    // Unreachable
    fprintf(stderr, "Error: get_pieces_positions has a bug");
    exit(1);
}


int main(void)
{
    Board board = create_default_board();

    int evaluation = evaluate_board(&board);
    printf("eval: %d\n", evaluation);

    return 0;
}
