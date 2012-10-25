#pragma once
#include "marcusrmStarbucks.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

marcusrmStarbucks :: marcusrmStarbucks(){
	this->locations = new vector<Entry*>();
	
	//need to randomize list first
	this->tree_head = NULL; //how to do this?
	
	this->build();



	//IMPORT data in file to buffer
	//BUILD buffer into tree
	//
}


marcusrmStarbucks :: ~marcusrmStarbucks(){
	
	burnTree(this->tree_head);
	this->tree_head = NULL;
}

void marcusrmStarbucks :: burnTree(Leaf* tree_head){

	//recursive stop case
	if(tree_head == NULL){
		return;
	}
	
	//recursive call delete on children
	burnTree(tree_head->leftChild);
	burnTree(tree_head->rightChild);
	
	//delete information on current node:
	delete &(tree_head->data->identifier);
	delete &(tree_head->data->x);
	delete &(tree_head->data->y);
	delete (tree_head->data);
	delete (tree_head);
	tree_head = NULL;

}

void importData(vector<Entry>* locations, string fileName){

	try{
		ifstream fid (fileName, ifstream::in);

		int index = 0;
		char* buffer = new char[256];
		Entry* temp = new Entry;

		while(!fid.eof()){//Brinkman says that this shouldn't work.

			fid.getline(buffer,256,',');
			temp->identifier = (string)buffer;

			fid.getline(buffer,256,',');
			temp->x  = atof(buffer); //NOTE: THIS MIGHT BE GIVING ME TOO MANY SIG FIGS

			fid.getline(buffer,256,'\r');
			temp->y  = atof(buffer); //NOTE: THIS MIGHT BE GIVING ME TOO MANY SIG FIGS

			locations->push_back(*temp);

			//checking output for correctness:
			//console() << (locations->back()).identifier << ","<< (locations->back()).x << ","<< (locations->back()).y << std::endl ;

			index++;
		}

		fid.close();

	}catch(ios_base::failure){

		cout << "Error Reading File\n";
		return;

	}
}

void marcusrmStarbucks :: build(){

	int n = (*(this->locations)).size();

	for(int i = 0; i < n; i++){
		if((*(this->locations)).at(i) != NULL){
			
			Leaf* nextLeaf = this->insert((*(this->locations)).at(i), this->tree_head, true);

			//Allocate space for the new Leaf's members

			Leaf* leftChild = new Leaf;
			Leaf* rightChild = new Leaf;
			Entry* data = new Entry;

			nextLeaf->leftChild = leftChild;
			nextLeaf->rightChild = rightChild;
			nextLeaf->data = data;

		}
	}

}

Leaf* marcusrmStarbucks :: insert(Entry* c, Leaf* head, bool xlevel){

	if(head == NULL)
		return head; //NEED TO ASSIGN NODE AT THIS POINT.

	if(xlevel){
		double difference = abs(head->data->x - c->x);

		if(difference > 0) 
			insert(c, head->rightChild, !xlevel);
		else
			insert(c, head->leftChild, !xlevel);
	}
	else{
		double difference = abs(head->data->y - c->y);

		if(difference > 0) 
			insert(c, head->rightChild, !xlevel);
		else
			insert(c, head->leftChild, !xlevel);

	}
	

}

Entry* marcusrmStarbucks :: getNearest(double x, double y){

	Leaf* solution = search(x, y, this->tree_head, true);

	return solution->data;
}

Leaf* marcusrmStarbucks :: search(double x, double y, Leaf* head, bool xlevel){

	if(head->leftChild == NULL || head->rightChild == NULL)
		return head;

	double distance = sqrt((head->data->x - x)*(head->data->x - x) + (head->data->y - y)*(head->data->y - y));

	if(distance < MARGIN)
		return head;

	Leaf* candidate;

	if(xlevel){
		if(x > head->data->x){
			candidate = search(x, y, head->rightChild, !xlevel);

			distance = sqrt((candidate->data->x - x)*(candidate->data->x - x) + (candidate->data->y - y)*(candidate->data->y - y));

			if(distance < abs(candidate->data->x - x))
				candidate = search(x, y, head->leftChild, !xlevel);
		}
		else{
			candidate = search(x, y, head->leftChild, !xlevel);

			distance = sqrt((candidate->data->x - x)*(candidate->data->x - x) + (candidate->data->y - y)*(candidate->data->y - y));

			if(distance < abs(candidate->data->x - x))
				candidate = search(x, y, head->rightChild, !xlevel);
		}
	}
	else{
		if(y > head->data->y){
			candidate = search(x, y, head->rightChild, !xlevel);

			distance = sqrt((candidate->data->x - x)*(candidate->data->x - x) + (candidate->data->y - y)*(candidate->data->y - y));

			if(distance < abs(candidate->data->y - y))
				candidate = search(x, y, head->leftChild, !xlevel);
		}
		else{
			candidate = search(x, y, head->leftChild, !xlevel);

			distance = sqrt((candidate->data->x - x)*(candidate->data->x - x) + (candidate->data->y - y)*(candidate->data->y - y));

			if(distance < abs(candidate->data->y - y))
				candidate = search(x, y, head->rightChild, !xlevel);
		}
	}

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Entry* marcusrmStarbucks :: getNearestSlow(double x, double y){

	int size = (*(this->locations)).size();
	double distance = 0 , minDistance = 10; // start minimum distance at 10, which is more than the maximum possible distance
	Entry* minEntry = (*(this->locations)).at(0);

	for(int i = 0; i < size; i++){

		Entry* current = (*(this->locations)).at(i);
		distance = sqrt((current->x - x)*(current->x - x) + (current->y - y)*(current->y - y));

		if(distance < minDistance){
			minDistance = distance;
			minEntry = current;
		}

	}

	return minEntry;
}