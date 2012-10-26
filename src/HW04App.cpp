#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "marcusrmStarbucks.h"
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

	
	//create input method for file path
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

