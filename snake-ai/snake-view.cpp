

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


    if( m_sq.size() >= 3 )
    {
       int mate1 = (::rand() % m_sq.size());
       int mate2 = (::rand() % m_sq.size()-1);
       
       auto it = m_sq.begin();
       for( int i=0; i < mate1 && it != m_sq.end(); i++,it++ )
           ;
       Snake *male = *it;

       it = m_sq.begin();
       for( int i=0; i < mate2 && it != m_sq.end(); i++,it++ )
           ;
       Snake *female = *it;

       female->procreate( male );

       m_snake = female->give_birth();

          
    }
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
        
        if( m_hiscore <= s )
        {
            m_sq.push_front( m_snake );		

            if( m_sq.size() > 30 )
                m_sq.pop_back();

            m_hiscore = s;

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
    
   
    //cout << m_hiscore << " / " << m_generations << " / " << m_iterations << " / " << m_snake->m_snake.size() << " / " << m_snake->moves() << " / " << m_snake->moves_left() << endl;
    
    
    ::mvprintw( 40, 4, " %d / %d / %d / %d/ %d / %d         ",  
            m_hiscore,
            m_generations,
            m_iterations,
            m_snake->m_snake.size(),
            m_snake->moves(),
            m_snake->moves_left() );

#if 0 
    ::mvprintw( 41, 14, "  %d|%d:%d-%d       "
            ,layer< tag1 >( m_snake->get_net() ).get_output().k()
            ,layer< tag1 >( m_snake->get_net() ).get_output().nc()
            ,layer< tag1 >( m_snake->get_net() ).get_output().nr()
            ,layer< tag1 >( m_snake->get_net() ).get_output().size()
     );

    ::mvprintw( 42, 14, "  %d|%d:%d-%d       "
            ,layer< tag2 >( m_snake->get_net() ).get_output().k()
            ,layer< tag2 >( m_snake->get_net() ).get_output().nc()
            ,layer< tag2 >( m_snake->get_net() ).get_output().nr()
            ,layer< tag2 >( m_snake->get_net() ).get_output().size()
     );

    ::mvprintw( 43, 14, "  %d|%d:%d-%d ( %0.6f )    %d     "
            ,layer< tag3 >( m_snake->get_net() ).get_output().k()
            ,layer< tag3 >( m_snake->get_net() ).get_output().nc()
            ,layer< tag3 >( m_snake->get_net() ).get_output().nr()
            ,layer< tag3 >( m_snake->get_net() ).get_output().size()
            ,layer< tag3 >( m_snake->get_net() ).get_output().host()[0]
            ,layer< tag3 >( m_snake->get_net() ).subnet().get_output().size()
     );


    ::mvprintw( 44, 14, "  %d  %d  %d  %d  %d  %d         "
            ,layer< tag1 >( m_snake->get_net() ).get_output().size()
            ,layer< tag2 >( m_snake->get_net() ).get_output().size()
            ,layer< tag3 >( m_snake->get_net() ).get_output().size()
            ,layer< tag1 >( m_snake->get_net() ).subnet().get_output().size()
            ,layer< tag2 >( m_snake->get_net() ).subnet().get_output().size()
            ,layer< tag3 >( m_snake->get_net() ).subnet().get_output().size()
     );

    ::mvprintw( 44, 14, "  %d   %0.6f   %0.6f        "
            ,layer< tag1 >( m_snake->get_net() ).subnet().layer_details().get_weights().size()
            ,layer< tag1 >( m_snake->get_net() ).subnet().layer_details().get_weights().host()[0]
            ,layer< tag1 >( m_snake->get_net() ).subnet().layer_details().get_weights().host()[0]
     );


     layer< tag1 >( m_snake->get_net() ).subnet().layer_details().get_weights().host()[0] = .4;


    ::mvprintw( 45, 14, "  %d   %0.6f   %0.6f        "
            ,layer< tag1 >( m_snake->get_net() ).subnet().layer_details().get_weights().size()
            ,layer< tag1 >( m_snake->get_net() ).subnet().layer_details().get_weights().host()[0]
            ,layer< tag1 >( m_snake->get_net() ).subnet().layer_details().get_weights().host()[0]
     );

#endif

    //this->redraw();

}


void SnakeView::draw( int x, int y )
{

        mvaddch( x, y, ' ' );
    
        for( auto p : m_snake->m_snake )
            mvaddch( p.x, p.y, '0' );
       

        //::refresh();
    
}

