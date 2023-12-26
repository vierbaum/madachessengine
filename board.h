#ifndef BOARD_H_
#define BOARD_H_

#include "magic_numbers.h"
#include <stdio.h>
#include <string.h>

#define U64 unsigned long long
#define countbits(board) __builtin_popcountll(board)
#define find_first_bit(board) __builtin_ffsll(board)
#define USE_32_BIT_MULTIPLICATIONS
#define BLACKPRINT "\x1b[30;41m"

static const U64 ON_A = 72340172838076673ULL;
static const U64 ON_B = 144680345676153346ULL;
static const U64 ON_H = 9259542123273814144ULL;
static const U64 ON_G = 4629771061636907072ULL;
static const char *UNICODE_PIECES[] = {"♙", "♘", "♗", "♖", "♕", "♔",
                                       "♟︎", "♞", "♝", "♜", "♛", "♚"};

static unsigned int random_state = 1804289383;

// square numbers
enum {
  A8, B8, C8, D8, E8, F8, G8, H8,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A1, B1, C1, D1, E1, F1, G1, H1, NOT_ON_BOARD
};

enum { WKC = 1, WQC = 2, BKC = 4, BQC = 8 };

enum { P, N, B, R, Q, K, p, n, b, r, q, k };

enum { ROOK, BISHOP };

enum { white, black, both };

static const int char_to_pieces[] = {
    ['P'] = P, ['N'] = N, ['B'] = B, ['R'] = R, ['Q'] = Q, ['K'] = K,
    ['p'] = p, ['n'] = n, ['b'] = b, ['r'] = r, ['q'] = q, ['k'] = k};

static const char *SQUARER[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "a7", "b7", "c7",
    "d7", "e7", "f7", "g7", "h7", "a6", "b6", "c6", "d6", "e6", "f6",
    "g6", "h6", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a4",
    "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a3", "b3", "c3", "d3",
    "e3", "f3", "g3", "h3", "a2", "b2", "c2", "d2", "e2", "f2", "g2",
    "h2", "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "NAN"};

static const char *PIECER = "PNBRQKpnbrqk";

static const int bishop_occ_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7,
    5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 7,
    7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6};

static const int rook_occ_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11, 12, 11, 11, 11, 11, 11, 11, 12};

static const int bit_table[64] = {
    63, 30, 3,  32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34,
    61, 29, 2,  51, 21, 43, 45, 10, 18, 47, 1,  54, 9,  57, 0,  35,
    62, 31, 40, 4,  49, 5,  52, 26, 60, 6,  23, 44, 46, 27, 56, 16,
    7,  39, 48, 24, 59, 14, 12, 55, 38, 28, 58, 20, 37, 17, 36, 8};

extern U64 pawn_attacks[2][64];
extern U64 knight_attacks[64];
extern U64 king_attacks[64];
extern U64 bishop_attacks[64][512];
extern U64 rook_attacks[64][4096];
extern U64 bishop_mask[64];
extern U64 rook_mask[64];

typedef struct {
  U64 bitboards[12];
  U64 occupancies[3];
  char enP;
  char stage;
  char castling;
  char side;
  int ply;
} Board;

U64 find_MN(int, int, int);

U64 gen_bishop_attack(int, U64);

U64 gen_bishop_occ(int);

U64 gen_king_attack(int);

U64 gen_knight_attack(int);

U64 gen_pawn_attack(int, char);

U64 gen_rook_attack(int, U64);

U64 gen_rook_occ(int);

static inline U64 get_bishop_attack(int pos, U64 occ) {
  // magic bitboards - do not touch
  occ &= bishop_mask[pos];
  occ *= bishop_MN[pos];
  occ >>= 64 - bishop_occ_bits[pos];
  return bishop_attacks[pos][occ];
}

static inline char get_bit(U64 board, int pos) {
  return (board & (1ULL << pos)) != 0;
}

static inline U64 get_rook_attacks(int pos, U64 occ) {
  // magic bitboards - do not touch
  occ &= rook_mask[pos];
  occ *= rook_MN[pos];
  occ >>= 64 - rook_occ_bits[pos];
  return rook_attacks[pos][occ];
}

static inline U64 get_queen_attack(int pos, U64 occ) {
  // calculate queen attacks using rook and bishop
  U64 bishop = 0;
  U64 occ1 = occ;
  occ &= bishop_mask[pos];
  occ *= bishop_MN[pos];
  occ >>= 64 - bishop_occ_bits[pos];
  bishop = bishop_attacks[pos][occ];

  occ1 &= rook_mask[pos];
  occ1 *= rook_MN[pos];
  occ1 >>= 64 - rook_occ_bits[pos];
  return rook_attacks[pos][occ1] | bishop;
}

static inline char is_attacked(int square, int c_side, Board *board) {
  // return if square is attacked
  if ((c_side == white) && (pawn_attacks[black][square] & board->bitboards[P]))
    return 1;
  if ((c_side == black) && (pawn_attacks[white][square] & board->bitboards[p]))
    return 1;

  if (knight_attacks[square] &
      ((c_side == white) ? board->bitboards[N] : board->bitboards[n]))
    return 1;

  if (get_bishop_attack(square, board->occupancies[both]) &
      ((c_side == white) ? board->bitboards[B] : board->bitboards[b]))
    return 1;

  if (get_rook_attacks(square, board->occupancies[both]) &
      ((c_side == white) ? board->bitboards[R] : board->bitboards[r]))
    return 1;

  if (get_queen_attack(square, board->occupancies[both]) &
      ((c_side == white) ? board->bitboards[Q] : board->bitboards[q]))
    return 1;

  if ((c_side == white) &&
      king_attacks[square] &
          ((c_side == white) ? board->bitboards[K] : board->bitboards[k]))
    return 1;
  if ((c_side == black) &&
      king_attacks[square] &
          ((c_side == white) ? board->bitboards[K] : board->bitboards[k]))
    return 1;
  // by default return false
  return 0;
}

void init_king_attacks();

void init_knight_attacks();

void init_magic_numbers();

void init_pawn_attacks();

void init_slider_attacks();

static inline int pop1st_bit(U64 *bb) {
  U64 b = *bb ^ (*bb - 1);
  unsigned int fold = (unsigned)((b & 0xffffffff) ^ (b >> 32));
  *bb &= (*bb - 1);
  return bit_table[(fold * 0x783a9b23) >> 26];
}

void print_attacked_squares(char, Board *);

void print_bit_board(U64);

void print_board(Board *);

void print_board_simple(Board *);

unsigned int random32();

U64 random64();

static inline char rem1st_bit(U64 *b) {
  int p = find_first_bit(*b) - 1;
  *b ^= (1ULL << p);
  return p;
}

static inline void remove_bit(U64 *board, int pos) { *board ^= (1ULL << pos); }

static inline U64 set_occupancy(int index, int bits, U64 m) {
  int i, j;
  U64 result = 0ULL;
  for (i = 0; i < bits; i++) {
    j = pop1st_bit(&m);
    if (index & (1 << i))
      result |= (1ULL << j);
  }
  return result;
}

void setup_from_fen(char *, Board *);

static inline void set_piece(U64 *board, int pos) { *board |= (1ULL << pos); }

#endif // BOARD_H_