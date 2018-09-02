/*

 * main.cpp
 *
 *  Created on: 21 бреб„ 2017
 *      Author: michal
*/


#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "puzzleElement.h"
#include "jigsawPuzzle.h"


bool isValidEdge(const string& str){
	if (str=="0"||str=="1"||str=="-1")
		return true;
	return false;
}

bool isPositiveNum(const string& str){
	for(unsigned int i=0; i< str.size(); i++)
		if ((i==0 && str[i]==48) || (str[i]<48||str[i]>57))
			return false;
	return true;
}

void writeSolutionToOutputFile(ofstream& fout, int** solvedPuzzle, int height, int width){
	if (solvedPuzzle==NULL)
		fout << "Cannot solve puzzle: it seems that there is no proper solution" << endl;
	else{
		for (int i=0; i<height; i++){
			for (int j=0; j<width; j++){
				fout << solvedPuzzle[i][j];
				if (j!=width-1)
					fout << " ";
			}
			fout << endl;
		}
	}
}

bool checkMissingCorners(ofstream& fout, bool* missingCorners){
	bool res = false;
	string corners[4] = {"TL", "TR", "BR", "BL"};
	if (missingCorners!=NULL){
		for (int i=0; i<4; i++){
			if (missingCorners[i]){
				fout << "Cannot solve puzzle: missing corner element: " << corners[i] << endl;
				res = true;
			}
		}
		delete[] missingCorners;
	}
	return res;
}

bool writeUnsolvableToOutputFile(ofstream& fout, jigsawPuzzle* puzzle, bool* missingCorners, bool& hasMissingCornersOutOfFour){
	bool unsolvable = false, *solution;
	if (puzzle->wrongNumOfStraightEdges()){
		fout << "Cannot solve puzzle: wrong number of straight edges" << endl;
		unsolvable = true;
	}
	if (puzzle->notEnoughStraightEdgesForFrame()){
		fout << "Cannot solve puzzle: not enough straight edges for frame" << endl;
		unsolvable = true;
	}
	solution = puzzle->missingCornerElement(missingCorners, hasMissingCornersOutOfFour);
	if (checkMissingCorners(fout, solution))
		unsolvable = true;

	if (puzzle->sumOfEdgesNotZero()){
		if (hasMissingCornersOutOfFour)
			checkMissingCorners(fout, missingCorners);
		fout << "Cannot solve puzzle: sum of edges is not zero" << endl;
		unsolvable = true;
	}
	return unsolvable;
}


jigsawPuzzle* readFromInputFile(ifstream& fin, ofstream& fout){
	string line, tmp, tmp2, IdsOutOfRangeError, invalidIdsError;
	bool hasIdsOutOfRange = false, hasInvalidId = false, invalidRead = false;
	stringstream ss, ss2;
	int i, j, numOfElements = 0, elementId = 0, left, top, right, bottom, validElements = 0, numOfGivenElements = 0;
	getline(fin, line);
	i=0;
	ss << line;
	while (ss >> tmp){
		if ((i==0 && tmp != "NumElements") || (i==1 && tmp != "=") || (i==2 && !isPositiveNum(tmp)) || (i>2)){
			j=0;
			if (i==0){
				ss2 << tmp;
				while (getline(ss2, tmp2, '=')){
					if ((j==0 && tmp2 != "NumElements") || (j==2 && !isPositiveNum(tmp2))){
						cout << "Wrong format of first line of arguments" << endl;
						return NULL;
					}
					else if (j==1)
						numOfElements = stoi(tmp2);
					j++;
				}
			}
			else{
				cout << "Wrong format of first line of arguments" << endl;
				return NULL;
			}
		}
		i++;
	}
	if (numOfElements==0)
		numOfElements = stoi(tmp);

	puzzleElement **elements = new puzzleElement*[numOfElements];
	for (i=0; i<numOfElements; i++)
		elements[i] = NULL;
	bool *givenElements = new bool[numOfElements];
	fill_n(givenElements, numOfElements, false);
	string *wrongElements = new string[numOfElements];
	fill_n(wrongElements, numOfElements, "");

	while (getline(fin, line)){
		ss.str("");
		ss.clear();
		ss << line;
		invalidRead = false;
		i=0;
		while (getline(ss, tmp, ' ')){
			if (i==0){
				if (!isPositiveNum(tmp)){
					if (hasInvalidId){
						invalidIdsError.append(", ");
						invalidIdsError.append(tmp);
					}
					else{
						hasInvalidId = true;
						invalidIdsError = "Puzzle cannot have the following IDs: ";
						invalidIdsError.append(tmp);
					}
					invalidRead = true;
					break;
				}
				else if (stoi(tmp)>numOfElements){
					if (hasIdsOutOfRange){
						IdsOutOfRangeError.append(", ");
						IdsOutOfRangeError.append(tmp);
					}
					else{
						hasIdsOutOfRange = true;
						IdsOutOfRangeError = "Puzzle of size ";
						IdsOutOfRangeError.append(to_string(numOfElements));
						IdsOutOfRangeError.append(" cannot have the following IDs: ");
						IdsOutOfRangeError.append(tmp);
					}
					invalidRead = true;
					break;
				}
				else{
					elementId = stoi(tmp);
					if (!givenElements[elementId-1]){
						givenElements[elementId-1]= true;
						numOfGivenElements++;
					}
					else{
						fout << "Puzzle ID " << elementId << " was given more than once" << endl;
						invalidRead = true;
						break;
					}
				}
			}
			else{
				if (i>4 || !isValidEdge(tmp)){
					wrongElements[elementId-1].append(line);
					invalidRead = true;
					break;
				}
				left = (i==1) ? stoi(tmp) : left;
				top = (i==2) ? stoi(tmp) : top;
				right = (i==3) ? stoi(tmp) : right;
				bottom = (i==4) ? stoi(tmp) : bottom;
			}

			i++;
			if (invalidRead){
				break;
			}
		}

		if (i>0 && !invalidRead){
			if (i<5)
				wrongElements[elementId-1].append(line);
			else{
				elements[elementId-1] = new puzzleElement(elementId, left, top, right, bottom);
				validElements++;
			}
		}
	}

	if (numOfGivenElements==numOfElements && !hasIdsOutOfRange && !hasInvalidId && validElements==numOfElements){
		delete[] givenElements;
		delete[] wrongElements;
		jigsawPuzzle* puzzle = new jigsawPuzzle(numOfElements, elements);
		return puzzle;
	}

	if (numOfGivenElements < numOfElements){
		fout << "Missing puzzle element(s) with the following IDs: ";
		for (i=0; i<numOfElements; i++){
			if (!givenElements[i]){
				numOfGivenElements++;
				if (numOfGivenElements < numOfElements){
					fout << i+1 << ", ";
				}
				else
					fout << i+1 << endl;
			}
		}
	}
	if (hasIdsOutOfRange)
		fout << IdsOutOfRangeError << endl;
	if (hasInvalidId)
		fout << invalidIdsError << endl;
	if (validElements!=numOfElements){
		for (i=0; i<numOfElements; i++){
			if (wrongElements[i]!=""){
				fout << "Puzzle ID " << i+1 << " has wrong data: " << wrongElements[i] << endl;
			}
		}
	}

	delete[] givenElements;
	delete[] wrongElements;
	for (i=0; i<numOfElements; i++){
		if (elements[i]!=NULL)
			delete elements[i];
	}
	delete[] elements;
	return NULL;
}

