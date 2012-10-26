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
	this->tree_head = new Leaf();
	this->tree_head->data = new Entry;
	this->tree_head->leftChild = new Leaf();
	this->tree_head->rightChild = new Leaf();

}


marcusrmStarbucks :: ~marcusrmStarbucks(){
	
	burnTree(this->tree_head);
	this->tree_head = NULL;
}

void marcusrmStarbucks :: burnTree(Leaf* tree_head){

	//recursive stop case
	if(tree_head->data == NULL){
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

void marcusrmStarbucks :: importData(string fileName){

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
			
			(*(this->locations)).push_back(temp);

			//checking output for correctness:
			//console() << (locations->back())->identifier << ","<< (locations->back())->x << ","<< (locations->back())->y << std::endl ;

			index++;
		}

		fid.close();

	}catch(ios_base::failure){

		cout << "Error Reading File\n";
		return;

	}
}

void marcusrmStarbucks :: build(Entry* c, int n){

	n = (*(this->locations)).size();

	//set the head of the tree to the values of the first node:
	this->tree_head->data = (*(this->locations)).at(0);

	//now build the rest of the tree:
	for(int i = 1; i < n; i++){
			
		//print the nodes as they're being inserted so I can see randomness.
		//console() << (*(this->locations)).at(i)->identifier << ","<< (*(this->locations)).at(i)->x << ","<< (*(this->locations)).at(i)->y << endl;

		Leaf* nextLeaf = this->insert((*(this->locations)).at(i), this->tree_head, true);

		//Allocate space for the new Leaf's members

		//may be allocating this space incorrectly.
		//check to see if i can do it in the insert method so that the 
		//children can be assigned to null to signify ends of tree
		nextLeaf->data = (*(this->locations)).at(i);
		nextLeaf->leftChild = new Leaf();
		nextLeaf->rightChild = new Leaf();
		

	}

}

Leaf* marcusrmStarbucks :: insert(Entry* c, Leaf* head, bool xlevel){

	if(head->data == NULL)
		return head; //NEED TO ASSIGN NODE AT THIS POINT.

	if(xlevel){
	
		if(c->x > head->data->x) 
			insert(c, head->rightChild, !xlevel);
		else
			insert(c, head->leftChild, !xlevel);
	}
	else{

		if(c->y > head->data->y) 
			insert(c, head->rightChild, !xlevel);
		else
			insert(c, head->leftChild, !xlevel);

	}
	

}

void marcusrmStarbucks :: printInOrder(Leaf* head){
	
	if(head->data == NULL)
		return;
	printInOrder(head->leftChild);
	console() << head->data->identifier << ","<< head->data->x << ","<< head->data->y << endl;
	printInOrder(head->rightChild);


}

Entry* marcusrmStarbucks :: getNearest(double x, double y){

	Leaf* solution = search(x, y, this->tree_head, true);

	return solution->data;
}

Leaf* marcusrmStarbucks :: search(double x, double y, Leaf* head, bool xlevel){

	//this is probably a bad return case.
	if(head->data == NULL)
		return NULL;

	double distance = sqrt((head->data->x - x)*(head->data->x - x) + (head->data->y - y)*(head->data->y - y));

	if(distance < MARGIN)
		return head;

	Leaf* candidate = NULL;
//~~~~~~ X LEVEL ~~~~~~~~//
	if(xlevel){
		if(x > head->data->x){//~~~~~~ RIGHT SIDE ~~~~~~~~//
			candidate = search(x, y, head->rightChild, !xlevel);

			if(candidate == NULL)
				candidate = head;
			/*else{
				distance = sqrt((candidate->data->x - x)*(candidate->data->x - x) + (candidate->data->y - y)*(candidate->data->y - y));

				if(distance > abs(candidate->data->x - x))
					candidate = search(x, y, head->leftChild, !xlevel);
			}*/
		}
		else{//~~~~~~ LEFT SIDE ~~~~~~~~//
			candidate = search(x, y, head->leftChild, !xlevel);
			
			if(candidate == NULL)
				candidate = head;
			/*else{
				distance = sqrt((candidate->data->x - x)*(candidate->data->x - x) + (candidate->data->y - y)*(candidate->data->y - y));

				if(distance > abs(candidate->data->x - x))
					candidate = search(x, y, head->rightChild, !xlevel);
			}*/
		}
	}
//~~~~~~ Y LEVEL ~~~~~~~~//
	else{
		if(y > head->data->y){//~~~~~~ RIGHT SIDE ~~~~~~~~//
			candidate = search(x, y, head->rightChild, !xlevel);
						
			if(candidate == NULL)
				candidate = head;
			/*else{
				distance = sqrt((candidate->data->x - x)*(candidate->data->x - x) + (candidate->data->y - y)*(candidate->data->y - y));

				if(distance > abs(candidate->data->y - y))
					candidate = search(x, y, head->leftChild, !xlevel);
			}*/
		}
		else{//~~~~~~ LEFT SIDE ~~~~~~~~//
			candidate = search(x, y, head->leftChild, !xlevel);
						
			if(candidate == NULL)
				candidate = head;
			/*else{
				distance = sqrt((candidate->data->x - x)*(candidate->data->x - x) + (candidate->data->y - y)*(candidate->data->y - y));

				if(distance > abs(candidate->data->y - y))
					candidate = search(x, y, head->rightChild, !xlevel);
			}*/
		}
	}
	
	return candidate;

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