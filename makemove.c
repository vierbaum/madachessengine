#include "makemove.h"
#include "board.h"

void print_move(int move) {
  printf("\nSOURCE TARGET PIEC PROM C D E C\n");
  for (int i = 0; i < 24; i++) {
    printf("%d", (move & (1U << i)) != 0);
    if (i == 5 || i == 11 || i == 15 || i >= 19)
      printf(" ");
  }
  printf("\n  %s     %s    %c\n", SQUARER[get_source(move)],
         SQUARER[get_target(move)], PIECER[get_piece(move)]);
  return;
}

void print_move_list(moves *list) {
  printf("count: %d\n", list->count);
  for (int i = 0; i < list->count; i++) {
    printf("%d ", i);
    print_move(list->moves[i]);
  }
}

void print_move_uci(int move, char *p) {
  // print move to char*
  *p = SQUARER[get_source(move)][0];
  *(p + 1) = SQUARER[get_source(move)][1];
  *(p + 2) = SQUARER[get_target(move)][0];
  *(p + 3) = SQUARER[get_target(move)][1];
  *(p + 4) = PROMOTED_PIECES[get_promoted(move)];
  *(p + 5) = '\0';
}
