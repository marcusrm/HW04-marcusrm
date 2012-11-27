#pragma once
#include "marcusrmStarbucks.h"
//#include <boost/accumulators/accumulators.hpp>
//#include <boost/accumulators/statistics/stats.hpp>
//#include <boost/accumulators/statistics/median.hpp>

using namespace ci;
using namespace ci::app;
using namespace std;
//using namespace boost::accumulators;

marcusrmStarbucks :: marcusrmStarbucks(){
	//set up tree head
	this->tree_head = new Leaf();
	this->tree_head->data = new Entry;
	this->tree_head->leftChild = new Leaf();
	this->tree_head->rightChild = new Leaf();
	this->currentStarbucks = new Leaf();

	//don't want to make a list of entries yet. we don't know the data size
	//this->locations = NULL;
}


marcusrmStarbucks :: ~marcusrmStarbucks(){
	
	delete this->tree_head;
	tree_head = NULL;
}

int marcusrmStarbucks :: importData(Entry* importedData, string fileName){

	try{
		ifstream fid (fileName, ifstream::in);

		int index = 0;
		char* buffer = new char[256];
		
		while(!fid.eof()){//Brinkman says that this eof shouldn't work. (last bits)
			Entry* temp = new Entry;

			fid.getline(buffer,256,',');
			temp->identifier = (string)buffer;

			fid.getline(buffer,256,',');
			temp->x  = atof(buffer); //NOTE: THIS MIGHT BE GIVING ME TOO MANY SIG FIGS

			fid.getline(buffer,256,'\r');
			temp->y  = atof(buffer); //NOTE: THIS MIGHT BE GIVING ME TOO MANY SIG FIGS
			
			importedData[index] = *temp;

			//checking output for correctness:
			//console() << (locations->back())->identifier << ","<< (locations->back())->x << ","<< (locations->back())->y << std::endl ;

			index++;
		}

		//this->n = index;

		fid.close();

		return index;

	}catch(ios_base::failure){

		cout << "Error Reading File\n";
		return NULL;

	}
}

/*
int marcusrmStarbucks::findMedian(Entry* c, int n, bool xlevel){
	double median = 0;
	accumulator_set<double, stats<tag::median>> accX;
	accumulator_set<double, stats<tag::median>> accY;
	int minEntryIndex = -1;
	double minDifference = 10; // 10 is above highest possible distance;
	double difference = 0;

	for(int i = 0; i < n; i++){
		if(xlevel)
			accX(c[i].x);
		else
			accY(c[i].y);
	}

	if(xlevel)
		double median = boost::accumulators::median(accX);
	else
		double median = boost::accumulators::median(accY);

	for(int i = 0; i < n; i++){

		if(xlevel){
			difference = abs(c[i].x - median);
			if(difference < minDifference){
				minEntryIndex = i;
				minDifference = difference;
			}
		}
		else{
			difference = abs(c[i].y - median);
			if(difference < minDifference){
				minEntryIndex = i;
				minDifference = difference;
			}
		}
	}
	
	return minEntryIndex;
}
*/

void marcusrmStarbucks :: build(Entry* c, int n){

	
	//SHUFFLE METHOD

	//shuffle the locations array
	this->shuffle(c,n);

	//set the head of the tree to the values of the first node:
	this->tree_head->data = &c[0];

	//now build the rest of the tree:
	for(int i = 1; i < n; i++){
			
		//print the nodes as they're being inserted so I can see randomness.
		//console() << c[i].identifier << ","<< c[i].x << ","<< c[i].y << endl;

		Leaf* nextLeaf = this->insert(&c[i], this->tree_head, true);

		//Allocate space for the new Leaf's members & make sure tree is terminated with NULL leaves
		nextLeaf->data = &c[i];
		nextLeaf->leftChild = new Leaf();
		nextLeaf->rightChild = new Leaf();
		

	}
	

	//MEDIAN METHOD

	//insertMedian((this->tree_head), c, n, true);

}

//I found this method on an online tutorial website and slightly modified it
void marcusrmStarbucks::shuffle(Entry* c, int n){
       
	srand ( unsigned ( time (NULL) ) );
    int random;

	//swap each node with a random node in the array
    if(n>1){
            for(int i = 0; i < n; ++i){
                    random = rand() % n;
                    Entry temp = c[random];
                    c[random] = c[i];
                    c[i] = temp;
            }
    }
}

/*
void marcusrmStarbucks :: insertMedian(Leaf* head, Entry* c, int n, bool xlevel){
	
	//if we get down to a dataset of ONE node, then quit.
	if(n <= 1 || c->identifier == c[n].identifier)
		return;

	int medianIndex = findMedian(c,n,!xlevel);

	head->data = &c[medianIndex];
	head->leftChild = new Leaf();
	head->rightChild = new Leaf();
	insertMedian(head->leftChild, c, medianIndex, !xlevel);
	insertMedian(head->rightChild, &(c[medianIndex+1]), n-(medianIndex+1), !xlevel);

}
*/

Leaf* marcusrmStarbucks :: insert(Entry* c, Leaf* head, bool xlevel){

	if(head->data == NULL)
		return head; 

	if(xlevel){
	
		if(c->x > head->data->x) 
			return insert(c, head->rightChild, !xlevel);
		else
			return insert(c, head->leftChild, !xlevel);
	}
	else{

		if(c->y > head->data->y) 
			return insert(c, head->rightChild, !xlevel);
		else
			return insert(c, head->leftChild, !xlevel);

	}
	
	
}

