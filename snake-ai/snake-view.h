
#pragma once

#include <math.h>
#include <vector>
#include <deque>
#include <dlib/timer.h>
#include <dlib/matrix.h>
#include <dlib/dnn.h>
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
    int m_hiscorer_count;
    int m_iterations;
    int m_generations;
    part m_food;
    Snake *m_snake;
    unsigned char red, green,blue;
    bool m_warp;
    //thread  thx; //( void(*), Cool* );
    Snake* m_mama_snake;
    std::deque<Snake*> m_sq;

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
