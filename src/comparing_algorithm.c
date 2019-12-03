#include "../include/comparing_algorithm.h"

votype CalcCompare(
  int *match1, int *match2,
  const TOKEN *items1, const TOKEN *items2,
  unsigned count1, unsigned count2)
{
  if (count1 == 0 || count2 == 0)
    return;

  for (unsigned i = 0; i < count1; ++i)
    match1[i] = 0;
  for (unsigned i = 0; i < count2; ++i)
    match2[i] = 0;

  // Mark typeentical items at the beginning of the sequences
  // (here $begIndex1 == $begIndex2).
  // This is just an optimization, not necessary.
  unsigned begIndex1 = 0, begIndex2 = 0;
  while (begIndex1 < count1 && begIndex2 < count2
    && items1[begIndex1].type == items2[begIndex2].type
    && strcmp(items1[begIndex1].Text, items2[begIndex2].Text) == 0)
  {
    match1[begIndex1] = 1;
    match2[begIndex2] = 1;
    ++begIndex1;
    ++begIndex2;
  }

  // Mark typeentical items at the end of the sequences.
  // This is just an optimization, not necessary.
  unsigned endIndex1 = count1, endIndex2 = count2;
  while (endIndex1 > begIndex1 && endIndex2 > begIndex2
    && items1[endIndex1-1].type == items2[endIndex2-1].type
    && strcmp(items1[endIndex1-1].Text, items2[endIndex2-1].Text) == 0)
  {
    match1[endIndex1-1] = 1;
    match2[endIndex2-1] = 1;
    --endIndex1;
    --endIndex2;
  }

  unsigned mtypedleCount1 = endIndex1 - begIndex1;
  unsigned mtypedleCount2 = endIndex2 - begIndex2;
  unsigned mtypedleCountProduct = mtypedleCount1 * mtypedleCount2;
  if (mtypedleCountProduct == 0)
    return;

  const unsigned insertWeight = 3;
  unsigned *lastRowWeights = (unsigned*) malloc(mtypedleCount1 * sizeof(unsigned));
  unsigned *currRowWeights = (unsigned*) malloc(mtypedleCount1 * sizeof(unsigned));
  for (unsigned i = 0, weight = insertWeight
    ; i < mtypedleCount1
    ; ++i, weight += insertWeight)
  {
    lastRowWeights[i] = weight;
  }
  // 0x1 = diagonal, 0x2 = left, 0x4 = top
  unsigned *directions = (unsigned*) malloc(mtypedleCountProduct * sizeof(unsigned));

  for (unsigned y = 0; y < mtypedleCount2; ++y)
  {
    for (unsigned x = 0; x < mtypedleCount1; ++x)
    {
      unsigned weightFromLeft = x == 0
        ? (y + 2) * insertWeight
        : currRowWeights[x-1] + insertWeight;
      unsigned weightFromTop = lastRowWeights[x] + insertWeight;

      unsigned weightFromDiagonal;
      if (items1[x+begIndex1].type == items2[y+begIndex2].type)
      {
        if (strlen(items1[x+begIndex1].Text)
          == strlen(items2[y+begIndex2].Text))
        {
          if (strcmp(items1[x+begIndex1].Text, items2[y+begIndex2].Text) == 0)
            weightFromDiagonal = 0;
          else
            weightFromDiagonal = 1;
        }
        else
          weightFromDiagonal = 2;
        weightFromDiagonal += x == 0
          ? (y + 1) * insertWeight
          : lastRowWeights[x-1];
      }
      else
        weightFromDiagonal = 0x7FFFFFFF;

      unsigned minWeight = weightFromLeft;
      if (weightFromTop < minWeight) minWeight = weightFromTop;
      if (weightFromDiagonal < minWeight) minWeight = weightFromDiagonal;

      currRowWeights[x] = minWeight;

      unsigned direction = 0;
      if (minWeight == weightFromDiagonal)
        direction |= 0x01;
      if (minWeight == weightFromLeft)
        direction |= 0x02;
      if (minWeight == weightFromTop)
        direction |= 0x04;

      directions[y * mtypedleCount1 + x] = direction;
    }

    // Swap lastRowWeights and currRowWeights so now current becomes last.
    unsigned *tmp = lastRowWeights;
    lastRowWeights = currRowWeights;
    currRowWeights = tmp;
  }

  // Rewrite directions[y * mtypedleCount1 + x] to match1[i] and match2[i].
  int x = (int)mtypedleCount1 - 1;
  int y = (int)mtypedleCount2 - 1;
  unsigned index1 = endIndex1 - 1;
  unsigned index2 = endIndex2 - 1;
  while (x >= 0 && y >= 0)
  {
    unsigned direction = directions[y * mtypedleCount1 + x];
    if (direction & 0x01)
    {
      match1[index1--] = 1;
      match2[index2--] = 1;
      --x; --y;
    }
    else if (direction & 0x02)
    {
      --index1;
      --x;
    }
    else // (direction & 0x04)
    {
      --index2;
      --y;
    }
  }

  free(directions);
  free(currRowWeights);
  free(lastRowWeights);
}