void marcusrmStarbucks :: printInOrder(Leaf* head){
	
	if(head->data == NULL)
		return;
	printInOrder(head->leftChild);
	console() << head->data->identifier << "," << head->data->x << "," << head->data->y << endl;
	printInOrder(head->rightChild);


}

Entry* marcusrmStarbucks :: getNearest(double x, double y){

	Leaf* solution = search(x, y, this->tree_head, true);
	this->currentStarbucks = solution;

	return solution->data;
}


Leaf* marcusrmStarbucks :: search(double x, double y, Leaf* head, bool xlevel){

	if(head->data == NULL)
		return NULL;

	double parentDistance = sqrt((head->data->x - x)*(head->data->x - x) + (head->data->y - y)*(head->data->y - y));

	if(parentDistance < MARGIN)
		return head;

	bool rightSide = false;
	Leaf* candidate = NULL;
	Leaf* otherPath = NULL;

	
	if(xlevel){
		if(x > head->data->x){
			candidate = search(x, y, head->rightChild, !xlevel);
			otherPath = head->leftChild;
		}else{
			candidate = search(x, y, head->leftChild, !xlevel);
			otherPath = head->rightChild;
		}
	}else{
		if(y > head->data->y){
			candidate = search(x, y, head->rightChild, !xlevel);
			otherPath = head->leftChild;
		}else{
			candidate = search(x, y, head->leftChild, !xlevel);
			otherPath = head->rightChild;
		}
	}

	if(candidate == NULL){//~~~~~~CANDIDATE NULL~~~~~~//
		candidate = head;
		Leaf* twinCandidate = search(x, y, otherPath, !xlevel);

		if(twinCandidate != NULL){
			double twinDistance = sqrt((twinCandidate->data->x - x)*(twinCandidate->data->x - x) 
				+ (twinCandidate->data->y - y)*(twinCandidate->data->y - y));

			if(twinDistance < parentDistance)
				candidate = twinCandidate;
		}
	}
	else{//~~~~~~CANDIDATE NOT NULL~~~~~~//
		double candidateDistance = sqrt((candidate->data->x - x)*(candidate->data->x - x) 
					+ (candidate->data->y - y)*(candidate->data->y - y));

		if(parentDistance < candidateDistance){//~~~~~~~~~~~PARENT BETTER THAN CHILD~~~~~~~~~~~~~~//
			candidate = head;
			Leaf* twinCandidate = search(x, y, otherPath, !xlevel);

			if(twinCandidate != NULL){
				double twinDistance = sqrt((twinCandidate->data->x - x)*(twinCandidate->data->x - x) 
					+ (twinCandidate->data->y - y)*(twinCandidate->data->y - y));

				if(twinDistance < parentDistance)
					candidate = twinCandidate;
			}
		}
		else if(xlevel && candidateDistance > abs(head->data->x - x)){//~~~~~~CANDIDATE GREATER THAN BOUNDARY XLEVEL~~~~~~//
			Leaf* twinCandidate = search(x, y, otherPath, !xlevel);

			if(twinCandidate != NULL){//~~~~~~CHECK TWIN~~~~~~//
				double twinDistance = sqrt((twinCandidate->data->x - x)*(twinCandidate->data->x - x) 
					+ (twinCandidate->data->y - y)*(twinCandidate->data->y - y));

				if(twinDistance < candidateDistance)
					candidate = twinCandidate;
			}
		}
		else if(!xlevel && candidateDistance > abs(head->data->y - y)){//~~~~~~CANDIDATE GREATER THAN BOUNDARY YLEVEL~~~~~~//
			Leaf* twinCandidate = search(x, y, otherPath, !xlevel);

			if(twinCandidate != NULL){//~~~~~~CHECK TWIN~~~~~~//
				double twinDistance = sqrt((twinCandidate->data->x - x)*(twinCandidate->data->x - x) 
					+ (twinCandidate->data->y - y)*(twinCandidate->data->y - y));

				if(twinDistance < candidateDistance)
					candidate = twinCandidate;
			}
		}

	}
	
	return candidate;
	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Entry* marcusrmStarbucks :: getNearestSlow(Entry* c, int n, double x, double y){

	double distance = 0 , minDistance = 10; // start minimum distance at 10, which is more than the maximum possible distance
	Entry* minEntry = NULL;

	for(int i = 0; i < n; i++){

		Entry* current = &c[i];
		distance = sqrt((current->x - x)*(current->x - x) + (current->y - y)*(current->y - y));

		if(distance < minDistance){
			minDistance = distance;
			minEntry = current;
		}

	}

	return minEntry;
}

void marcusrmStarbucks::draw(int kAppWidth, int kAppHeight, uint8_t* pixels, Leaf* head){
	
	if(head->data == NULL)
		return;

	Vec2f coordinate = Vec2f(head->data->x * kAppWidth, kAppHeight-(head->data->y * kAppHeight));
	gl::color(head->color);
	gl::drawSolidCircle(coordinate, 1, 0);
	

	draw(kAppWidth, kAppHeight, pixels, head->rightChild);
	draw(kAppWidth, kAppHeight, pixels, head->leftChild);
}



