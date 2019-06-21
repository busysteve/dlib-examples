

//class SnakeView : public Fl_Double_Window

#include <iostream>
#include "snake-ai.h"
#include "snake-ai-gui.h"

//std::vector< la::la_vector<double> > vecs;

SnakeView::SnakeView( )
    : m_hiscore(0)
    , m_iterations(0)
    , m_generations(0)
{
    m_snake = nullptr;
    ::initscr();
    ::cbreak();
    ::noecho();
    ::curs_set(0);
    ::clear();
    
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

    
    delete m_snake;
    m_snake = new Snake(  );
    m_snake->init( 2+std::rand()%32, 2+std::rand()%44, "/tmp/fittest_snake.net" );
}


void SnakeView::movesnake( )
{
    int wx = 36;
    int wy = 48;
    
    m_snake->movesnake( wx, wy, m_food.x, m_food.y );
    
    
    //if( hit_wall( p )  )
    if( m_snake->dead() )
    {
        //draw();
        

        int s = m_snake->score();
        
        //sprintf( net_file_name, "./nets/%08d", (int)score() );
        
        if( m_hiscore < s )
        {
            m_snake->serialize( "/tmp/snake.net" );
            m_hiscore = s;
        }

        if( m_iterations++ >= 2000 )
        {
            m_snake->deserialize( "/tmp/snake.net" );
            m_snake->serialize( "/tmp/fittest_snake.net" );
            m_snake->serialize( "./fittest_snake.net" );
            
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
    }



    if( !m_warp )
    {
        draw( m_snake->oldpart().x, m_snake->oldpart().y );  
    } 
    
        ::refresh();
    
   
    //cout << m_hiscore << " / " << m_generations << " / " << m_iterations << " / " << m_snake->m_snake.size() << " / " << m_snake->moves() << " / " << m_snake->moves_left() << endl;
    
    
    ::mvprintw( 40, 4, " %d / %d / %d / %d/ %d / %d      ",  
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

