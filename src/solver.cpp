#include "defines.h"
#include "solver.h"
vector<pair<Coordinate, uint8_t>> gCombinationVec;
vector<pair<Coordinate, uint8_t>> gFinalCombinationVec;
vector<Info> gInfoVector;

unordered_set<uint8_t> rowNumSetArry[9];
unordered_set<uint8_t> colNumSetArry[9];
unordered_set<uint8_t> sqrNumSetArry[9];

bool backtracking() {
	Info newInfo = gInfoVector.back();
	gInfoVector.pop_back();
	Coordinate newCoord = newInfo.coord;
	
	for (auto& num : newInfo.availNums) {

		if ((rowNumSetArry[newCoord.row].find(num) != rowNumSetArry[newCoord.row].end()) &&
			(colNumSetArry[newCoord.col].find(num) != colNumSetArry[newCoord.col].end()) &&
			(sqrNumSetArry[newCoord.sqr_translate()].find(num) != sqrNumSetArry[newCoord.sqr_translate()].end())) {
			
			gCombinationVec.push_back(make_pair(newCoord, num));

			if (gInfoVector.size() == 0) {
				gFinalCombinationVec = gCombinationVec;
				return true; 
			}

			rowNumSetArry[newCoord.row].erase(num);
			colNumSetArry[newCoord.col].erase(num);
			sqrNumSetArry[newCoord.sqr_translate()].erase(num);

			bool result = backtracking();

			if (result == true) return true;

			//If not found a result from this branch
			//Restore the information

			gCombinationVec.pop_back();
			rowNumSetArry[newCoord.row].insert(num);
			colNumSetArry[newCoord.col].insert(num);
			sqrNumSetArry[newCoord.sqr_translate()].insert(num);
		}
	}

	gInfoVector.push_back(newInfo);
	return false;	
}

void solver(uint8_t* board) {
	gInfoVector.clear();
	gCombinationVec.clear();
	gFinalCombinationVec.clear();

	/* Vector of info for each row, col and square region
	 * Initial info would be only one that contains all availble coords and available numbers
	 * In the calculating process, we can split the info to more information sets 
	 * to be more precise
	 */

	unordered_set<uint8_t> refNumSet = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	for (int i = 0; i < 9; i++) {
		rowNumSetArry[i] = refNumSet;
		colNumSetArry[i] = refNumSet;
		sqrNumSetArry[i] = refNumSet;
	}

	for (uint8_t row = 0; row < 9; row++) {
		for (uint8_t col = 0; col < 9; col++) {
			uint8_t num = board[row * 9 + col];
			Coordinate curCoord = {row, col};
			if (num != 0) {
				if (rowNumSetArry[row].find(num) != rowNumSetArry[row].end()) {
					rowNumSetArry[row].erase(num);
				}

				if (colNumSetArry[col].find(num) != colNumSetArry[col].end()) {
					colNumSetArry[col].erase(num);
				}

				uint8_t	sqrIndex = curCoord.sqr_translate();
				if (sqrNumSetArry[sqrIndex].find(num) != sqrNumSetArry[sqrIndex].end()) {
					sqrNumSetArry[sqrIndex].erase(num);
				}
			}
		}
	}

	/* Break down the information sets by cross check
	 * Get available numbers for each coord
	 */
	
	for (uint8_t row = 0; row < 9; row++) {
		for (uint8_t col = 0; col < 9; col++) {
			uint8_t num = board[row * 9 + col];
			//Collect the information for open coord
			if (num == 0) {
				Coordinate coord = {row, col};
				uint8_t sqrIndex = coord.sqr_translate();

				unordered_set<uint8_t> rowColIntersectionSet;
				unordered_set<uint8_t> numSet;

				for (auto& numFromRow : rowNumSetArry[row]) {
					if (colNumSetArry[col].find(numFromRow) != colNumSetArry[col].end()) {
						rowColIntersectionSet.insert(numFromRow);
					}	
				}

				for (auto& numFromIntersection : rowColIntersectionSet) {
					if (sqrNumSetArry[sqrIndex].find(numFromIntersection) != sqrNumSetArry[sqrIndex].end()) {
						numSet.insert(numFromIntersection);
					}
				}

				//If only has one intersection, it has to be the number
				//Else push in the gInfoVec to further backtracking
				if (numSet.size() == 1) {
					gCombinationVec.push_back(make_pair(coord, *numSet.begin()));
				} else {
					gInfoVector.push_back((Info){coord, numSet});
				}
			}
		}
	}

	//for (auto & comb : gCombinationVec) {
	//	cout << (int)comb.first.row << "," << (int)comb.first.col << " : " << (int)comb.second << endl;
	//}
	//
	//cout << " ========" << endl;

	//for (auto & info : gInfoVector) {
	//	cout << (int)info.coord.row << "," << (int)info.coord.col << " : ";
	//	for (auto & num : info.availNums) {
	//		cout << (int)num << " ";
	//	}
	//	cout << endl;
	//}

	for (auto & comb : gCombinationVec) {
		uint8_t row = comb.first.row;
		uint8_t col = comb.first.col;
		uint8_t num = comb.second;
		uint8_t sqr = (Coordinate){row, col}.sqr_translate();
		if (rowNumSetArry[row].find(num) != rowNumSetArry[row].end()) rowNumSetArry[row].erase(num);
		if (colNumSetArry[col].find(num) != colNumSetArry[col].end()) colNumSetArry[col].erase(num);
		if (sqrNumSetArry[sqr].find(num) != sqrNumSetArry[sqr].end()) sqrNumSetArry[sqr].erase(num);
	}

	//If no more information needed to back track stop here.
	if (gInfoVector.size() == 0) {
		for (auto& comb : gFinalCombinationVec) {
			board[comb.first.row * 9 + comb.first.col] = comb.second;
		}
	} else {
		if (backtracking()) {
			//cout << "FOUND" << endl;
			for (auto& comb : gFinalCombinationVec) {
				board[comb.first.row * 9 + comb.first.col] = comb.second;
			}
		} else {
			//cout << "NOT FOUND" << endl;
		}
	}
}
