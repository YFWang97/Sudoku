#ifndef SOLVER_H
#define SOLVER_H

#include "defines.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>
#include <iterator>
#include <algorithm>

void solver(uint8_t* board);

typedef struct coordinate {
	uint8_t row;
	uint8_t col;

	uint8_t sqr_translate() {
		return row / 3 * 3 + col / 3;
	}

	bool operator==(const struct coordinate& coord) const {
		return (row == coord.row) && (col == coord.col);
	}

} Coordinate;

namespace std {
	template <>
	struct hash<struct coordinate> {
		size_t operator() (const struct coordinate& coord) const {
			return hash<uint8_t>()(coord.row) ^ hash<uint8_t>()(coord.col);
		}
	};
}

typedef struct info {
	Coordinate coord;
	unordered_set<uint8_t> availNums;
} Info;

#endif
