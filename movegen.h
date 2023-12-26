#ifndef MOVEGEN_H_
#define MOVEGEN_H_

#include "board.h"
#include "makemove.h"

/*generate moves to move_list
RUN FORCING MOVES BEFORE TO AVOID MEMORY LEAKS, THIS IS INTENDED BEHAVIOUR*/
static inline void generate_moves(moves *move_list, Board *board) {
  char cp, np;
  U64 bb, attacks;

  if (board->side == white) {
    // white bishops
    bb = board->bitboards[B];
    while (bb) { // checking all bishops
      cp = rem1st_bit(&bb);
      attacks = get_bishop_attack(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if (!(board->occupancies[both] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, B, 0, 0, 0, 0, 0));
      }
    }

    // white knight
    bb = board->bitboards[N];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = knight_attacks[cp];
      while (attacks) {
        np = rem1st_bit(&attacks);
        if (!(board->occupancies[both] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, N, 0, 0, 0, 0, 0));
      }
    }

    // white pawn
    bb = board->bitboards[P];
    while (bb) {
      cp = rem1st_bit(&bb);

      np = cp - 8;
      if (!(board->occupancies[both] & (1ULL << np))) {
        if (np <= H8) { // promotions
          add_move(move_list, enc_move(cp, np, P, Q, 0, 0, 0, 0));
          add_move(move_list, enc_move(cp, np, P, R, 0, 0, 0, 0));
          add_move(move_list, enc_move(cp, np, P, N, 0, 0, 0, 0));
          add_move(move_list, enc_move(cp, np, P, B, 0, 0, 0, 0));
        } else // normal move
          add_move(move_list, enc_move(cp, np, P, 0, 0, 0, 0, 0));

        if (cp >= A2 && !(board->occupancies[both] &
                          (1ULL << (np - 8)))) // double pawn move
          add_move(move_list, enc_move(cp, (np - 8), P, 0, 0, 1, 0, 0));
      }
    }

    // white rook
    bb = board->bitboards[R];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = get_rook_attacks(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if (!(board->occupancies[both] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, R, 0, 0, 0, 0, 0));
      }
    }

    // white queen
    bb = board->bitboards[Q];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = get_bishop_attack(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if (!(board->occupancies[both] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, Q, 0, 0, 0, 0, 0));
      }
      attacks = get_rook_attacks(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if (!(board->occupancies[both] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, Q, 0, 0, 0, 0, 0));
      }
    }

    // white king
    bb = board->bitboards[K];
    cp = rem1st_bit(&bb);
    attacks = king_attacks[cp];
    while (attacks) {
      np = rem1st_bit(&attacks);
      if (!(board->occupancies[both] & (1ULL << np)))
        add_move(move_list, enc_move(cp, np, K, 0, 0, 0, 0, 0));
    }
    // TODO
    if (board->castling & WKC && !get_bit(board->occupancies[both], F1) &&
        !get_bit(board->occupancies[both], G1) &&
        !is_attacked(E1, black, board) && !is_attacked(F1, black, board))
      add_move(move_list, enc_move(E1, G1, K, 0, 0, 0, 0, 1));
    if (board->castling & WQC && !get_bit(board->occupancies[both], D1) &&
        !get_bit(board->occupancies[both], B1) &&
        !get_bit(board->occupancies[both], C1) &&
        !is_attacked(E1, black, board) && !is_attacked(D1, black, board) &&
        !is_attacked(C1, black, board))
      add_move(move_list, enc_move(E1, C1, K, 0, 0, 0, 0, 1));
  } else {

    // black bishops
    bb = board->bitboards[b];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = get_bishop_attack(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if (!(board->occupancies[both] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, b, 0, 0, 0, 0, 0));
      }
    }

    // black knight
    bb = board->bitboards[n];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = knight_attacks[cp];
      while (attacks) {
        np = rem1st_bit(&attacks);
        if (!(board->occupancies[both] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, n, 0, 0, 0, 0, 0));
      }
    }

    // black pawns
    bb = board->bitboards[p];
    while (bb) {
      cp = rem1st_bit(&bb);

      np = cp + 8;
      if (!(board->occupancies[both] & (1ULL << np))) {
        if (np >= A1) {
          add_move(move_list, enc_move(cp, np, p, q, 0, 0, 0, 0));
          add_move(move_list, enc_move(cp, np, p, r, 0, 0, 0, 0));
          add_move(move_list, enc_move(cp, np, p, n, 0, 0, 0, 0));
          add_move(move_list, enc_move(cp, np, p, b, 0, 0, 0, 0));
        } else
          add_move(move_list, enc_move(cp, np, p, 0, 0, 0, 0, 0));
        if (cp <= H7 && !(board->occupancies[both] & (1ULL << (np + 8))))
          add_move(move_list, enc_move(cp, (np + 8), p, 0, 0, 1, 0, 0));
      }
    }

    // black rook
    bb = board->bitboards[r];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = get_rook_attacks(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if (!(board->occupancies[both] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, r, 0, 0, 0, 0, 0));
      }
    }

    // black queen
    bb = board->bitboards[q];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = get_bishop_attack(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if (!(board->occupancies[both] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, q, 0, 0, 0, 0, 0));
      }
      attacks = get_rook_attacks(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if (!(board->occupancies[both] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, q, 0, 0, 0, 0, 0));
      }
    }

    // black king
    bb = board->bitboards[k];
    cp = rem1st_bit(&bb);
    attacks = king_attacks[cp];
    while (attacks) {
      np = rem1st_bit(&attacks);
      if (!(board->occupancies[both] & (1ULL << np)))
        add_move(move_list, enc_move(cp, np, k, 0, 0, 0, 0, 0));
    }
    if (board->castling & BKC && !get_bit(board->occupancies[both], F8) &&
        !get_bit(board->occupancies[both], G8) &&
        !is_attacked(E8, white, board) && !is_attacked(F8, white, board))
      add_move(move_list, enc_move(E8, G8, k, 0, 0, 0, 0, 1));
    if (board->castling & BQC && !get_bit(board->occupancies[both], D8) &&
        !get_bit(board->occupancies[both], B8) &&
        !get_bit(board->occupancies[both], C8) &&
        !is_attacked(D8, white, board) && !is_attacked(E8, white, board) &&
        !is_attacked(C8, white, board))
      add_move(move_list, enc_move(E8, C8, k, 0, 0, 0, 0, 1));
  }
  return;
}

