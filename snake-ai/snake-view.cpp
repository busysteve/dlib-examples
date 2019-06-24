

//class SnakeView : public Fl_Double_Window

#include <iostream>
#include "snake-view.h"
#include "snake-ai.h"

//std::vector< la::la_vector<double> > vecs;

SnakeView::SnakeView( )
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

   
         
    delete m_snake;

    if( m_mama_snake != NULL )
	m_snake = m_mama_snake->give_birth();
    else
        m_snake = new Snake(  );

    m_snake->init( 2+std::rand()%32, 2+std::rand()%44, NULL );
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
            if( (m_hiscorer_count++ % 2 ) == 0 )
            	m_snake->serialize( "/tmp/snake1.net" );
            else
            	m_snake->serialize( "/tmp/snake2.net" );
		
            m_hiscore = s;
        }

        if( m_iterations++ >= 2000 )
        {
            delete m_mama_snake;
            m_mama_snake = new Snake();
            m_mama_snake->deserialize( "/tmp/snake1.net" );
            Snake *mate = new Snake();
            mate->deserialize( "/tmp/snake2.net" );

            m_mama_snake->procreate( mate );

            delete mate;

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

