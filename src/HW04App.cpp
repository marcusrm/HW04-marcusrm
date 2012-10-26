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
};

void HW04App::setup()
{
	srand ( unsigned ( time (NULL) ) );
	
	double x = 0.720394;
	double y = 0.394542;

	marcusrmStarbucks* myStarbucks = new marcusrmStarbucks();

	string fileName = "../src/Starbucks_2006.csv";
	myStarbucks->importData(fileName);

	//use built in shuffle function on vector
	random_shuffle((*(myStarbucks->locations)).begin(), (*(myStarbucks->locations)).end());

	myStarbucks->build(NULL,NULL);
	//myStarbucks->printInOrder(myStarbucks->tree_head);

	Entry* slowSolution = new Entry;
	Entry* fastSolution = new Entry;

	slowSolution = myStarbucks->getNearestSlow(x,y);
	fastSolution = myStarbucks->getNearest(x,y);

	console() << "Slow solution: " << endl << "City: " << slowSolution->identifier << endl
		<< "x: " << slowSolution->x << endl << "y: " << slowSolution->y << endl;
	console() << "Fast solution: " << endl << "City: " << fastSolution->identifier << endl
		<< "x: " << fastSolution->x << endl << "y: " << fastSolution->y << endl;

	//~~~~~~~~~~~~~~ACCURACY TESTING CODE~~~~~~~~~~~~~~//
	/*
	for(int i = 0; i < 10; i++){
		x = ((double)rand())/RAND_MAX;
		y = ((double)rand())/RAND_MAX;

		console() << "x,y: " << x << "," << y << endl;

		slowSolution = myStarbucks->getNearestSlow(x,y);
		fastSolution = myStarbucks->getNearest(x,y);

		console() << "Slow solution: " << endl << "City: " << slowSolution->identifier << endl
			<< "x: " << slowSolution->x << endl << "y: " << slowSolution->y << endl;
		console() << "Fast solution: " << endl << "City: " << fastSolution->identifier << endl
			<< "x: " << fastSolution->x << endl << "y: " << fastSolution->y << endl;
	}
	*/

	/*	
	//~~~~~~~~~~~~~~TIMING TESTING CODE~~~~~~~~~~~~~~//
	//Thanks to Dr. Brinkman for showing us these time-telling features in boost.
	boost::posix_time::ptime startSlow = boost::posix_time::microsec_clock::local_time();
	for(int i = 0; i < 10000; i++){
		slowSolution = myStarbucks->getNearestSlow(x,y);
	}
	boost::posix_time::ptime endSlow = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration msDiffSlow = endSlow - startSlow;

	boost::posix_time::ptime startFast = boost::posix_time::microsec_clock::local_time();
	for(int i = 0; i < 10000; i++){
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
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( HW04App, RendererGl )

