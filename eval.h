#ifndef EVAL_H_
#define EVAL_H_ // test
#include "board.h"

enum {opening, midgame, endgame};

extern int pawn_table[3][64];
extern int knight_table[3][64];
extern int bishop_table[3][64];
extern int rook_table[3][64];
extern int queen_table[3][64];
extern int king_table[3][64];
extern char reverse_lookup[64];

#define QUEEN_VALUE 900
#define ROOK_VALUE 500
#define BISHOP_VALUE 300
#define KNIGHT_VALUE 300
#define PAWN_VALUE 100

static inline int eval_position(Board *board) {
  int game_stage = 0;
  int eval = 0;
  while (board->bitboards[P])
    eval += pawn_table[game_stage][rem1st_bit(board->bitboards + P)];
  while (board->bitboards[p])
    eval -= pawn_table[game_stage][reverse_lookup[rem1st_bit(board->bitboards + p)]];

  while (board->bitboards[N])
    eval += knight_table[game_stage][rem1st_bit(board->bitboards + N)];
  while (board->bitboards[n])
    eval -= knight_table[game_stage][reverse_lookup[rem1st_bit(board->bitboards + n)]];

  while (board->bitboards[B])
    eval += bishop_table[game_stage][rem1st_bit(board->bitboards + B)];
  while (board->bitboards[b])
    eval -= bishop_table[game_stage][reverse_lookup[rem1st_bit(board->bitboards + b)]];

  while (board->bitboards[R])
    eval += rook_table[game_stage][rem1st_bit(board->bitboards + R)];
  while (board->bitboards[r])
    eval -= rook_table[game_stage][reverse_lookup[rem1st_bit(board->bitboards + r)]];

  while (board->bitboards[Q])
    eval += queen_table[game_stage][rem1st_bit(board->bitboards + Q)];
  while (board->bitboards[q])
    eval -= queen_table[game_stage][reverse_lookup[rem1st_bit(board->bitboards + q)]];

  return (board->side == white) ? eval: -eval;
}
#endif