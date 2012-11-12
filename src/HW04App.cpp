/*
* author: RJ Marcus
* file: HW04App.cpp
* date: Nov 1, 2012
* class: CSE274--Dr. Brinkman
* 
*
*	This program satisfies requirements: A, B, C, E, F, G
*
*	The program takes about 15 seconds to start up. Don't worry, it'll load.
*
*	For the coverage map for change in population density:
*
	//the lighter the color is, the bigger the population change. 
	//if a region seems dark, then it had relatively little change.
	//large increase: green
	//small/medium increase: teal
	//small/medium decrease: purple
	//large decrease: red
	//zero change: grey
*/


#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "marcusrmStarbucks.h"
#include "cinder/ImageIo.h"
#include "Resources.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

struct censusPoint{
	double x;
	double y;
	int pop;
};

class HW04App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* settings);
	void keyDown(KeyEvent event);
	void drawCoverage();
	void importCensusData(vector<censusPoint>* census, string filename);
	void assignColor(Leaf* head);

	Surface* starbucksSurface;
	Surface* coverageSurface;
	Surface* censusSurface;
	Surface* us_map;

	//Width and height of the screen
	static const int kAppWidth=500;
	static const int kAppHeight=500;
	static const int kTextureSize=1024; //Must be the next power of 2 bigger or equal to app dimensions

	vector<Leaf*>* coverageMatrix;
	marcusrmStarbucks* myStarbucks;
	Entry* importedStarbucksData;
	int starbucksDataSize;
	Entry* nearest;
	double x;
	double y;


	bool isCoverageOn;
	bool showNearest;

};

void HW04App::prepareSettings(Settings* settings){
	(*settings).setWindowSize(kAppWidth,kAppHeight);
	(*settings).setResizable(false);
}

void HW04App::setup()
{
	
	vector<censusPoint>* census2000 = new vector<censusPoint>();
	vector<censusPoint>* census2010 = new vector<censusPoint>();

	starbucksSurface = new Surface(kAppWidth,kAppHeight,false);
	coverageSurface = new Surface(kAppWidth,kAppHeight,false);
	censusSurface = new Surface(kAppWidth,kAppHeight,false);
	us_map = new Surface(loadImage(loadResource(US_MAP)));

	importedStarbucksData = new Entry[10000];

	isCoverageOn = false;
	showNearest = false;

	myStarbucks = new marcusrmStarbucks();

	starbucksDataSize = myStarbucks->importData(importedStarbucksData, "../src/Starbucks_2006.csv");

	myStarbucks->build(importedStarbucksData,starbucksDataSize);

	//for each pixel on the screen, find the closest starbucks and put
	//a pointer to it at that index of the coverageMatrix.
	coverageMatrix = new vector<Leaf*>();
	for(int j = 0; j < kAppHeight; j++){
		for(int i = 0; i < kAppWidth; i++){
			myStarbucks->getNearest(((double)i)/kAppWidth, 1 - ((double)j)/kAppHeight);
			coverageMatrix->push_back(myStarbucks->currentStarbucks);
		}
	}


	importCensusData(census2000,"../src/Census_2000.csv");
	importCensusData(census2010,"../src/Census_2010.csv");

	int xCoord;
	int yCoord;
	Leaf* current;

	//take the 2010 population for each city in the census, 
	//look up the closest starbucks at that location in the 
	//coveragematrix, then update that starbuck's population
	for(int i = 0; i < census2010->size(); i++){
		xCoord = floor((census2010->at(i).x * kAppWidth));
		yCoord = floor((census2010->at(i).y * kAppHeight));
		current = coverageMatrix->at(xCoord + yCoord*kAppWidth);
		current->pop += census2010->at(i).pop;
	}

	//same thing as before, but with the 2000 population
	for(int i = 0; i < census2000->size(); i++){
		xCoord = floor((census2000->at(i).x * kAppWidth));
		yCoord = floor((census2000->at(i).y * kAppHeight));
		current = coverageMatrix->at(xCoord + yCoord*kAppWidth);
		current->oldPop += census2000->at(i).pop;
	}

	//decide color for each starbucks based on population change
	assignColor(myStarbucks->tree_head);

	drawCoverage();

	//now average the pixels of the coverage map with the us map
	//for a transparency effect so we can see borders when coverage is
	//turned on. Colors are a little easier to differentiate between
	//when this isn't on, but it wouldn't be terribly helpful
	//if you couldn't see the borders.
	ColorAT<uint8_t> coverage_pixel;
	ColorAT<uint8_t> map_pixel;
	ColorAT<uint8_t> newColor;
	for(int j = 0; j < kAppHeight; j++){
		for(int i = 0; i < kAppWidth; i++){
			coverage_pixel = coverageSurface->getPixel(Vec2f(i,j));
			map_pixel = us_map->getPixel(Vec2f(i,j));

			newColor.r = (coverage_pixel.r + map_pixel.r * 0.5)/2;
			newColor.g = (coverage_pixel.g + map_pixel.g * 0.5)/2;
			newColor.b = (coverage_pixel.b + map_pixel.b * 0.5)/2;

			coverageSurface->setPixel(Vec2f(i,j),newColor);

		}
	}

}

