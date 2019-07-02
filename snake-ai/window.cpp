// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

    This is an example illustrating the use of the gui api from the dlib C++ Library.


    This is a pretty simple example.  It makes a window with a user
    defined widget (a draggable colored box) and a button.  You can drag the
    box around or click the button which increments a counter. 
*/




#include <dlib/gui_widgets.h>
#include <sstream>
#include <string>


#include "snake-view.h"
#include "snake-pop.h"


using namespace std;
using namespace dlib;

//  ----------------------------------------------------------------------------

//  ----------------------------------------------------------------------------

int main( int argc, char** argv )
{

    if( argc != 3 )
    {
        printf("%s [population] [threads]\n ", argv[0] );
        exit(0);
    }


    int delay = 100;
           
    ::initscr();
    ::cbreak();
    ::noecho();
    ::curs_set(0);
    ::clear();

 
    int population = atoi( argv[1] );
    int threads = atoi( argv[2] );


    Population pop( population, threads );

    pop.initializeSnakes( 38, 42, 38, 42 );

    pop.update();
    pop.show();


    int highscore = 0;   


    while( true )
    {
        if( pop.done() )
        {
            highscore = pop.bestSnake->score();
            pop.calculateFitness();
            pop.naturalSelection();
            pop.show();
        }
        else
        {
            //pop.update();
            pop.handle_snakes();
            //pop.show();
        }
        int ch = 0; // getch();
        //mvprintw( 42, 20, "%c : %d", ch, ch );
        
        if( ch == 65 )
        {
            //my_window.sv().warp( false );
            if( --delay < 0 )
                delay = 0;
            //my_window.delay_ms( delay );
            //mvprintw( 42, 10, "Delay = %d", delay );
            refresh();
        }    
        else if( ch == 66 )
        {
            //my_window.sv().warp( false );
            if( ++delay > 100 )
                delay = 100;
            //my_window.delay_ms( delay );
            //mvprintw( 41, 10, "Delay = %d", delay );
            refresh();
        }
        else if( ch == 'w' )
        {
            //my_window.delay_ms( 1 );
            //my_window.sv().warp( true );
        }
        else if( ch == 81 || ch == 113 )
        {
            refresh();
            endwin();
            exit(0);
        }

        //::refresh();

        if( delay < 10 )
            delay = 10;
        else if( delay > 100 )
            delay = 100;
        


        //dlib::sleep( 20 );
    }
        ::sleep(10000);



    ::refresh();
    ::endwin();

    return 0;
}

//  ----------------------------------------------------------------------------

// Normally, if you built this application on MS Windows in Visual Studio you
// would see a black console window pop up when you ran it.  The following
// #pragma directives tell Visual Studio to not include a console window along
// with your application.  However, if you prefer to have the console pop up as
// well then simply remove these #pragma statements.
#ifdef _MSC_VER
#   pragma comment( linker, "/entry:mainCRTStartup" )
#   pragma comment( linker, "/SUBSYSTEM:WINDOWS" )
#endif

//  ----------------------------------------------------------------------------

