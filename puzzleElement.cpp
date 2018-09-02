/*
 * puzzlePiece.cpp
 *
 *  Created on: 21 ����� 2017
 *      Author: michal
 */

#include "puzzleElement.h"

puzzleElement :: puzzleElement(int elementNumber, int left, int top, int right, int bottom){
	elementId = elementNumber;
	elementEdges = new int[4];
	elementEdges[0] = left;
	elementEdges[1] = top;
	elementEdges[2] = right;
	elementEdges[3] = bottom;
	goodForCorner = new bool[4];
	goodForCorner[0] = (top==0&&left==0) ? true : false;
	goodForCorner[1] = (top==0&&right==0) ? true : false;
	goodForCorner[2] = (bottom==0&&right==0) ? true : false;
	goodForCorner[3] = (bottom==0&&left==0) ? true : false;
	placedOnBoard = false;
}

int puzzleElement :: getElementId() const{
	return elementId;
}
const int* puzzleElement :: getElementEdges() const{
	return elementEdges;
}

const bool* puzzleElement :: getGoodForCorner() const{
	return goodForCorner;
}

bool puzzleElement :: getPlacedOnBoard() const{
	return placedOnBoard;
}

void puzzleElement :: setPlacedOnBoard(bool x){
	placedOnBoard = x;
}

bool puzzleElement :: isSquare(){
	return (goodForCorner[0] && goodForCorner[1] && goodForCorner[2] && goodForCorner[3]);
}


puzzleElement :: ~puzzleElement(){
	delete[] goodForCorner;
	delete[] elementEdges;
}

void puzzleElement :: printElement(){
	cout << "elementId: " << endl;
	cout << elementId << endl;

	cout << "elementEdges: " << endl;
	for (int i = 0 ; i<4 ; i++)
		cout << elementEdges[i]<< endl;


}

