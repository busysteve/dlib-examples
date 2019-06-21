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


using namespace std;
using namespace dlib;

//  ----------------------------------------------------------------------------

//  ----------------------------------------------------------------------------

class swin 
{
    /*
        Here I am going to define our window.  In general, you can define as 
        many window types as you like and make as many instances of them as you want.
        In this example I am only making one though.
    */
public:
    swin(
    ) : // All widgets take their parent window as an argument to their constructor.
        snake(), // the color_box will be blue and 101 pixels wide and tall
        t( snake, &SnakeView::movesnake )
    {

        //timer<SnakeView> t( snake, &SnakeView::movesnake );

        t.set_delay_time( 10 );
        //t.start();

    } 

    void start()
    {
        t.start();
    }

    void delay_ms( int v )
    {
        t.set_delay_time( v );
    }

    SnakeView& sv() { return snake; }

    ~swin(
    )
    {
        // You should always call close_window() in the destructor of window
        // objects to ensure that no events will be sent to this window while 
        // it is being destructed.  
    }

private:

    SnakeView snake;
    timer<SnakeView> t;
};

//  ----------------------------------------------------------------------------

int main()
{
    // create our window
    swin my_window;

    int delay = 100;
           
    my_window.delay_ms( delay );

    my_window.start();

    // wait until the user closes this window before we let the program 
    // terminate.


    while( true )
    {
        int ch = getch();
        mvprintw( 42, 20, "%c : %d", ch, ch );
        
        if( ch == 65 )
        {
            my_window.sv().warp( false );
            if( --delay < 0 )
                delay = 0;
            my_window.delay_ms( delay );
            mvprintw( 42, 10, "Delay = %d", delay );
            refresh();
        }    
        else if( ch == 66 )
        {
            my_window.sv().warp( false );
            if( ++delay > 100 )
                delay = 100;
            my_window.delay_ms( delay );
            mvprintw( 41, 10, "Delay = %d", delay );
            refresh();
        }
        else if( ch == 'w' )
        {
            my_window.delay_ms( 0 );
            my_window.sv().warp( true );
        }
        else if( ch == 81 || ch == 113 )
        {
            refresh();
            endwin();
            exit(0);
        }

        //::refresh();

        if( delay < 0 )
            delay = 0;
        else if( delay > 100 )
            delay = 100;
        


        dlib::sleep( 20 );
    }
        ::sleep(10000);



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

