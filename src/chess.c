#include <stdint.h>
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


// Not sure I'll ever use it
uint64_t get_all_occupied_squares(Board* board)
{
    uint64_t result = 0;
    for (size_t i = 0; i < N_PIECES; ++i) {
        result |= board->pieces[i];
    }
    return result;
}


uint64_t get_white_occupied_squares(Board* board)
{
    uint64_t result = 0;
    for (size_t i = 0; i < N_PIECES / 2; ++i) {
        result |= board->pieces[i];
    }
    return result;
}


uint64_t get_black_occupied_squares(Board* board)
{
    uint64_t result = 0;
    for (size_t i = N_PIECES / 2; i < N_PIECES; ++i) {
        result |= board->pieces[i];
    }
    return result;
}


uint64_t* get_pieces_positions(uint64_t pieces)
{
    uint64_t piece_count = (uint64_t) count_bits(pieces);
    uint64_t* pieces_positions = malloc(sizeof(uint64_t) * piece_count);
    if (pieces_positions == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }

    size_t index = 0;
    uint64_t position = 1ULL;
    for (size_t _ = 0; _ < BOARD_SQUARES; ++_) {
        if ((pieces & position) != 0) {
            pieces_positions[index++] = position;
        }
        position = position << 1;
    }

    return pieces_positions;
}