bool handleOneElement(ofstream& fout, jigsawPuzzle* puzzle){
	if (puzzle->getNumOfElements() > 1)
		return false;

	if (puzzle->getElements()[0]->isSquare())
		fout << "1" << endl;

	else {
		if (puzzle->wrongNumOfStraightEdges())
			fout << "Cannot solve puzzle: wrong number of straight edges" << endl;
		if (puzzle->notEnoughStraightEdgesForFrame())
			fout << "Cannot solve puzzle: not enough straight edges for frame" << endl;


		if (!puzzle->getElements()[0]->getGoodForCorner()[0])
			fout << "Cannot solve puzzle: missing corner element: TL" << endl;
		else if (!puzzle->getElements()[0]->getGoodForCorner()[1])
			fout << "Cannot solve puzzle: missing corner element: TR" << endl;
		else if (!puzzle->getElements()[0]->getGoodForCorner()[2])
			fout << "Cannot solve puzzle: missing corner element: BR" << endl;
		else if (!puzzle->getElements()[0]->getGoodForCorner()[3])
			fout << "Cannot solve puzzle: missing corner element: BL" << endl;
		if (puzzle->sumOfEdgesNotZero())
			fout << "Cannot solve puzzle: sum of edges is not zero" << endl;
	}
	return true;


}

int main (int argc, char **argv){
	bool checkOneElement;
	if (argc < 3)
	{
		cout << "Usage: main was not given 2 arguments." << endl;
		return 1;
	}
	int solutionWidth, solutionHeight, **solution;
	bool* missingCorners = new bool[4];
	fill_n(missingCorners, 4, true);
	bool hasMissingCornersOutOfFour = false;
	ifstream fin(argv[1]);
	if (!fin.is_open()){
		cout << "Usage: could not open input file." << endl;
		return 1;
	}
	ofstream fout(argv[2]);
	jigsawPuzzle* puzzle = readFromInputFile(fin, fout);
	fin.close();
	if (puzzle == NULL){
		if (missingCorners!=NULL&&hasMissingCornersOutOfFour)
			checkMissingCorners(fout, missingCorners);
		fout.close();
		return 1;
	}

	checkOneElement = handleOneElement(fout, puzzle);
	if (checkOneElement){
		delete puzzle;
		fout.close();
		return 0;
	}
	if (writeUnsolvableToOutputFile(fout, puzzle, missingCorners, hasMissingCornersOutOfFour)){
		delete puzzle;
		fout.close();
		return 1;
	}
	solution = puzzle->solvePuzzle(solutionHeight, solutionWidth);
	writeSolutionToOutputFile(fout, solution, solutionHeight, solutionWidth);
	delete puzzle;
	fout.close();
	return 0;
}

