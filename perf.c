#include "perf.h"
#include "board.h"
#include "makemove.h"
#include "movegen.h"

U64 perf_head(Board* b, int depth) {
  if (depth == 0)
    return 1;
  U64 count = 0;
  moves movelist;
  generate_forcing_moves(&movelist, b);
  generate_moves(&movelist, b);

  Board cb;
  U64 nodes;
  char move_uci[64];
  for (int i = 0; i < movelist.count; i++) {
    cb = *b;

    if(make_move(movelist.moves[i], &cb)) {
      print_move_uci(movelist.moves[i], move_uci);

      nodes = perf(&cb, depth - 1);

      printf("%s: %llu\n", move_uci, nodes);
      count += nodes;

    }
  }
  printf("sum: %llu\n", count);
  return count;
}

U64 perf(Board *b, int depth) {
  if (depth == 0)
    return 1;
  U64 count = 0;
  moves movelist;
  generate_forcing_moves(&movelist, b);
  generate_moves(&movelist, b);

  Board cb;
  for (int i = 0; i < movelist.count; i++) {
    cb = *b;

    if(make_move(movelist.moves[i], &cb))
      count += perf(&cb, depth - 1);
  }
  return count;
}

int perf_test() {
  char* boardfen[] = {
  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ", 
  "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 
  "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 
  "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 
  "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 
  "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 "};

  U64 pos_values[][8] = {
    {1, 20, 400, 8902, 197281, 4865609, 119060324, 3195901860},
    {1, 48, 2039, 97862, 4085603, 193690690, 0, 0},
    {1, 14, 191, 2812, 43238, 674624, 11030083, 178633661},
    {1, 6, 264, 9467, 422333, 15833292, 706045033, 0},
    {1, 44, 1486, 62379, 2103487, 89941194, 0, 0},
    {1, 46, 2079, 89890, 3894594, 164075551, 6923051137, 287188994746}
  };

  Board b;
  for (int pos = 0; pos <= 5; pos++) {
    printf("starting test %d\n", pos);
    setup_from_fen(boardfen[pos], &b);

    for (int i =0; i <= 7; i++) {
      if (pos_values[pos][i]) {
        if (perf(&b, i) != pos_values[pos][i]) {
          printf("FAILED POSITION %d DEPTH %d\n", pos, i);
          return 1;
        }
        else
          printf("passed depth %d\n", i);

        }
    }
  }
  return 0;
}