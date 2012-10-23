#pragma once
#include "marcusrmStarbucks.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

marcusrmStarbucks :: marcusrmStarbucks(vector<Entry*> locations){
	
	this->build(locations, locations.size());
	this->tree_head = NULL; //how to do this?


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
	if(tree_head == NULL)
		return;
	
	//recursive call delete on children
	burnTree(tree_head->leftChild);
	burnTree(tree_head->rightChild);
	
	//delete information on current node:
	delete (tree_head->data->identifier);
	delete (tree_head->data->x);
	delete (tree_head->data->y);
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

void marcusrmStarbucks :: build(vector<Entry*> locations, int n){

	for(int i = 0; i < n; i++){
		if(locations.at(i) != NULL){
			
			Leaf* nextLeaf = this->insert(locations.at(i), this->tree_head, true);

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



	return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Entry* marcusrmStarbucks :: getNearestSlow(double x, double y){


	return NULL;
}