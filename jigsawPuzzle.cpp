/*
 * jigsawPuzzle.cpp
 *
 *  Created on: 22 ����� 2017
 *      Author: michal
 */



#include "jigsawPuzzle.h"


jigsawPuzzle :: jigsawPuzzle(int elementsNum, puzzleElement** puzzleElements){
	numOfElements = elementsNum;
	numOfDivisors = findNumOfDivisors();
	divisors = findDivisors();
	elements = puzzleElements;
	leftStraightEdges = 0, topStraightEdges = 0, rightStraightEdges = 0, bottomStraightEdges = 0;
	countStraightEdges();

}

int jigsawPuzzle :: findNumOfDivisors(){
	int root = floor(sqrt(numOfElements));
	int cnt=0;

	for(int i=1; i<=root; i++){
		if(numOfElements%i == 0)
			cnt++;
	}

	return cnt;
}

int** jigsawPuzzle :: findDivisors(){
	int root = floor(sqrt(numOfElements));
	int index;
	int** divisors;


	divisors = new int*[numOfDivisors];
	for(int i=0; i<numOfDivisors; i++)
		divisors[i] = new int[2];

	index=0;
	for(int i=1; i<=root; i++){
		if(numOfElements%i == 0){
			divisors[index][0] = i;
			divisors[index][1] = numOfElements/i;
			index++;
		}
	}

	return divisors;
}

void jigsawPuzzle :: countStraightEdges(){
	for (int i=0; i<numOfElements; i++){
		leftStraightEdges += (elements[i]->getElementEdges()[0]==0) ? 1 : 0;
		topStraightEdges += (elements[i]->getElementEdges()[1]==0) ? 1 : 0;
		rightStraightEdges += (elements[i]->getElementEdges()[2]==0) ? 1 : 0;
		bottomStraightEdges += (elements[i]->getElementEdges()[3]==0) ? 1 : 0;
	}

}

int jigsawPuzzle :: getNumOfElements() const{
	return numOfElements;
}
int jigsawPuzzle :: getNumOfDivisors() const{
	return numOfDivisors;
}
int** jigsawPuzzle :: getDivisors() const{
	return divisors;
}

puzzleElement** jigsawPuzzle :: getElements() const{
	return elements;
}

int** jigsawPuzzle :: solvePuzzle(int& height, int& width){
	int n, m;
	int res;

	int** solvedPuzzle;

	for(int i=0; i<numOfDivisors; i++){
		n = divisors[i][0];
		m = divisors[i][1];
		//cout << n << " " << m << endl;

		//cout << leftStraightEdges << ", " << topStraightEdges << ", " << rightStraightEdges << ", " << bottomStraightEdges << endl;

		if (n <= leftStraightEdges && n <= rightStraightEdges && m <= topStraightEdges && m <= bottomStraightEdges){
			solvedPuzzle = createPuzzle(n, m);
			res = solvePuzzleRec(0 ,0 , n, m, solvedPuzzle);
			if (res == 1){
				height = n;
				width = m;
				return solvedPuzzle;
			}
			destroyPuzzle(solvedPuzzle, n);
		}
		if (m <= leftStraightEdges && m <= rightStraightEdges && n <= topStraightEdges && n <= bottomStraightEdges){

			solvedPuzzle = createPuzzle(m, n);
			res = solvePuzzleRec(0 ,0 , m, n, solvedPuzzle);
			if (res == 1){
				height = m;
				width = n;
				return solvedPuzzle;
			}
			destroyPuzzle(solvedPuzzle, m);
		}
	}

	return NULL;
}



int jigsawPuzzle :: solvePuzzleRec(int row, int col, int numOfRows, int numOfColumns, int** solvedPuzzle){
	int lefttAdj, topAdj, rightAdj, bottomAdj;
	int res;
	//int numOfValidOptions;

	if (row == numOfRows)
		return 1;

	lefttAdj = (col == 0) ? 0 :  elements[solvedPuzzle[row][col-1]-1]->getElementEdges()[2];
	topAdj = (row == 0) ? 0 : elements[solvedPuzzle[row-1][col]-1]->getElementEdges()[3];
	rightAdj = (col == numOfColumns-1) ? 0 : -2;
	bottomAdj = (row == numOfRows-1) ? 0 : -2;

	//cout << row << " " << col << endl;
	//cout << lefttAdj << ", " << topAdj << ", " << rightAdj << ", " << bottomAdj << endl;

	//numOfValidOptions =0;

	for (int i=0; i<numOfElements; i++){
		if (!elements[i]->getPlacedOnBoard()){
			//cout << "id: " << elements[i]->getElementId() << ", "<< elements[i]->getElementEdges()[0] << ", " << elements[i]->getElementEdges()[1] << ", " << elements[i]->getElementEdges()[2] << ", " << elements[i]->getElementEdges()[3] << endl;

			//numOfValidOptions++;
			if ((elements[i]->getElementEdges()[0] + lefttAdj == 0) && (elements[i]->getElementEdges()[1] + topAdj == 0) &&
						(rightAdj == -2 || elements[i]->getElementEdges()[2] + rightAdj == 0) &&
							(bottomAdj == -2 || elements[i]->getElementEdges()[3] + bottomAdj == 0)){
				solvedPuzzle[row][col] = elements[i]->getElementId();
				elements[i]->setPlacedOnBoard(true);
				if (col == numOfColumns-1)
					res = solvePuzzleRec(row+1, 0, numOfRows, numOfColumns, solvedPuzzle);
				else
					res = solvePuzzleRec(row, col+1, numOfRows, numOfColumns, solvedPuzzle);

				if (res == 1)
					return 1;

				solvedPuzzle[row][col] = -1;
				elements[i]->setPlacedOnBoard(false);
			}
		}
	}

	//if (res == 0 || numOfValidOptions == 0)
	return 0;

}

