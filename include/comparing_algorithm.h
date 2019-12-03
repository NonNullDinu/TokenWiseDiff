#ifndef COMP_ALGO_H
#define COMP_ALGO_H
#include "tokens.h"

void CalcCompare(
  int *match1, int *match2,
  const TOKEN *items1, const TOKEN *items2,
  unsigned count1, unsigned count2);

#endif