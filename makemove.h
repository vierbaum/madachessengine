#ifndef MAKE_MOVE_H_
#define MAKE_MOVE_H_

#include "board.h"

// bits of flag
#define SOURCESQUARE 63 // = 0...0111111
#define TARGETSQUARE 4032
#define PIECE 61440
#define PROMOTED 983040
#define CAPTURE 1048576
#define DOUBLEPUSH 2097152
#define ENPASSANT 4194304
#define CASTLING 8388608

#define enc_move(source, target, piece, promoted, capture, double, enpassant,  \
                 castling)                                                     \
  (source) | (target << 6) | (piece << 12) | (promoted << 16) |                \
      (capture << 20) | (double << 21) | (enpassant << 22) | (castling << 23)
#define get_source(move) (move) & (SOURCESQUARE)
#define get_target(move) ((move) & (TARGETSQUARE)) >> 6
#define get_piece(move) ((move) & (PIECE)) >> 12
#define get_promoted(move) ((move) & (PROMOTED)) >> 16
#define get_capture(move) ((move) & (CAPTURE))
#define get_double(move) ((move) & (DOUBLEPUSH))
#define get_en_passant(move) ((move) & (ENPASSANT))
#define get_castling(move) ((move) & (CASTLING))

static const char PROMOTED_PIECES[] = {
    [Q] = 'q', [R] = 'r', [B] = 'b', [N] = 'n',
    [q] = 'q', [r] = 'r', [b] = 'b', [n] = 'n'};

static const int castline_rights[64] = {
    7,  15, 15, 15, 3,  15, 15, 11, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 13, 15, 15, 15, 12, 15, 15, 14};

typedef struct {
  int moves[1024];
  int count;
} moves;

static inline void add_move(moves *move_list, int move) {
  move_list->moves[move_list->count] = move;
  move_list->count++;
}

void print_move(int);

static inline int make_move(int move, Board *board) {
  int cp, np, piece, promotion, dpush, enPas;
  cp = get_source(move);
  np = get_target(move);
  piece = get_piece(move);
  promotion = get_promoted(move);
  dpush = get_double(move);
  enPas = get_en_passant(move);

  Board cBoard = *board;
  remove_bit(board->bitboards + piece, cp);

  if (get_capture(move)) {
    if (board->side == white)
      for (char cpiece = p; cpiece <= k;
           cpiece++) { // setting all blacks pieces to 0
        if (get_bit(board->bitboards[cpiece], np)) {
          remove_bit(board->bitboards + cpiece, np);
          break;
        }
      }

    else
      for (char cpiece = P; cpiece <= K; cpiece++) {
        if (get_bit(board->bitboards[cpiece], np)) {
          remove_bit(board->bitboards + cpiece, np);
          break;
        }
      }
  }

  if (promotion) { // set piece to promotion
    set_piece(board->bitboards + promotion, np);
  } else
    set_piece(board->bitboards + piece, np);

  if (enPas) // remove trailing pawn
    (board->side == white) ? remove_bit(board->bitboards + p, np + 8)
                           : remove_bit(board->bitboards + P, np - 8);

  board->enP = NOT_ON_BOARD; // resetting en passant

  board->ply++;

  if (dpush)
    (board->side == white) ? (board->enP = np + 8) : (board->enP = np - 8);

  if (get_castling(move)) {
    switch (np) {
    case (G1):
      remove_bit(board->bitboards + R, H1);
      set_piece(board->bitboards + R, F1);
      break;

    case (C1):
      remove_bit(board->bitboards + R, A1);
      set_piece(board->bitboards + R, D1);
      break;

    case (G8):
      remove_bit(board->bitboards + r, H8);
      set_piece(board->bitboards + r, F8);
      break;

    case (C8):
      remove_bit(board->bitboards + r, A8);
      set_piece(board->bitboards + r, D8);
      break;
    }
  }
  // updating castling
  board->castling &= castline_rights[cp];
  board->castling &= castline_rights[np];

  memset(board->occupancies, 0ULL, 24);

  // regenerating occupancies
  for (int bb_piece = P; bb_piece <= K; bb_piece++)
    board->occupancies[white] |= board->bitboards[bb_piece];

  for (int bb_piece = p; bb_piece <= k; bb_piece++)
    board->occupancies[black] |= board->bitboards[bb_piece];

  board->occupancies[both] |= board->occupancies[white];
  board->occupancies[both] |= board->occupancies[black];

  board->side ^= 1;

  if (is_attacked((board->side == white)
                      ? find_first_bit(board->bitboards[k]) - 1
                      : find_first_bit(board->bitboards[K]) - 1,
                  board->side, board)) {
    // return illegal move
    *board = cBoard;
    return 0;
  }
  return 1;
}

void print_move_list(moves *);

void print_move_uci(int, char *);

#endif