int** createPuzzle(int numOfRows, int numOfColumns){
	int** solvedPuzzle;

	solvedPuzzle = new int*[numOfRows];
	for(int i=0; i<numOfRows; i++){
		solvedPuzzle[i] = new int[numOfColumns];
	}

	for(int i=0; i<numOfRows; i++){
		for (int j=0; j< numOfColumns; j++)
			solvedPuzzle[i][j] = -1;
	}

	return solvedPuzzle;
}

void destroyPuzzle(int** puzzle, int numOfRows){
	for(int i = 0; i < numOfRows; i++)
	  delete[] puzzle[i];
	delete[] puzzle;
}

bool jigsawPuzzle :: wrongNumOfStraightEdges(){
	//bool hasTLCorner = false, hasTRCorner = false, hasBRCorner = false, hasBLCorner = false;
	if (leftStraightEdges!=rightStraightEdges || topStraightEdges!=bottomStraightEdges)
		return true;
	return false;
}

bool jigsawPuzzle :: notEnoughStraightEdgesForFrame(){
	if ((divisors[numOfDivisors-1][0] + divisors[numOfDivisors-1][1]<=topStraightEdges)*2 <= (leftStraightEdges+rightStraightEdges+topStraightEdges+bottomStraightEdges))
		return false;
	return true;
}

bool* jigsawPuzzle :: missingCornerElementForRowOrColumnShapedPuzzle(){
	bool* missingCorners = new bool[4];
	fill_n(missingCorners, 4, true);
	int numOfSquares = 0, numOfTops = 0, numOfLefts = 0, numOfRights = 0, numOfBottoms = 0;
	for (int i=0; i<numOfElements; i++){
		if (elements[i]->isSquare())
			numOfSquares++;
		else if (elements[i]->getGoodForCorner()[0]&&elements[i]->getGoodForCorner()[3]){
			numOfLefts++;
			missingCorners[0] = missingCorners[3] = false;
		}
		else if (elements[i]->getGoodForCorner()[0]&&elements[i]->getGoodForCorner()[1]){
			numOfTops++;
			missingCorners[0] = missingCorners[1] = false;
		}
		else if (elements[i]->getGoodForCorner()[2]&&elements[i]->getGoodForCorner()[3]){
			numOfBottoms++;
			missingCorners[2] = missingCorners[3] = false;
		}
		else if (elements[i]->getGoodForCorner()[1]&&elements[i]->getGoodForCorner()[2]){
			numOfRights++;
			missingCorners[1] = missingCorners[2] = false;
		}
	}
	if (numOfSquares>=2 || (numOfSquares==1 && (numOfLefts!=0 || numOfRights!=0 || numOfTops!=0 || numOfBottoms!=0)))
		fill_n(missingCorners, 4, false);
	else if (numOfSquares==1)
		missingCorners[1] = missingCorners[2] = missingCorners[3] = false;
	return 	missingCorners;

}

void checkEmptyCornerArray(int& size, int* arr){
	if (size==0){
		arr[0] = 0;
		size++;
	}
}


