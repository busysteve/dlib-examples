
#pragma once

//#include <FL/Fl.H>
//#include <FL/Fl_Double_Window.H>
//#include <FL/Fl_Window.H>
//#include <FL/Fl_Timer.H>
//#include <FL/forms.H>
//#include <FL/fl_draw.H>
#include <math.h>
#include <vector>
#include <deque>
#include <dlib/timer.h>
#include <dlib/matrix.h>
#include <dlib/dnn.h>
//#include <dlib/dnn/tensor_tools.h>
#include <thread>
#include <chrono>
#include <sstream>

#include <curses.h>

#include "snake-ai.h"

using namespace dlib;
using namespace std;



class SnakeView 
{
   
    rectangle area;
    int m_hiscore;
    int m_iterations;
    int m_generations;
    part m_food;
    Snake *m_snake;
    unsigned char red, green,blue;
    bool m_warp;
    //thread  thx; //( void(*), Cool* );
public:

    SnakeView( );
	
    void movesnake();
    void restart();
    void draw( int x, int y );
    void warp( bool w ) { m_warp = w; }
    virtual ~SnakeView() {
        refresh();
        endwin();
    }
};
