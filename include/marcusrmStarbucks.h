
#pragma once
#include "Starbucks.h"
#include <iostream>
#include <fstream>
#include <vector>

#define MARGIN 0.00001

//These are the nodes of my kd-tree
class Leaf { 
public:
	Entry* data;
	Leaf* leftChild;
	Leaf* rightChild;

	Leaf(){
		this->data = NULL;
	}
	
	~Leaf(){
		delete this->data;
		delete this->leftChild;
		delete this->rightChild;
		leftChild = NULL;
		rightChild = NULL;
	}
};


class marcusrmStarbucks : public Starbucks {
public:

	Leaf* tree_head;

	marcusrmStarbucks();
	~marcusrmStarbucks();

	virtual void build(Entry* c, int n);
	virtual Entry* getNearest(double x, double y);
	Entry* getNearestSlow(Entry* c, int n, double x, double y);
	void printInOrder(Leaf* head);

	//puts the imported data into 'importedData' array, and returns the size of that array
	int importData(Entry* importedData, string fileName);


private:
	
	Leaf* insert(Entry* c, Leaf* head, bool xlevel);
	Leaf* search(double x, double y, Leaf* head, bool xlevel);
	void shuffle(Entry* c, int n);
	static double findMedian(Entry* c, int n, bool xlevel);

};