void HW04App::mouseDown( MouseEvent event )
{
	if(event.isLeft()){
		//clear the previous highlighted spot and show the new one
		gl::clear();
		nearest = myStarbucks->getNearest(((double)event.getX())/kAppWidth,(kAppHeight - ((double)event.getY()))/kAppHeight);
		showNearest = true;
	}
	else if (event.isRight()){
		gl::clear();//clears the highlighted spot and turns off shownearest
		showNearest = false;
	}
}

void HW04App::keyDown( KeyEvent event){

	if(event.getChar() == 'c'){
		
		//clear surfaces if coverage was on before.
		if(isCoverageOn)
			gl::clear();

		//toggle coverage surface
		isCoverageOn = !isCoverageOn;
	}


}

void HW04App::update()
{
	
}

void HW04App::importCensusData(vector<censusPoint>* census, string filename){

	try{
		ifstream fid (filename, ifstream::in);

		char* buffer = new char[256];
		char* tokens = new char[256];
		censusPoint* temp = new censusPoint;
		
		while(!fid.eof()){

			fid.getline(buffer,256,'\r');
			
			//skip the first four tokens of each line
			strtok(buffer,",");
			strtok(NULL,",");
			strtok(NULL,",");
			strtok(NULL,",");
			
			temp->pop = (int) atof(strtok(NULL,","));

			temp->x = atof(strtok(NULL,","));

			temp->y = 1 - atof(strtok(NULL,","));
			
			census->push_back(*temp);
		}

		fid.close();

	}catch(ios_base::failure){
		cout << "Error Reading File\n";
	}
}

void HW04App::assignColor(Leaf* head){

	double popColor;
	double popThreshold = 11000; //value chosen by trial and error
	int popMax = 22000; //value chosen by trial and error
	int popChange = head->pop - head->oldPop;

	if(head->data == NULL)
		return;

	if(head->pop != 0 && head->oldPop != 0){
		if(abs(popChange) > popThreshold){//LARGE POPULATION CHANGE

			popColor = min(abs(popChange),popMax) / ((double)popMax);

			if(popChange > 0) //positive change; make green
				head->popColor = Color(0,popColor,0);
			else //negative change; make red
				head->popColor = Color(popColor,0,0);
		}
		else{//SMALL POPULATION CHANGE

			popColor = abs(popChange)/popThreshold;

			if(popChange > 0) //positive change; make teal
				head->popColor = Color(0,popColor,popColor);
			else if(popChange < 0) //negative change; make purple
				head->popColor = Color(popColor,0,popColor);
			else //no change; make grey.
				head->popColor = Color(.5,.5,.5);
		}
	}
	assignColor(head->leftChild);
	assignColor(head->rightChild);
}

void HW04App::drawCoverage(){
	
	Leaf* current;
	//~~~~~~~~~~~~~~~~~~DRAW COVERAGE MAP~~~~~~~~~~~~~~~~~~//
	//paint each pixel on the coverage surface the color of the closest 
	//starbucks at that pixel (from the coveragematrix)
	for(int j = 0; j < kAppHeight; j++){
		for(int i = 0; i < kAppWidth; i++){
			current = coverageMatrix->at(i+j*kAppWidth);
			coverageSurface->setPixel(Vec2f(i,j),(ColorT<uint8_t>)current->popColor);
		}
	}

}

void HW04App::draw()
{
	gl::color(1,1,1);
	gl::draw(*us_map);
	
	myStarbucks->draw(kAppWidth, kAppHeight, starbucksSurface->getData(), myStarbucks->tree_head);
	
	if(isCoverageOn){
		gl::color(1,1,1);
		gl::draw(*coverageSurface);
	}

	//makes the dot change color every .25 seconds
	if(showNearest){
		if(((int) clock()) % 500 < 250)
			gl::color(1,1,1);
		else
			gl::color(myStarbucks->currentStarbucks->color);

		Vec2f coordinate = Vec2f(nearest->x * kAppWidth, kAppHeight-(nearest->y * kAppHeight));
		gl::drawSolidCircle(coordinate, 3, 0);
	}
	
}

CINDER_APP_BASIC( HW04App, RendererGl )

