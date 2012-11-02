#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "marcusrmStarbucks.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

class HW04App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* settings);
	void keyDown(KeyEvent event);
	Surface* mySurface_; //The Surface object whose pixel array we will modify

	//Width and height of the screen
	static const int kAppWidth=700;
	static const int kAppHeight=700;
	static const int kTextureSize=1024; //Must be the next power of 2 bigger or equal to app dimensions

	marcusrmStarbucks* myStarbucks;
	Entry* importedData;
	int importedSize;
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
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
	//Surface us_map(loadImage( loadResource(RES_BABY) ));

	importedData = new Entry[10000];

	x = 0.6375931031084;
	y = 0.9758290208923;

	isCoverageOn = false;
	showNearest = false;

	myStarbucks = new marcusrmStarbucks();

	string fileName = "../src/Starbucks_2006.csv";
	importedSize = myStarbucks->importData(importedData, fileName);

	//console() << (*(importedData + sizeof(Entry)*180))->identifier << endl;

	myStarbucks->build(importedData,importedSize);
	//myStarbucks->printInOrder(myStarbucks->tree_head);

	/*
	slowSolution = myStarbucks->getNearestSlow(x,y);
	fastSolution = myStarbucks->getNearest(x,y);

	console() << "Slow solution: " << endl << "City: " << slowSolution->identifier << endl
		<< "x: " << slowSolution->x << endl << "y: " << slowSolution->y << endl;
	console() << "Fast solution: " << endl << "City: " << fastSolution->identifier << endl
		<< "x: " << fastSolution->x << endl << "y: " << fastSolution->y << endl;
	*/
	
	
	//~~~~~~~~~~~~~~ACCURACY TESTING CODE~~~~~~~~~~~~~~//
	/*
	int n = 10000;
	int correct = 0;
	//double fastDistance;
	//double slowDistance;

	for(int i = 0; i < n; i++){
		x = ((double)rand())/RAND_MAX;
		y = ((double)rand())/RAND_MAX;

		//console() << "x,y: " << x << "," << y << endl;
		
		fastSolution = myStarbucks->getNearest(x,y);
		slowSolution = myStarbucks->getNearestSlow(importedData, importedSize, x,y);

		//console() << "Fast solution: " << endl << "City: " << fastSolution->identifier << endl
		//	<< "x: " << fastSolution->x << endl << "y: " << fastSolution->y << endl;
		//console() << "Slow solution: " << endl << "City: " << slowSolution->identifier << endl
		//	<< "x: " << slowSolution->x << endl << "y: " << slowSolution->y << endl;

		
		//fastDistance = sqrt((fastSolution->x - x)*(fastSolution->x - x) + (fastSolution->y - y)*(fastSolution->y - y));
		//slowDistance = sqrt((slowSolution->x - x)*(slowSolution->x - x) + (slowSolution->y - y)*(slowSolution->y - y));

		//if(fastDistance <= slowDistance*1.25)
		//	correct++;
		

		if(fastSolution->x == slowSolution->x && fastSolution->y == slowSolution->y)
			correct++;

	}

	console() << "Correct: " << correct*100.0/n << "%" << endl;
	*/

		/*
	//~~~~~~~~~~~~~~TIMING TESTING CODE~~~~~~~~~~~~~~//
	//Thanks to Dr. Brinkman for showing us these time-telling features in boost.
	boost::posix_time::ptime startSlow = boost::posix_time::microsec_clock::local_time();
	for(int i = 0; i < 100000; i++){
		slowSolution = myStarbucks->getNearestSlow(importedData, importedSize, x,y);
	}
	boost::posix_time::ptime endSlow = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration msDiffSlow = endSlow - startSlow;

	boost::posix_time::ptime startFast = boost::posix_time::microsec_clock::local_time();
	for(int i = 0; i < 100000; i++){
		fastSolution = myStarbucks->getNearest(x,y);
	}
	boost::posix_time::ptime endFast = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration msDiffFast = endFast - startFast;

	console() << "Slow solution: " << msDiffSlow << endl << "City: " << slowSolution->identifier << endl
		<< "x: " << slowSolution->x << endl << "y: " << slowSolution->y << endl;
	console() << "Fast solution: " << msDiffFast << endl << "City: " << fastSolution->identifier << endl
		<< "x: " << fastSolution->x << endl << "y: " << fastSolution->y << endl;
	*/
	

}

void HW04App::mouseDown( MouseEvent event )
{
	if(event.isLeft()){
		nearest = myStarbucks->getNearest(((double)event.getX())/kAppWidth,(kAppHeight - ((double)event.getY()))/kAppHeight);
		showNearest = true;
	}
	else if (event.isRight()){
		gl::clear();
		showNearest = false;
	}
}

void HW04App::keyDown( KeyEvent event){

	if(event.getChar() == 'c'){
		if(!isCoverageOn){
			myStarbucks->drawCoverage(kAppWidth,kAppHeight,mySurface_);
			isCoverageOn = !isCoverageOn;
		}
		else{
			gl::clear();
			myStarbucks->draw(kAppWidth, kAppHeight,myStarbucks->tree_head);
			isCoverageOn = !isCoverageOn;
		}
	}

}

void HW04App::update()
{


}

void HW04App::draw()
{
	gl::draw(*mySurface_);
	myStarbucks->draw(kAppWidth, kAppHeight,myStarbucks->tree_head);

	if(showNearest){
		if(((int) clock()) % 500 < 250)
			gl::color(1,1,1);
		else
			gl::color(myStarbucks->currentStarbucksColor);

		Vec2f coordinate = Vec2f(nearest->x * kAppWidth, kAppHeight-(nearest->y * kAppHeight));
		gl::drawSolidCircle(coordinate, 3, 0);
		
	}
	
}

CINDER_APP_BASIC( HW04App, RendererGl )