MoveArray create_move_array(void)
{
    size_t capacity = 20;
    Move* moves = malloc(sizeof(Move) * capacity);
    if (moves == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    MoveArray result;
    result.moves = moves;
    result.count = 0;
    result.capacity = capacity;
    return result;
}


bool is_piece_in_row(uint64_t piece_position, uint row)
{
    return ((piece_position >> ((row - 1) * 8)) & 0xFF) != 0;
}


size_t get_piece_row(uint64_t piece_position)
{
    for (size_t i = 0; i < ROW_SQUARES; ++i) {
        if ((piece_position & (0xFFULL << (i * ROW_SQUARES))) != 0ULL)
            return i + 1;
    }
    fprintf(stderr, "Error: get_piece_row failed\n");
    exit(1);
}


bool is_piece_in_column(uint64_t piece_position, uint col)
{
    uint64_t collider = 0ULL;
    uint64_t column = (COL_SQUARES + 1) - col; // 1 -> 8 ; 8 -> 1 ; 4 -> 5
    for (size_t i = 0; i < COL_SQUARES; ++i) {
        collider |= (1ULL << (column + i * COL_SQUARES));
    }
    return piece_position & collider;
}


size_t get_piece_col(uint64_t piece_position)
{
    uint64_t collider = 0ULL;
    for (size_t i = 0; i < COL_SQUARES; ++i) {
        collider |= (1ULL << (i * COL_SQUARES));
    }
    for (size_t i = 0; i < COL_SQUARES; ++i) {
        if ((collider & piece_position) != 0ULL)
            return COL_SQUARES - i;
        collider <<= 1;
    }
    fprintf(stderr, "Error: get_piece_col failed\n");
    exit(1);
}


void insert_move_into_array(MoveArray* arr, Move item)
{
    if (arr->count == arr->capacity) {
        Move* temp = realloc(arr->moves, sizeof(Move) * arr->capacity * 2);
        if (temp == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(1);
        } else {
            arr->moves = temp;
            arr->capacity *= 2;
        }
    }
    arr->moves[arr->count++] = item;
}


void insert_moves_into_array(MoveArray* arr, PIECE_INDEX piece_type, uint64_t previous_position, uint64_t next_positions)
{
    int move_counter = count_bits(next_positions);
    uint64_t* new_positions = get_pieces_positions(next_positions);
    for (size_t i = 0; i < (size_t) move_counter; ++i) {
        insert_move_into_array(arr, (Move) {piece_type, previous_position, new_positions[i]});
    }
}


uint64_t get_pseudomoves_from_white_pawn(Board* board, uint64_t piece_position, uint64_t same_color_occupied_squares, uint64_t opposite_color_occupied_squares)
{
    uint64_t found_positions = 0ULL;
    uint64_t potential_new_position;
    uint64_t occupied_squares = same_color_occupied_squares & opposite_color_occupied_squares;
    // Double step
    if (is_piece_in_row(piece_position, 2)) {
        potential_new_position = piece_position << 16;
        if ((potential_new_position & occupied_squares) == 0ULL) {
            found_positions |= potential_new_position;
        }
    }
    // Normal step
    potential_new_position = piece_position << 8;
    if ((potential_new_position & occupied_squares) == 0ULL) {
        found_positions |= potential_new_position;
    }
    // Eat to the left
    if (!is_piece_in_column(piece_position, 1)) {
        potential_new_position = piece_position << 9;
        if ((potential_new_position & opposite_color_occupied_squares) != 0ULL || (board->en_passant && board->en_passant_square == potential_new_position)) {
            found_positions |= potential_new_position;
        }
    }
    // Eat to the right
    if (!is_piece_in_column(piece_position, 8)) {
        potential_new_position = piece_position << 7;
        if ((potential_new_position & opposite_color_occupied_squares) != 0ULL || (board->en_passant && board->en_passant_square == potential_new_position)) {
            found_positions |= potential_new_position;
        }
    }
    return found_positions;
}


uint64_t get_pseudomoves_from_black_pawn(Board* board, uint64_t piece_position, uint64_t same_color_occupied_squares, uint64_t opposite_color_occupied_squares)
{
    uint64_t found_positions = 0ULL;
    uint64_t potential_new_position;
    uint64_t occupied_squares = same_color_occupied_squares & opposite_color_occupied_squares;
    // Double step
    if (is_piece_in_row(piece_position, 6)) {
        potential_new_position = piece_position >> 16;
        if ((potential_new_position & occupied_squares) == 0ULL) {
            found_positions |= potential_new_position;
        }
    }
    // Normal step
    potential_new_position = piece_position >> 8;
    if ((potential_new_position & occupied_squares) == 0ULL) {
        found_positions |= potential_new_position;
    }
    // Eat to the left
    if (!is_piece_in_column(piece_position, 1)) {
        potential_new_position = piece_position >> 7;
        if ((potential_new_position & opposite_color_occupied_squares) != 0ULL || (board->en_passant && board->en_passant_square == potential_new_position)) {
            found_positions |= potential_new_position;
        }
    }
    // Eat to the right
    if (!is_piece_in_column(piece_position, 8)) {
        potential_new_position = piece_position >> 8;
        if ((potential_new_position & opposite_color_occupied_squares) != 0ULL || (board->en_passant && board->en_passant_square == potential_new_position)) {
            found_positions |= potential_new_position;
        }
    }
    return found_positions;
}


uint64_t get_pseudomoves_from_rook(uint64_t piece_position, uint64_t same_color_occupied_squares, uint64_t opposite_color_occupied_squares)
{
    uint64_t found_positions = 0ULL;
    for (size_t i = 0; i < 4; ++i) {
        uint64_t current_square = piece_position;
        while (true) {
            if (i == 0) { // left
                if (is_piece_in_column(current_square, 8))
                    break;
                current_square <<= 1;
            } else if (i == 1) { // right
                if (is_piece_in_column(current_square, 1))
                    break;
                current_square >>= 1;
            } else if (i == 2) { // up
                if (is_piece_in_row(current_square, 8))
                    break;
                current_square <<= 8;
            } else { // down
                if (is_piece_in_row(current_square, 1))
                    break;
                current_square >>= 8;
            }
            if ((current_square & same_color_occupied_squares) != 0ULL)
                break;
            found_positions |= current_square;
            if ((current_square & opposite_color_occupied_squares) != 0ULL)
                break;
        }
    }
    return found_positions;
}


uint64_t get_pseudomoves_from_knight(uint64_t piece_position, uint64_t same_color_occupied_squares)
{
    uint64_t found_positions = 0ULL;
    int piece_col = (int) get_piece_col(piece_position);
    int piece_row = (int) get_piece_row(piece_position);
    int knight_offsets[8] = { -17, -15, -10, -6, 6, 10, 15, 17 };
    for (size_t i = 0; i < 8; ++i) {
        uint64_t potential_new_position = piece_position << knight_offsets[i];
        if (potential_new_position > 0 && (potential_new_position & same_color_occupied_squares) == 0) {
            int col_difference = piece_col - (int) get_piece_col(potential_new_position);
            int row_difference = piece_row - (int) get_piece_row(potential_new_position);
            if ((ABS(col_difference) == 1 && ABS(row_difference) == 2) || (ABS(col_difference) == 2 && ABS(row_difference) == 1)) {
                found_positions |= potential_new_position;
            }
        }
    }
    return found_positions;
}


void insert_pseudomoves_from_piece(Board* board, MoveArray* move_array, PIECE_INDEX piece_type, uint64_t piece_position, uint64_t same_color_occupied_squares, uint64_t opposite_color_occupied_squares)
{
    uint64_t next_positions;
    switch (piece_type) {
    case W_PAWN_I:
        next_positions = get_pseudomoves_from_white_pawn(board, piece_position, same_color_occupied_squares, opposite_color_occupied_squares);
        break;
    case B_PAWN_I:
        next_positions = get_pseudomoves_from_black_pawn(board, piece_position, same_color_occupied_squares, opposite_color_occupied_squares);
        break;
    case W_ROOK_I:
    case B_ROOK_I:
        next_positions = get_pseudomoves_from_rook(piece_position, same_color_occupied_squares, opposite_color_occupied_squares);
        break;
    case W_KNIGHT_I:
    case B_KNIGHT_I:
        next_positions = get_pseudomoves_from_knight(piece_position, same_color_occupied_squares);
        break;
    case W_BISHOP_I:
    case W_QUEEN_I:
    case W_KING_I:
    case B_BISHOP_I:
    case B_QUEEN_I:
    case B_KING_I:
    default:
        fprintf(stderr, "Unreachable code reached at insert_pseudomoves_from_piece");
        exit(1);
    }
    insert_moves_into_array(move_array, piece_type, piece_position, next_positions);
}


MoveArray get_pseudomoves_from_board(Board* board)
{
    MoveArray move_array = create_move_array();
    size_t starting_index = board->turn == WHITE_TURN ? 0 : (N_PIECES / 2);
    size_t ending_index = starting_index + N_PIECES / 2;
    uint64_t same_color_occupied_squares = get_white_occupied_squares(board);
    uint64_t opposite_color_occupied_squares = get_black_occupied_squares(board);
    if (!board->turn == WHITE_TURN) {
        uint64_t temp = same_color_occupied_squares;
        same_color_occupied_squares = opposite_color_occupied_squares;
        opposite_color_occupied_squares = temp;
    }
    for (size_t i = starting_index; i < ending_index; ++i) {
        uint64_t* pieces_positions = get_pieces_positions(board->pieces[i]);
        int piece_count = count_bits(board->pieces[i]);
        for (size_t j = 0; j < (size_t) piece_count; ++j) {
            insert_pseudomoves_from_piece(board, &move_array, i, pieces_positions[j], same_color_occupied_squares, opposite_color_occupied_squares);
        }
        free(pieces_positions);
    }
    return move_array;
}
