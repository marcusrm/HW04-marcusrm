#pragma once
#include "marcusrmStarbucks.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

marcusrmStarbucks :: marcusrmStarbucks(){
	//IMPORT data in file to buffer
	//BUILD buffer into tree
	//
}


marcusrmStarbucks :: ~marcusrmStarbucks(){
	//recursively delete all entries
	//free all pointers
	//set pointers to zero.
}

void importData(vector<Entry>* locations, string fileName){

	try{
		ifstream fid (fileName, ifstream::in);

		int index = 0;
		char* buffer = new char[256];
		Entry* temp = new Entry;

		while(!fid.eof()){

			fid.getline(buffer,256,',');
			temp->identifier = (string)buffer;

			fid.getline(buffer,256,',');
			temp->x  = atof(buffer); //NOTE: THIS MIGHT BE GIVING ME TOO MANY SIG FIGS

			fid.get(buffer,256,'\r');
			temp->y  = atof(buffer); //NOTE: THIS MIGHT BE GIVING ME TOO MANY SIG FIGS

			locations->push_back(*temp);

			console() << (locations->back()).identifier << ","<< (locations->back()).x << ","<< (locations->back()).y << std::endl ;

			index++;
		}

		fid.close();

	}catch(ios_base::failure){

		cout << "Error Reading File\n";
		return;

	}
}

void marcusrmStarbucks :: build(Entry* c, int n){

	for(int i = 0; i < n; i++){
		if(c != NULL)
			this->insert(c + n*sizeof(Entry));
	}

}

void marcusrmStarbucks :: insert(Entry* c){



}

Entry* marcusrmStarbucks :: getNearest(double x, double y){



	return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void marcusrmStarbucks :: insertSlow(Entry* current){


}

Entry* marcusrmStarbucks :: getNearestSlow(double x, double y){


	return NULL;
}