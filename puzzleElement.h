/*
 * puzzlePiece.h
 *
 *  Created on: 21 ����� 2017
 *      Author: michal
 */

#ifndef PUZZLEELEMENT_H_
#define PUZZLEELEMENT_H_
#include <iostream>

using namespace std;

class puzzleElement{
private:
	int elementId;
	int* elementEdges;
	bool* goodForCorner; //0-TL, 1-TR, 2-BR, 3-BL
	bool placedOnBoard;
public:
	puzzleElement(int elementNumber, int left, int top, int right, int bottom);
	bool isSquare();
	int getElementId() const;
	const int* getElementEdges() const;
	const bool* getGoodForCorner() const;
	bool getPlacedOnBoard() const;
	void setPlacedOnBoard(bool x);
	void printElement();

	~puzzleElement();

};




#endif /* PUZZLEELEMENT_H_ */
