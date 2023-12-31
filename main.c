/*
copyright (C) 2022-2023 Deniz Cetin & Nick Daiber

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. 
*/

#include "board.h"
#include <stdio.h>
#include "makemove.h"
#include "movegen.h"
#include "perf.h"
#include "eval.h"
#include "search.h"

// attack tables
U64 pawn_attacks[2][64];
U64 knight_attacks[64];
U64 king_attacks[64];
U64 bishop_attacks[64][512];
U64 rook_attacks[64][4096];
U64 bishop_mask[64];
U64 rook_mask[64];
U64 bitboards[12];
U64 occupancies[3];

void init_all();


int main(int argc, char *argv[]) {
  init_all();

  //test-fen
  char s[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  Board b;
  setup_from_fen(s, &b);

  printf("%d\n", alphaBeta(&b, -1000, 1000, 10));
  return 0;
}

void init_all() {
  init_pawn_attacks();
  init_knight_attacks();
  init_king_attacks();
  init_slider_attacks();

  // clearing bitboards
  for (int i = 0; i <= k; i++) bitboards[i] = 0ULL;
}