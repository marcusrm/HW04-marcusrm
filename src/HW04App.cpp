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
	marcusrmStarbucks* myStarbucks = new marcusrmStarbucks();
	//create input method for file path
	string fileName = "../src/Starbucks_2006.csv";
	importData(myStarbucks->locations,fileName);

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