void jigsawPuzzle :: missingCornerElementForRegularPuzzle(bool* missingCorners){
	int i, j, k, l, m, cnt;
	bool updatedCorners = false;
	bool** missingCornersNumber = new bool*[4];
	for (m=0; m<4; m++){
		missingCornersNumber[m] = NULL;
	}
	int *TLCorners = new int[numOfElements];
	int *TRCorners = new int[numOfElements];
	int *BRCorners = new int[numOfElements];
	int *BLCorners = new int[numOfElements];
	int TLCornersIndex = 0, TRCornersIndex = 0, BRCornersIndex = 0, BLCornersIndex = 0;
	for (i=0; i<numOfElements; i++){
		if (elements[i]->getGoodForCorner()[0]){
			TLCorners[TLCornersIndex] = elements[i]->getElementId();
			TLCornersIndex++;
		}
		if (elements[i]->getGoodForCorner()[1]){
			TRCorners[TRCornersIndex] = elements[i]->getElementId();
			TRCornersIndex++;
		}
		if (elements[i]->getGoodForCorner()[2]){
			BRCorners[BRCornersIndex] = elements[i]->getElementId();
			BRCornersIndex++;
		}
		if (elements[i]->getGoodForCorner()[3]){
			BLCorners[BLCornersIndex] = elements[i]->getElementId();
			BLCornersIndex++;
		}
	}
	checkEmptyCornerArray(TLCornersIndex, TLCorners);
	checkEmptyCornerArray(TRCornersIndex, TRCorners);
	checkEmptyCornerArray(BRCornersIndex, BRCorners);
	checkEmptyCornerArray(BLCornersIndex, BLCorners);

	for (i=0; i<TLCornersIndex; i++){
		if (TLCorners[i]!=0)
			missingCorners[0] = false;
		else
			missingCorners[0] = true;
		for (j=0; j<TRCornersIndex; j++){
			if (TRCorners[j]!=0 && TRCorners[j]!=TLCorners[i])
				missingCorners[1] = false;
			else
				missingCorners[1] = true;
			for (k=0; k<BRCornersIndex; k++){
				if (BRCorners[k]!=0 && BRCorners[k]!=TRCorners[j] && BRCorners[k]!=TLCorners[i])
					missingCorners[2] = false;
				else
					missingCorners[2] = true;
				for (l=0; l<BLCornersIndex; l++){
					if (BLCorners[l]!=0 && BLCorners[l]!=BRCorners[k] && BLCorners[l]!=TRCorners[j] && BLCorners[l]!=TLCorners[i])
						missingCorners[3] = false;
					else
						missingCorners[3] = true;

					cnt = (missingCorners[0]==false) + (missingCorners[1]==false) + (missingCorners[2]==false) + (missingCorners[3]==false);
					if (cnt==4){
						for (m=0; m<4; m++){
							if (missingCornersNumber[m]!=NULL)
								delete[] missingCornersNumber[m];
						}
						delete[] missingCornersNumber;
						delete[] TLCorners;
						delete[] TRCorners;
						delete[] BRCorners;
						delete[] BLCorners;
						return;
					}
					if (missingCornersNumber[cnt]==NULL){
						missingCornersNumber[cnt] = new bool[4];
						missingCornersNumber[cnt][0] = missingCorners[0];
						missingCornersNumber[cnt][1] = missingCorners[1];
						missingCornersNumber[cnt][2] = missingCorners[2];
						missingCornersNumber[cnt][3] = missingCorners[3];
					}
				}
			}
		}
	}
	for (m=3; m>=0; m--){
		if (missingCornersNumber[m]!=NULL){
			if (!updatedCorners){
				missingCorners[0] = missingCornersNumber[m][0];
				missingCorners[1] = missingCornersNumber[m][1];
				missingCorners[2] = missingCornersNumber[m][2];
				missingCorners[3] = missingCornersNumber[m][3];
				updatedCorners = true;
			}
			delete[] missingCornersNumber[m];
		}
	}
	delete[] missingCornersNumber;
	delete[] TLCorners;
	delete[] TRCorners;
	delete[] BRCorners;
	delete[] BLCorners;
	return;
}

bool* jigsawPuzzle :: missingCornerElement(bool* missingCorners, bool& hasMissingCornersOutOfFour){
	bool* missingCornersRowOrColumn;
	if (numOfDivisors==1)
		return 	missingCornerElementForRowOrColumnShapedPuzzle();
	else{
		missingCornerElementForRegularPuzzle(missingCorners);
		if (missingCorners[0]==false && missingCorners[1]==false && missingCorners[2]==false && missingCorners[3]==false)
			return missingCorners;
		else{
			missingCornersRowOrColumn = missingCornerElementForRowOrColumnShapedPuzzle();
			if (missingCornersRowOrColumn[0]==false && missingCornersRowOrColumn[1]==false && missingCornersRowOrColumn[2]==false && missingCornersRowOrColumn[3]==false){
				hasMissingCornersOutOfFour = true;
				return missingCornersRowOrColumn;
			}
			else
				delete[] missingCornersRowOrColumn;
		}
	}
	return 	missingCorners;
}

bool jigsawPuzzle :: sumOfEdgesNotZero(){
	int i, j, sum = 0;
	for (i=0; i<numOfElements; i++){
		for (j=0; j<4; j++){
			sum += elements[i]->getElementEdges()[j];
		}
	}
	return sum!=0;
}

jigsawPuzzle :: ~jigsawPuzzle(){
	//deleting divisors
	for(int i = 0; i < numOfDivisors; i++)
		delete[] divisors[i];
	delete[] divisors;

	//deleting elements
	for(int i = 0; i < numOfElements; i++)
		elements[i]->~puzzleElement();
	delete[] elements;

}


