

//class Snake : public Fl_Double_Window

#include <iostream>
#include "snake-ai.h"
#include "snake-ai-gui.h"

//std::vector< la::la_vector<double> > vecs;

Snake::Snake( )
	: rnd( ::time(NULL) )
    , m_hiscore(0)
    , m_iterations(0)
    , m_generations(0)
    , m_dead(false)
    , m_print(false)
    , m_got_food(false)
    , net( prelu_( rnd.get_double_in_range(.001, .999) ), prelu_( rnd.get_double_in_range( .001, .999 ) ) )
	, trainer( net )
{

    
    
    //net = new net_type( prelu_( rnd.get_double_in_range(.1, .9) ), prelu_( rnd.get_double_in_range( .1, .9 ) ) );
    
    //trainer = new dnn_trainer<net_type>( *net );
    
    
    tt::tensor_rand trand;
    
    //trand.fill_gaussian( layer<2>(net) );
    //trand.fill_gaussian( layer<4>(net) );

    
    
    
    if(false) {
        std::vector< input_matrix_type > vecMats;
        std::vector< long unsigned int > vecOuts;
        
#if 0
        for( int i=0; i < 1000; i++ )
        {
            vecMats.push_back( { 
                        std::rand()%100, std::rand()%100, std::rand()%100,   
                        std::rand()%100, std::rand()%100, std::rand()%100,   
                        std::rand()%100, std::rand()%100, std::rand()%100,   
                        std::rand()%100, std::rand()%100, std::rand()%100,   
                        std::rand()%100, std::rand()%100, std::rand()%100,   
                        std::rand()%100, std::rand()%100, std::rand()%100,   
                        std::rand()%100, std::rand()%100, std::rand()%100,   
                        std::rand()%100, std::rand()%100, std::rand()%100   
            } );
            
            vecOuts.push_back( std::rand() % 4 );
        }
        
        trainer.train( vecMats, vecOuts );
#endif

    }
    //thx.detach();    

}

#if 0
Snake* Snake::procreate( Snake* mate )
{
    
    net_type net;
    
    
    Snake* baby = new Snake();
    
    
    
    
}
#endif

void Snake::init( int wx, int wy, const char* snake_net_file )
{
    m_moves_left = 500;
    m_moves = 0;
    
    m_snake.clear();
    
    ::srand( (unsigned int)time(NULL) );
    
    m_snake.push_back( part(2+std::rand()%wx, 2+std::rand()%wy) );
    m_snake.push_back( part( m_snake.front().x+1, m_snake.front().y ) );

    
    try
    {
        dlib::deserialize( snake_net_file ) >> m_fnet;    
        net = m_fnet;
    } catch(...){};
    
    
    trainer.set_learning_rate(0.01);
    trainer.set_min_learning_rate(0.001);
    trainer.set_mini_batch_size(100);
    trainer.set_max_num_epochs( 200 );
    trainer.be_verbose();
    
    cerr << net << endl;
    
}


