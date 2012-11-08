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
	void assignPopulation(vector<censusPoint>* census);
	void assignPopulationChange(vector<censusPoint>* census);
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

	//build a matrix the size of the screen that shows what starbucks each pixel is closest to.
	coverageMatrix = new vector<Leaf*>();
	for(int i = 0; i < kAppHeight; i++){
		for(int j = 0; j < kAppWidth; j++){
			myStarbucks->getNearest(((double)j)/kAppWidth, 1 - ((double)i)/kAppHeight);
			coverageMatrix->push_back(myStarbucks->currentStarbucks);
		}
	}


	importCensusData(census2000,"../src/Census_2000.csv");
	importCensusData(census2010,"../src/Census_2010.csv");
	assignPopulation(census2010);
	assignPopulationChange(census2000);
	assignColor(myStarbucks->tree_head);
	drawCoverage();
}

void HW04App::mouseDown( MouseEvent event )
{
	if(event.isLeft()){
		gl::clear();//clear the previous highlighted spot
		nearest = myStarbucks->getNearest(((double)event.getX())/kAppWidth,(kAppHeight - ((double)event.getY()))/kAppHeight);
		showNearest = true;
	}
	else if (event.isRight()){
		gl::clear();//clear the highlighted spot
		showNearest = false;
	}
}

void HW04App::keyDown( KeyEvent event){

	if(event.getChar() == 'c'){
		
		if(isCoverageOn)
			gl::clear();

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
		int x;
		int y;
		
		while(!fid.eof()){//Brinkman says that this eof shouldn't work. (last bits)
			censusPoint* temp = new censusPoint;

			fid.getline(buffer,256,'\r');
			
			//skip the first four tokens of each line
			strtok(buffer,",");
			strtok(NULL,",");
			strtok(NULL,",");
			strtok(NULL,",");
			
			temp->pop = (int) atof(strtok(NULL,","));

			y = atof(strtok(NULL,","));
			temp->y = (y - 24)/(49-24);

			x = atof(strtok(NULL,","));
			temp->x = (x - (-125))/((-63) - (-125));
			
			census->push_back(*temp);
		}

		fid.close();

	}catch(ios_base::failure){
		cout << "Error Reading File\n";
	}
}

//put in an older census report to find the difference between the 
void HW04App::assignPopulationChange(vector<censusPoint>* census){
	int xCoord;
	int yCoord;
	Leaf* current;
	
	for(int i = 0; i < census->size(); i++){
		xCoord = (int) (census->at(i).x * kAppWidth);
		yCoord = (int) (census->at(i).y * kAppHeight);
		current = coverageMatrix->at(xCoord + yCoord*kAppWidth);
		current->popChange = current->pop - census->at(i).pop;
	}
}

void HW04App::assignPopulation(vector<censusPoint>* census){
	int xCoord;
	int yCoord;
	Leaf* current;
	
	for(int i = 0; i < census->size(); i++){
		xCoord = (int) (census->at(i).x * kAppWidth);
		yCoord = (int) (census->at(i).y * kAppHeight);
		current = coverageMatrix->at(xCoord + yCoord*kAppWidth);
		current->pop = census->at(i).pop;
	}
}

void HW04App::assignColor(Leaf* head){
	
	double popColor;

	if(head->data == NULL)
		return;

	popColor = min(abs(head->popChange),20000) / 20000.0;

	if(head->popChange > 0)
		head->color = Color(0,popColor,0);
	else
		head->color = Color(popColor,0,0);

	assignColor(head->leftChild);
	assignColor(head->rightChild);
}

void HW04App::drawCoverage(){
	
	Leaf* current;
	//~~~~~~~~~~~~~~~~~~DRAW COVERAGE MAP~~~~~~~~~~~~~~~~~~//
	for(int j = 0; j < kAppHeight; j++){
		for(int i = 0; i < kAppWidth; i++){
			current = coverageMatrix->at(i+j*kAppWidth);
			coverageSurface->setPixel(Vec2f(i , j),(ColorT<uint8_t>)current->color);
		}
	}

}

void HW04App::draw()
{
	gl::color(1,1,1);
	gl::draw(*us_map);
	
	//if using the "show nearest" function, make the dot change color every .25 seconds
	myStarbucks->draw(kAppWidth, kAppHeight, starbucksSurface->getData(), myStarbucks->tree_head);
	
	if(isCoverageOn){
		gl::color(1,1,1);
		gl::draw(*coverageSurface);
	}

	
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

