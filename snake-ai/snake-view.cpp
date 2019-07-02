

//class SnakeView : public Fl_Double_Window

#include <iostream>
#include "snake-view.h"
#include "snake-ai.h"

//std::vector< la::la_vector<double> > vecs;

SnakeView::SnakeView(  )
    : m_hiscore(0)
    , m_hiscorer_count(0)
    , m_iterations(0)
    , m_generations(0)
    , m_mama_snake(NULL)
{
    m_snake = nullptr;
    ::initscr();
    ::cbreak();
    ::noecho();
    ::curs_set(0);
    ::clear();

    ::srand( time( NULL ) );
    
    restart();
}


void SnakeView::restart()
{
    m_food = { 2+std::rand()%32, 2+std::rand()%44 };

    if( !m_warp )
    {
        ::clear();
        mvaddch( m_food.x, m_food.y, '#' );
        ::refresh();
    }


    if( m_generations > 0 && m_sq.size() >= 3 )
    {
       int mate1 = (::rand() % m_sq.size());
       int mate2 = (::rand() % m_sq.size()-1);
      
       if( m_iterations == 0 )
       {
           m_snake = new Snake( m_sq.front() );
       }
       else
       {
           int r = ::rand() % 3;
           
           if( r == 0 )
           {
               m_snake = new Snake();
           }
           else if( r == 1 )
           {
               auto it = m_sq.begin(); 
               for( int i=0; i < mate1 && it != m_sq.end(); i++,it++ )
                   ; 
               Snake *male = *it;

               Snake *female = new Snake();

               m_snake = female->procreate( male );
               delete female;
           }
           else 
           {
               auto it = m_sq.begin();
               for( int i=0; i < mate1 && it != m_sq.end(); i++,it++ )
                   ;
               Snake *male = *it;

               it = m_sq.begin();
               for( int i=0; i < mate2 && it != m_sq.end(); i++,it++ )
                   ;
               Snake *female = *it;

               m_snake = female->procreate( male );
           }
       }
    }
    else
        m_snake = new Snake(  );

    m_snake->init( 36, 48, 2+std::rand()%32, 2+std::rand()%44 );
    m_snake->set_food( m_food.x, m_food.y );
}


void SnakeView::movesnake( )
{
    
    m_snake->move();
    
   

 
    //if( hit_wall( p )  )
    if( m_snake->dead() )
    {
        //draw();
        

        int f = m_snake->fitness();
        int x = m_snake->score();
        m_x = ( m_x < x ) ? x : m_x;
        
        //sprintf( net_file_name, "./nets/%08d", (int)score() );
        
        if( m_hiscore <= x )
        {
            m_sq.push_front( m_snake );		

            if( m_sq.size() > 30 )
                m_sq.pop_back();

            m_hiscore = x;

        }
	else
	{
            delete m_snake;
	}

        if( m_iterations++ >= 2000 )
        {
            

            m_iterations = 0;
            m_generations++;
        }
       
        //fill_rect(c,rect,rgb_pixel(red,green,blue));

        restart();
        return;
    }
    else if( m_snake->got_food() )
    {
        m_food = { 2+std::rand()%32, 2+std::rand()%44 }; 

        if( !m_warp )
        {
            ::clear();
            mvaddch( m_food.x, m_food.y, '#' );
            ::refresh();
        }
    }



    if( !m_warp )
    {
        draw( m_snake->oldpart().x, m_snake->oldpart().y );  
    } 
    
        ::refresh();
    
   
    ::mvprintw( 40, 4, " %d / %d / %d / %d / %d/ %d / %d         ",  
            m_x,
            m_hiscore,
            m_generations,
            m_iterations,
            m_snake->m_snake.size(),
            m_snake->moves(),
            m_snake->moves_left() );


    //this->redraw();

}


void SnakeView::draw( int x, int y )
{

        mvaddch( x, y, ' ' );
    
        for( auto p : m_snake->m_snake )
            mvaddch( p.x, p.y, '0' );
       

        //::refresh();
    
}





