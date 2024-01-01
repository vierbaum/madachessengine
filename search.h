#ifndef SEARCH_H_
#define SEARCH_H_
#include "board.h"
#include "eval.h"
#include "makemove.h"
#include "movegen.h"
#include <string.h>

static inline int quiesence(int alpha, int beta, Board* board) {
    int evaluation = eval_position(board);

    if (evaluation >= beta)
        return beta;
    if (evaluation > alpha)
        alpha = evaluation;

    moves movelist;
    generate_forcing_moves(&movelist, board);

    if (movelist.count == 0)
        return evaluation;

    Board cBoard;
    int score;
    for (int i = 0; i < movelist.count; i++) {
        cBoard = *board;
        if (make_move(movelist.moves[i], &cBoard)) {
            score = -quiesence(-beta, -alpha, &cBoard);
        }

        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    return alpha;
}

static inline int alphaBeta(Board* board, int alpha, int beta, int depthleft) {
    if (depthleft == 0)
        return eval_position(board);
    moves movelist;
    generate_forcing_moves(&movelist, board);
    generate_moves(&movelist, board);

    Board cb;
    int score;
    int legal_move;
    for (int i = 0; i < movelist.count; i++) {
        cb = *board;
        if (make_move(movelist.moves[i], &cb)) {
            legal_move = 1;
            score = -alphaBeta(&cb, -beta, -alpha, depthleft - 1);
            if (score >= beta)
                return beta; //  fail hard beta-cutoff
            if (score > alpha)
                alpha = score; // alpha acts like max in MiniMax
        }
    }
    // check-mate
    if (board->side == white && is_attacked(find_first_bit(board->bitboards[K]), black, board) && !legal_move)
        return -5000;
    if (board->side == black && is_attacked(find_first_bit(board->bitboards[k]), white, board) && !legal_move)
        return -5000;

    return alpha;
}

#endif