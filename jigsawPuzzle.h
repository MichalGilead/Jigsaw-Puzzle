/*
 * jigsawPuzzle.h
 *
 *  Created on: 22 ����� 2017
 *      Author: michal
 */


#include "puzzleElement.h"
#include <iostream>
#include <cmath>
using namespace std;
#include <iostream>


#ifndef JIGSAWPUZZLE_H_
#define JIGSAWPUZZLE_H_

class jigsawPuzzle{
private:
	int numOfElements; //number of pieces in the puzzle
	int** divisors;
	int numOfDivisors;
	puzzleElement** elements; //array of all pieces
	int leftStraightEdges;
	int topStraightEdges;
	int rightStraightEdges;
	int bottomStraightEdges;
	void countStraightEdges();
	int findNumOfDivisors();
	int** findDivisors();
	bool* missingCornerElementForRowOrColumnShapedPuzzle();
	void missingCornerElementForRegularPuzzle(bool* missingCorners);


public:
	jigsawPuzzle(int elementsNum, puzzleElement **puzzleElements);
	bool wrongNumOfStraightEdges();
	bool notEnoughStraightEdgesForFrame();
	bool* missingCornerElement(bool* missingCorners, bool& hasFourMissingCorners);
	bool sumOfEdgesNotZero();
	int** solvePuzzle(int& height, int& width);
	int  solvePuzzleRec(int row, int col, int numOfRows, int numOfColumns, int** solvedPuzzle);

	int getNumOfDivisors() const;
	int** getDivisors() const;
	int getNumOfElements() const;
	puzzleElement** getElements() const;


	~jigsawPuzzle();

};

int** createPuzzle(int numOfRows, int numOfColumns);
void destroyPuzzle(int** puzzle, int numOfRows);


#endif /* JIGSAWPUZZLE_H_ */