void Snake::movesnake( int wx, int wy, int fx, int fy )
{
    //int wx = 36;
    //int wy = 48;
    
    m_moves++;
    m_moves_left--;
    m_got_food = false;
    
    unsigned int direction;// = ::rand() % 4;
    //sqrt( (x2 − x1)2 + (y2 − y1)2 )
    
    part p = m_snake.front();

#if 1
    
    auto hit_wall = [&] (const part& h) -> bool {

        
        if( h.x == 0 )
            return true;
        else if( h.y == 0 )
            return true;
        else if( h.x == wx )
            return true;
        else if( h.y == wy )
            return true;
        
        return false;
    };
    
    auto hit_body = [&] (const part& h) -> bool {
        
            int i=0;
            for( auto s : m_snake )
            {
                if( i++ > 0  && h.x == s.x && h.y == s.y )
                {
                    return true;
                }
            }
        
        return false;
    };
    

    auto look = [&] ( int x, int y ) -> pair< int, float > {
        int saw = 0;
        float df = 1000.0;
        float dw = 1000.0;
        float db = 1000.0;

        int i=0;
        int lx = p.x;
        int ly = p.y;
        
        while( i < 1000 )
        {
            i++;
            
            lx += x;
            ly += y;
            
            for( auto s : m_snake )
            {
                if( lx == s.x && ly == s.y )
                {
                    db = sqrt( std::pow( (p.x - lx), 2.0) + std::pow( (p.y - ly), 2.0) );
                    return make_pair( 1, db );
                    break;
                }
            }

            
            if( lx == fx && ly == fy )
            {
                df = sqrt( std::pow( (p.x - lx), 2.0 ) + std::pow( (p.y - ly), 2.0 ) );
                return make_pair( 2, df );
                break;
            }

            
            if( lx == wx || ly == wy || lx == 0 || ly == 0 )
            {
                dw = sqrt( std::pow( (p.x - lx), 2.0) + std::pow( (p.y - ly), 2.0) );
                return make_pair( 0, dw );
                break;
            }
            
        }
        
        cout << dw << " : " << db << " : " << df << endl; 
        

        cout << "look() error" << endl;

        return make_pair( -1, 0.0 );
    };

    
    //up
    auto saw = look( 0, -1 );
//    cout << saw.first << " - " << saw.second << endl;
    m_imat(0,0) = ( saw.first == 1 ? 1.0 : 0.0 ); // body
    m_imat(1,0) = ( saw.first == 2 ? 1.0 : 0.0 ); // food
    m_imat(2,0) = 1.0 / saw.second; // distance

    //down
    saw = look( 0, 1 );
//    cout << saw.first << " - " << saw.second << endl;
    m_imat(0,1) = ( saw.first == 1 ? 1.0 : 0.0 ); // wall
    m_imat(1,1) = ( saw.first == 2 ? 1.0 : 0.0 ); // food
    m_imat(2,1) = 1.0 / saw.second; // distance

    //left
    saw = look( -1, 0 );
//    cout << saw.first << " - " << saw.second << endl;
    m_imat(0,2) = ( saw.first == 1 ? 1.0 : 0.0 ); // wall
    m_imat(1,2) = ( saw.first == 2 ? 1.0 : 0.0 ); // food
    m_imat(2,2) = 1.0 / saw.second; // distance

    //right
    saw = look( 1, 0 );
//    cout << saw.first << " - " << saw.second << endl;
    m_imat(0,3) = ( saw.first == 1 ? 1.0 : 0.0 ); // wall
    m_imat(1,3) = ( saw.first == 2 ? 1.0 : 0.0 ); // food
    m_imat(2,3) = 1.0 / saw.second; // distance

    //up / left
    saw = look( -1, -1 );
//    cout << saw.first << " - " << saw.second << endl;
    m_imat(0,4) = ( saw.first == 1 ? 1.0 : 0.0 ); // wall
    m_imat(1,4) = ( saw.first == 2 ? 1.0 : 0.0 ); // food
    m_imat(2,4) = 1.0 / saw.second; // distance

    //up / right
    saw = look( 1, -1 );
//    cout << saw.first << " - " << saw.second << endl;
    m_imat(0,5) = ( saw.first == 1 ? 1.0 : 0.0 ); // wall
    m_imat(1,5) = ( saw.first == 2 ? 1.0 : 0.0 ); // food
    m_imat(2,5) = 1.0 / saw.second; // distance

    //down / left
    saw = look( -1, 1 );
//    cout << saw.first << " - " << saw.second << endl;
    m_imat(0,6) = ( saw.first == 1 ? 1.0 : 0.0 ); // wall
    m_imat(1,6) = ( saw.first == 2 ? 1.0 : 0.0 ); // food
    m_imat(2,6) = 1.0 / saw.second; // distance

    //down / right
    saw = look( 1, 1 );
//    cout << saw.first << " - " << saw.second << endl;
    m_imat(0,7) = ( saw.first == 1 ? 1.0 : 0.0 ); // wall
    m_imat(1,7) = ( saw.first == 2 ? 1.0 : 0.0 ); // food
    m_imat(2,7) = 1.0 / saw.second; // distance

    //cout << *net << endl;
    //cout << layer<2>(net).get_output() << endl;
    //layer<4>(net).get_output();
    
    std::vector< input_matrix_type > vecMats = { m_imat };
    
    //cout << (*net)( vecMats ).size() << endl;
    
    direction = net( vecMats )[0];
    
    m_last_good_observations.push_back( m_imat ); 
    m_last_good_moves.push_back( direction );
    
    std::vector< long unsigned int > vecOuts = { direction };
    
    if( (::rand() % 5) == 0 )
    {
        m_last_good_moves.push_back( ::rand()%4 );
        m_last_good_observations.push_back( { 
                        rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999),   
                        rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999),   
                        rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999),   
                        rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999),   
                        rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999),   
                        rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999),   
                        rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999),   
                        rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999), rnd.get_double_in_range(0.0, 0.999)
            } );
    }
    
    if( m_moves >= 20 && m_snake.size() >= 4 )
        trainer.train( m_last_good_observations, m_last_good_moves );
    
    //trainer.train_one_step( vecMats );
    
#endif
    
    auto back_part( m_snake.back() );

    m_oldpart = back_part;
    
    switch( direction )
    {
        case 0: // up
            p.y--;
            break;
        case 1: // down
            p.y++;
            break;
        case 2: // left
            p.x--;
           break;
        case 3: // right
            p.x++;
            break;
    }
    
    
    
    //if( hit_wall( p )  )
    if( hit_wall( p ) || hit_body( p ) )
    {
        char net_file_name[1000];
        
        int s = score();
        
        //sprintf( net_file_name, "./nets/%08d", (int)score() );
        
        m_dead = true;
        
        return;
    }
    else if( p.x == fx && p.y == fy )
    {
        m_got_food = true;

        m_moves_left += 100;
        
        ;// dont drop backpart

    }
    else
        m_snake.pop_back();
    
    m_snake.push_front( p );
 
    
    //cout << m_hiscore << " / " << m_generations << " / " << m_iterations << " / " << m_snake.size() << " / " << m_moves++ << " / " << m_moves_left-- << endl;
    


    //cerr << layer<4>(net).layer_details() << endl;
    
    //cerr << layer<4>(net).to_tensor().size() << endl;
    
    
    
}


