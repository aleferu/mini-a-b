#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


/**
 * Constants
 */

#define N_PIECES (12)
#define BOARD_SQUARES (64)
#define ROW_SQUARES (8)
#define COL_SQUARES (8)

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


/**
 * Enums
 */

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


/**
 * Structs
 */

typedef struct {
    uint64_t* pieces;
    bool turn;
    bool en_passant;
    uint64_t en_passant_square;
} Board;


typedef struct {
    PIECE_INDEX piece_type;
    uint64_t previous_position;
    uint64_t next_position;
} Move;


typedef struct {
    Move* moves;
    size_t capacity;
    size_t count;
} MoveArray;


/**
 * Functions
 */

Board create_default_board(void);
void destroy_board(Board* board);
int evaluate_board(Board* board);
