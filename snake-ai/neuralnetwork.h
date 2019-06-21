
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <math.h>
#include <vector>
#include <queue>

#include <dlib/matrix.h>
#include <thread>
#include <chrono>


#define PI	3.14159265358979323846

using namespace dlib;
using namespace std;


class NeuralNetworkView : public Fl_Double_Window
{

    int m_mx;
	int m_my;

	//thread  thx; //( void(*), Cool* );
	
public:
	NeuralNetworkView( int x, int y, int w, int h );
	
	virtual void draw();
		
	virtual int handle( int e );

};
