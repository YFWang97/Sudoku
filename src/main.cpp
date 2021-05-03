#include "defines.h" 
#include "solver.h"

bool checker(const uint8_t* board, const uint8_t* refBoard, bool acceptZero) {
	bool refCheck = (refBoard != NULL) ? 1 : 0;

	uint8_t num;
	uint8_t refNum;

	unordered_set<uint8_t> colSet[9];
	unordered_set<uint8_t> sqrSet[9];
	for (uint8_t row = 0; row < 9; row++) {
		unordered_set<uint8_t> rowSet;
		for (uint8_t col = 0; col < 9; col++) {
			if (refCheck) refNum = refBoard[row * 9 + col];			
			num = board[row * 9 + col];

			if (refCheck) {
				if (refNum != 0 && refNum != num)  return false;
			}

			if (acceptZero == false && num == 0) return false;
	
			if (acceptZero == true && num == 0) continue;

			if (rowSet.find(num) != rowSet.end()) return false; 
			if (colSet[col].find(num) != colSet[col].end()) return false;

			int sqrIndex = (Coordinate){row, col}.sqr_translate();
			if (sqrSet[sqrIndex].find(num) != sqrSet[sqrIndex].end()) return false;

			rowSet.insert(num);
			colSet[col].insert(num);
			sqrSet[sqrIndex].insert(num);			
		}
	}
	return true;
}

int main(int argc, char**argv) {
	string fileName;
	int totalNumOfBoards = -1;

	if (argc >= 2) {
		fileName = string(argv[1]);
	}

	if (argc >= 3) {
		totalNumOfBoards = stoi(string(argv[2]));
	}

	ifstream f(fileName);
	
	if (f.fail()) {
		cout << "Error: Board File Not Found" << endl;
		return 0;
	}

	uint8_t* board;
	uint8_t* refBoard;

	board = (uint8_t*) malloc(81 * sizeof(uint8_t));
	refBoard = (uint8_t*) malloc(81 * sizeof(uint8_t));

	int curIndexOfBoard = 0;
	int curRow = 0;
	int curCol = 0;

	while (!f.eof()) {
		string line;
		getline(f, line);

		if (line.size() == 0) {
			continue;
		} else {
			stringstream ss(line);
			string value;

			while (ss >> value) {
				board[curRow * 9 + curCol] = stoi(value);
				curCol++;
			}	

			curRow++;
			curCol = 0;
		}

		if (curRow == 9) {
			curRow = 0;
			cout << "NEW GAME: " << endl;
			for (int row = 0; row < 9; row++) {
				for (int col = 0; col < 9; col++) {
					cout << (int)board[row * 9 + col] << " ";
				}
				cout << endl;
			}
			memcpy(refBoard, board, 81 * sizeof(uint8_t));

			bool initBoardValid = checker(board, NULL, true);
	
			cout << "INIT BOARD VLD: " << initBoardValid << endl;

			solver(board);

			cout << "RESULT: " << endl;
			for (int row = 0; row < 9; row++) {
				for (int col = 0; col < 9; col++) {
					cout << (int)board[row * 9 + col] << " ";
				}
				cout << endl;
			}
		    bool valid = checker(board, refBoard, false);

			if (valid) cout << "VALID RESULT" << endl;
			else cout << "INVALID RESULT" << endl;

			cout << "-----------------" << endl;

			curIndexOfBoard++;
			if (curIndexOfBoard == totalNumOfBoards && totalNumOfBoards != -1) break;
		}
	}	

	f.close();
	return 0;
}
