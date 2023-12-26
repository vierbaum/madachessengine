#ifndef PERF_H_
#define PERF_H_
#include "board.h"
#include "movegen.h"

U64 perf(Board*, int);

U64 perf_head(Board*, int);

int perf_test();

#endif