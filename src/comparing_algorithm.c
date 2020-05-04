#include "comparing_algorithm.h"
#include <stdlib.h>

void LCS(const struct token_t *items1, const struct token_t *items2,
         unsigned count1, unsigned count2, size_t *matches_count) {
	if (count1 == 0 || count2 == 0)
		return;

	unsigned begIndex1 = 0, begIndex2 = 0;
	while (begIndex1 < count1 && begIndex2 < count2 && items1[begIndex1].type == items2[begIndex2].type) {
		++begIndex1;
		++begIndex2;
		(*matches_count) += 2;
	}
	unsigned endIndex1 = count1, endIndex2 = count2;
	while (endIndex1 > begIndex1 && endIndex2 > begIndex2 && items1[endIndex1 - 1].type == items2[endIndex2 - 1].type) {
		--endIndex1;
		--endIndex2;
		(*matches_count) += 2;
	}

	unsigned mtypedleCount1 = endIndex1 - begIndex1;
	unsigned mtypedleCount2 = endIndex2 - begIndex2;
	unsigned mtypedleCountProduct = mtypedleCount1 * mtypedleCount2;
	if (mtypedleCountProduct == 0)
		return;

	const unsigned insertWeight = 3;
	void* allocated = (unsigned *) malloc((2 * mtypedleCount1 + mtypedleCountProduct) * sizeof(unsigned));
	unsigned *lastRowWeights = allocated;

	unsigned *currRowWeights = lastRowWeights + mtypedleCount1;
	for (unsigned i = 0, weight = insertWeight; i < mtypedleCount1; ++i, weight += insertWeight) {
		lastRowWeights[i] = weight;
	}
	// 0x1 = diagonal, 0x2 = left, 0x4 = top
	unsigned *directions = currRowWeights + mtypedleCount1;

	for (unsigned y = 0; y < mtypedleCount2; ++y) {
		for (unsigned x = 0; x < mtypedleCount1; ++x) {
			unsigned weightFromLeft = x == 0
			                          ? (y + 2) * insertWeight
			                          : currRowWeights[x - 1] + insertWeight;
			unsigned weightFromTop = lastRowWeights[x] + insertWeight;

			unsigned weightFromDiagonal;
			if (items1[x + begIndex1].type == items2[y + begIndex2].type) {
				weightFromDiagonal = x == 0
				                     ? (y + 1) * insertWeight
				                     : lastRowWeights[x - 1];
			} else
				weightFromDiagonal = 0x7FFFFFFF;

			unsigned minWeight = weightFromLeft;
			if (weightFromTop < minWeight)
				minWeight = weightFromTop;
			if (weightFromDiagonal < minWeight)
				minWeight = weightFromDiagonal;

			currRowWeights[x] = minWeight;

			unsigned direction = 0;
			if (minWeight == weightFromDiagonal)
				direction |= 0x01U;
			if (minWeight == weightFromLeft)
				direction |= 0x02U;
			if (minWeight == weightFromTop)
				direction |= 0x04U;

			directions[y * mtypedleCount1 + x] = direction;
		}

		// Swap lastRowWeights and currRowWeights so now current becomes last.
		unsigned *tmp = lastRowWeights;
		lastRowWeights = currRowWeights;
		currRowWeights = tmp;
	}

	// Rewrite directions[y * mtypedleCount1 + x] to match1[i] and match2[i].
	int x = (int) mtypedleCount1 - 1;
	int y = (int) mtypedleCount2 - 1;
	unsigned index1 = endIndex1 - 1;
	unsigned index2 = endIndex2 - 1;
	while (x >= 0 && y >= 0) {
		unsigned direction = directions[y * mtypedleCount1 + x];
		if (direction & 0x01U) {
			--x;
			--y;
			(*matches_count) += 2;
		} else if (direction & 0x02U) {
			--index1;
			--x;
		} else // (direction & 0x04U)
		{
			--index2;
			--y;
		}
	}
	free(allocated);
}