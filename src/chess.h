// Copyright 2024 Alejandro Fernández <aleferu888@gmail.com>

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#ifndef CHESS_H // TODO: Better name lmao
#define CHESS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#define ABS(x) ((x) >= 0 ? (x) : -(x))


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
    W_PAWNS_S   = 0x000000000000FF00,
    W_ROOKS_S   = 0x0000000000000081,
    W_KNIGHTS_S = 0x0000000000000042,
    W_BISHOPS_S = 0x0000000000000024,
    W_QUEEN_S   = 0x0000000000000010,
    W_KING_S    = 0x0000000000000008,
    B_PAWNS_S   = 0x00FF000000000000,
    B_ROOKS_S   = 0x8100000000000000,
    B_KNIGHTS_S = 0x4200000000000000,
    B_BISHOPS_S = 0x2400000000000000,
    B_QUEEN_S   = 0x1000000000000000,
    B_KING_S    = 0x0800000000000000,
} PIECE_STARTING_POSITION;
*/
#define W_PAWNS_S   (0x000000000000FF00)
#define W_ROOKS_S   (0x0000000000000081)
#define W_KNIGHTS_S (0x0000000000000042)
#define W_BISHOPS_S (0x0000000000000024)
#define W_QUEEN_S   (0x0000000000000010)
#define W_KING_S    (0x0000000000000008)
#define B_PAWNS_S   (0x00FF000000000000)
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
    uint8_t castling_rights;
} Board;


typedef struct {
    uint64_t* positions;
    size_t count;
} PositionArray;


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

Board* create_default_board(void);
void destroy_board(Board* board);
int evaluate_board(Board* board);
size_t count_bits(uint64_t number);
uint64_t get_all_occupied_squares(Board* board);
uint64_t get_white_occupied_squares(Board* board);
uint64_t get_black_occupied_squares(Board* board);
PositionArray* get_pieces_positions(uint64_t pieces);
void destroy_position_array(PositionArray* position_array);
bool is_piece_in_row(uint64_t piece_position, size_t row);
size_t get_piece_row(uint64_t piece_position);
bool is_piece_in_column(uint64_t piece_position, size_t col);
size_t get_piece_column(uint64_t piece_position);
MoveArray* create_move_array(void);
void destroy_move_array(MoveArray* move_array);
void insert_move_into_array(MoveArray* arr, Move item);
void insert_moves_into_array(MoveArray* arr, PIECE_INDEX piece_type, uint64_t previous_position, uint64_t next_positions);
uint64_t get_pseudomoves_from_white_pawn(Board* board, uint64_t piece_position, uint64_t same_color_occupied_squares, uint64_t opposite_color_occupied_squares);
uint64_t get_pseudomoves_from_black_pawn(Board* board, uint64_t piece_position, uint64_t same_color_occupied_squares, uint64_t opposite_color_occupied_squares);
uint64_t get_pseudomoves_from_rook(uint64_t piece_position, uint64_t same_color_occupied_squares, uint64_t opposite_color_occupied_squares);
uint64_t get_pseudomoves_from_knight(uint64_t piece_position, uint64_t same_color_occupied_squares);
uint64_t get_pseudomoves_from_bishop(uint64_t piece_position, uint64_t same_color_occupied_squares, uint64_t opposite_color_occupied_squares);
uint64_t get_pseudomoves_from_queen(uint64_t piece_position, uint64_t same_color_occupied_squares, uint64_t opposite_color_occupied_squares);
uint64_t get_pseudomoves_from_king(uint64_t piece_position, uint64_t same_color_occupied_squares);
void insert_pseudomoves_from_piece(Board* board, MoveArray* move_array, PIECE_INDEX piece_type, uint64_t piece_position, uint64_t same_color_occupied_squares, uint64_t opposite_color_occupied_squares);
MoveArray* get_pseudomoves_from_board(Board* board);
bool is_square_attacked(uint64_t piece_position, Board* board);
MoveArray* get_moves_from_board(Board* board);


#endif // CHESS_H
