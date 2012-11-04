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
	Surface* drawCoverage();
	Surface* mySurface_; //The Surface object whose pixel array we will modify
	Surface* coverageSurface;

	//Width and height of the screen
	static const int kAppWidth=500;
	static const int kAppHeight=500;
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

	//Surface us_map(loadImage( loadResource(RES_BABY) ));

	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
	coverageSurface = new Surface(kAppWidth,kAppHeight,false);

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

	coverageSurface = drawCoverage();

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
	if(!isCoverageOn){//don't allow clicks while coverage is on.
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

	//grab the window surface  
	//mySurface_ = &(this->copyWinowSurface());
	
}

/*
void HW04App::fillSurface(Color color){

	uint8_t* pixels = (*mySurface_).getData();

	for(int y = 0; y < kAppHeight; y++){
		for(int x = 0; x < kAppWidth; x++){
			int offset = 3* (x + y*kTextureSize);
			pixels[offset] = color.r;
			pixels[offset+1] = color.g;
			pixels[offset+2] = color.b;
		}
	}
}
*/

Surface* HW04App::drawCoverage(){
	Surface* coverageSurface_copy = new Surface(kAppWidth, kAppHeight,false);
	int shift = 0;
	//~~~~~~~~~~~~~~~~~~DRAW COVERAGE MAP~~~~~~~~~~~~~~~~~~//
	for(int j = 0; j < kAppHeight; j++){
		for(int i = shift%2; i < kAppWidth; i+=2){
			myStarbucks->getNearest(((double)i)/kAppWidth, (kAppHeight - (double)j)/kAppHeight);
			Vec2f currentPixel = Vec2f(i , j);
			coverageSurface_copy->setPixel(currentPixel,(ColorT<uint8_t>)myStarbucks->currentStarbucksColor);
		}
	}

	shift = 1;
	
	for(int j = 0; j < kAppHeight; j++){
		for(int i = shift%2; i < kAppWidth; i+=2){
			Vec2f currentPixel = Vec2f(i , j);
			Vec2f upPixel = Vec2f(i , j-1);
			Vec2f leftPixel = Vec2f(i-1 , j);
			if(*(coverageSurface_copy->getPixel(upPixel)) == *(coverageSurface_copy->getPixel(currentPixel)) &&
				*(coverageSurface_copy->getPixel(leftPixel)) == *(coverageSurface_copy->getPixel(currentPixel)))
				coverageSurface_copy->setPixel(currentPixel,(ColorT<uint8_t>)myStarbucks->currentStarbucksColor);
			
			else{
				myStarbucks->getNearest(((double)i)/kAppWidth, (kAppHeight - (double)j)/kAppHeight);
				coverageSurface_copy->setPixel(currentPixel,ColorAT<uint8_t>(myStarbucks->currentStarbucksColor,127));
			}
		}
	}

	return coverageSurface_copy;

}

void HW04App::draw()
{

	//if using the "show nearest" function, make the dot change color every .25 seconds
	if(showNearest){
		if(((int) clock()) % 500 < 250)
			gl::color(1,1,1);
		else
			gl::color(myStarbucks->currentStarbucksColor);

		Vec2f coordinate = Vec2f(nearest->x * kAppWidth, kAppHeight-(nearest->y * kAppHeight));
		gl::drawSolidCircle(coordinate, 3, 0);

		myStarbucks->draw(kAppWidth, kAppHeight, mySurface_->getData(), kTextureSize, myStarbucks->tree_head);
		
	}
	
	if(isCoverageOn){
		gl::draw(*coverageSurface);
	}
	myStarbucks->draw(kAppWidth, kAppHeight, mySurface_->getData(), kTextureSize, myStarbucks->tree_head);

	
}

CINDER_APP_BASIC( HW04App, RendererGl )

