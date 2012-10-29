#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
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
	Surface* mySurface_; //The Surface object whose pixel array we will modify

	//Width and height of the screen
	static const int kAppWidth=700;
	static const int kAppHeight=700;
	static const int kTextureSize=1024; //Must be the next power of 2 bigger or equal to app dimensions

	marcusrmStarbucks* myStarbucks;
	Entry* importedData;
	int importedSize;
	Entry* slowSolution;
	Entry* fastSolution;
	double x;
	double y;

};

void HW04App::prepareSettings(Settings* settings){
	(*settings).setWindowSize(kAppWidth,kAppHeight);
	(*settings).setResizable(false);
}

void HW04App::setup()
{
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
	
	importedData = new Entry[10000];

	x = 0.6375931031084;
	y = 0.9758290208923;

	myStarbucks = new marcusrmStarbucks();

	string fileName = "../src/Starbucks_2006.csv";
	importedSize = myStarbucks->importData(importedData, fileName);

	//console() << (*(importedData + sizeof(Entry)*180))->identifier << endl;

	myStarbucks->build(importedData,importedSize);
	//myStarbucks->printInOrder(myStarbucks->tree_head);

	slowSolution = new Entry;
	fastSolution = new Entry;

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
}

void HW04App::update()
{


}

void HW04App::draw()
{


	/*
	gl::color(0.5,0.5,0.5);
	for(int i = 0; i < (*(myStarbucks->locations)).size(); i++){
		Vec2f coordinate = Vec2f(((*(myStarbucks->locations)).at(i))->x * 700, 700 - (((*(myStarbucks->locations)).at(i))->y * 700));
		gl::drawSolidCircle(coordinate, 2, 0);
	}
	*/

	//uint8_t* pixels = (*mySurface_).getData();
	//int offset;

	/*
	//~~~~~~~~~~~~~~~~~~DRAW COVERAGE MAP~~~~~~~~~~~~~~~~~~//
	for(int i = 0; i < kAppWidth; i++){
		for(int j = 0; j < kAppHeight; j++){
		//	offset = 3*(i + j*kAppWidth);

			fastSolution = myStarbucks->getNearest(((double)i)/kAppWidth, ((double)j)/kAppHeight);
			slowSolution = myStarbucks->getNearestSlow(*importedData, importedSize, ((double)i)/kAppWidth, ((double)j)/kAppHeight);
				
		//	if(fastSolution->x == slowSolution->x && fastSolution->y == slowSolution->y)
		//		pixels[offset] = 255;
		//	else
		//		pixels[offset] = 0;
			if(fastSolution->x != slowSolution->x || fastSolution->y != slowSolution->y){
				Vec2f coordinate = Vec2f(i,700-j);
				gl::color(1,0,0);
				gl::drawSolidCircle(coordinate, 1, 0);
			}

		}
	}
	*/
	

/*
	//~~~~~~~~~WHEN I INCLUDE THIS PART, MY FAST SOLUTION BEGINS TO GET LESS ACCURATE, THERE MUST BE SOME
	//KIND OF OPTIMIZATION GOING ON TO GET THE SOLUTION QUICKLY AND FEED IT TO THE DRAW FUNCTION HERE.

	//slow
	gl::color(1,0,0);
	Vec2f coordinate = Vec2f(slowSolution->x * 700, 700 - (slowSolution->y * 700));
	gl::drawSolidCircle(coordinate, 2, 0);

	//real x y
	gl::color(0,0,1);
	coordinate = Vec2f(x * 700, 700 - (y * 700));
	gl::drawSolidCircle(coordinate, 2, 0);

		//fast
	gl::color(0,1,0);
	coordinate = Vec2f(fastSolution->x * 700, 700 - (fastSolution->y * 700));
	gl::drawSolidCircle(coordinate, 2, 0);
	*/
}

CINDER_APP_BASIC( HW04App, RendererGl )