// generate forcing moves
static inline void generate_forcing_moves(moves *move_list, Board *board) {
  move_list->count = 0;
  char cp, np;
  U64 bb, attacks;

  if (board->side == white) {
    // white pawn
    bb = board->bitboards[P];
    while (bb) {
      cp = rem1st_bit(&bb);
      // capture left
      if (board->occupancies[black] & (1ULL << (cp - 9)) &&
          !(ON_A & (1ULL << cp))) {
        if ((cp - 9) <= H8) { // promotions
          add_move(move_list, enc_move(cp, (cp - 9), P, Q, 1, 0, 0, 0));
          add_move(move_list, enc_move(cp, (cp - 9), P, R, 1, 0, 0, 0));
          add_move(move_list, enc_move(cp, (cp - 9), P, N, 1, 0, 0, 0));
          add_move(move_list, enc_move(cp, (cp - 9), P, B, 1, 0, 0, 0));
        } else
          add_move(move_list, enc_move(cp, (cp - 9), P, 0, 1, 0, 0, 0));
      }
      // capture right
      if (board->occupancies[black] & (1ULL << (cp - 7)) &&
          !(ON_H & (1ULL << cp))) {
        if ((cp - 7) <= H8) { // promotions
          add_move(move_list, enc_move(cp, (cp - 7), P, Q, 1, 0, 0, 0));
          add_move(move_list, enc_move(cp, (cp - 7), P, R, 1, 0, 0, 0));
          add_move(move_list, enc_move(cp, (cp - 7), P, N, 1, 0, 0, 0));
          add_move(move_list, enc_move(cp, (cp - 7), P, B, 1, 0, 0, 0));
        } else
          add_move(move_list, enc_move(cp, (cp - 7), P, 0, 1, 0, 0, 0));
      }

      if (board->enP != NOT_ON_BOARD) {
        if (board->enP == cp - 7 && !((1ULL << cp) & ON_H))
          add_move(move_list, enc_move(cp, (cp - 7), P, 0, 0, 0, 1, 0));
        if (board->enP == cp - 9 && !((1ULL << cp) & ON_A)) {
          add_move(move_list, enc_move(cp, (cp - 9), P, 0, 0, 0, 1, 0));
        }
      }
    }

    // white bishops
    bb = board->bitboards[B];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = get_bishop_attack(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if ((board->occupancies[black] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, B, 0, 1, 0, 0, 0));
      }
    }

    // white knight
    bb = board->bitboards[N];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = knight_attacks[cp];
      while (attacks) {
        np = rem1st_bit(&attacks);
        if ((board->occupancies[black] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, N, 0, 1, 0, 0, 0));
      }
    }

    // white rook
    bb = board->bitboards[R];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = get_rook_attacks(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if ((board->occupancies[black] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, R, 0, 1, 0, 0, 0));
      }
    }

    // white queen
    bb = board->bitboards[Q];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = get_bishop_attack(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if ((board->occupancies[black] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, Q, 0, 1, 0, 0, 0));
      }
      attacks = get_rook_attacks(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if ((board->occupancies[black] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, Q, 0, 1, 0, 0, 0));
      }
    }

    // white king
    bb = board->bitboards[K];
    cp = rem1st_bit(&bb);
    attacks = king_attacks[cp];
    while (attacks) {
      np = rem1st_bit(&attacks);
      if ((board->occupancies[black] & (1ULL << np)))
        add_move(move_list, enc_move(cp, np, K, 0, 1, 0, 0, 0));
    }
  } else {
    // black pawns
    bb = board->bitboards[p];
    while (bb) {
      cp = rem1st_bit(&bb);
      // capture left
      if (board->occupancies[white] & (1ULL << (cp + 7)) &&
          !(ON_A & (1ULL << cp))) {
        if ((cp + 7) >= A1) {
          add_move(move_list, enc_move(cp, (cp + 7), p, q, 1, 0, 0, 0));
          add_move(move_list, enc_move(cp, (cp + 7), p, r, 1, 0, 0, 0));
          add_move(move_list, enc_move(cp, (cp + 7), p, n, 1, 0, 0, 0));
          add_move(move_list, enc_move(cp, (cp + 7), p, b, 1, 0, 0, 0));
        } else
          add_move(move_list, enc_move(cp, (cp + 7), p, 0, 1, 0, 0, 0));
      }
      // capture left
      if (board->occupancies[white] & (1ULL << (cp + 9)) &&
          !(ON_H & (1ULL << cp))) {
        if ((cp + 9) >= A1) {
          add_move(move_list, enc_move(cp, (cp + 9), p, q, 1, 0, 0, 0));
          add_move(move_list, enc_move(cp, (cp + 9), p, r, 1, 0, 0, 0));
          add_move(move_list, enc_move(cp, (cp + 9), p, n, 1, 0, 0, 0));
          add_move(move_list, enc_move(cp, (cp + 9), p, b, 1, 0, 0, 0));
        } else
          add_move(move_list, enc_move(cp, (cp + 9), p, 0, 1, 0, 0, 0));
      }

      if (board->enP != NOT_ON_BOARD) {
        if (board->enP == cp + 9 && !((1ULL << cp) & ON_H))
          add_move(move_list, enc_move(cp, (cp + 9), p, 0, 0, 0, 1, 0));
        if (board->enP == cp + 7 && !((1ULL << cp) & ON_A))
          add_move(move_list, enc_move(cp, (cp + 7), p, 0, 0, 0, 1, 0));
      }
    }

    // black bishops
    bb = board->bitboards[b];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = get_bishop_attack(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if ((board->occupancies[white] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, b, 0, 1, 0, 0, 0));
      }
    }

    // black knight
    bb = board->bitboards[n];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = knight_attacks[cp];
      while (attacks) {
        np = rem1st_bit(&attacks);
        if ((board->occupancies[white] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, n, 0, 1, 0, 0, 0));
      }
    }

    // black rook
    bb = board->bitboards[r];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = get_rook_attacks(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if ((board->occupancies[white] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, r, 0, 1, 0, 0, 0));
      }
    }

    // black queen
    bb = board->bitboards[q];
    while (bb) {
      cp = rem1st_bit(&bb);
      attacks = get_bishop_attack(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if ((board->occupancies[white] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, q, 0, 1, 0, 0, 0));
      }
      attacks = get_rook_attacks(cp, board->occupancies[both]);
      while (attacks) {
        np = rem1st_bit(&attacks);
        if ((board->occupancies[white] & (1ULL << np)))
          add_move(move_list, enc_move(cp, np, q, 0, 1, 0, 0, 0));
      }
    }

    // black king
    bb = board->bitboards[k];
    cp = rem1st_bit(&bb);
    attacks = king_attacks[cp];
    while (attacks) {
      np = rem1st_bit(&attacks);
      if ((board->occupancies[white] & (1ULL << np)))
        add_move(move_list, enc_move(cp, np, k, 0, 1, 0, 0, 0));
    }
  }
  return;
}
#endif