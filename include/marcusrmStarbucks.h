
#pragma once
#include "Starbucks.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include <iostream>
#include <fstream>
#include <vector>

#define MARGIN 0.00001

using namespace ci;
using namespace ci::app;
using namespace std;

//These are the nodes of my kd-tree
class Leaf { 
public:
	Entry* data;
	Leaf* leftChild;
	Leaf* rightChild;
	Color color;
	Color popColor;
	int pop;
	int oldPop;

	Leaf(){
		this->data = NULL;
		this->color = Color(((double)rand())/RAND_MAX,((double)rand())/RAND_MAX,((double)rand())/RAND_MAX);
		this->popColor = Color(0,0,0);
		this->pop = 0;
		this->oldPop = 0;
	}
	
	~Leaf(){
		delete this->data;
		delete this->leftChild;
		delete this->rightChild;
		delete this->color;
		leftChild = NULL;
		rightChild = NULL;
	}
};


class marcusrmStarbucks : public Starbucks {
public:

	Leaf* tree_head;
	Leaf* currentStarbucks;

	marcusrmStarbucks();
	~marcusrmStarbucks();

	virtual void build(Entry* c, int n);
	virtual Entry* getNearest(double x, double y);
	Entry* getNearestSlow(Entry* c, int n, double x, double y);
	void printInOrder(Leaf* head);

	//puts the imported data into 'importedData' array, and returns the size of that array
	int importData(Entry* importedData, string fileName);

	void draw(int kAppWidth, int kAppHeight, uint8_t* pixels, Leaf* head);
	void drawCoverage(int kAppWidth, int kAppHeight);
	//void draw_circle(Surface* mySurface, int x, int y, int r);

private:
	
	Leaf* insert(Entry* c, Leaf* head, bool xlevel);
	//void insertMedian(Leaf* head, Entry* c, int n, bool xlevel);
	Leaf* search(double x, double y, Leaf* head, bool xlevel);
	static void shuffle(Entry* c, int n);
	//static int findMedian(Entry* c, int n, bool xlevel);
	

};