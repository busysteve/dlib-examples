

//class Snake : public Fl_Double_Window

#include <iostream>
#include <string>
#include "snake-ai.h"

//std::vector< la::la_vector<double> > vecs;

Snake::Snake( )
	: rnd( ::time(NULL) )
    , m_hiscore(0)
    , m_iterations(0)
    , m_generations(0)
    , m_dead(false)
    , m_print(false)
    , m_got_food(false)
    //, net(prelu_(rnd.get_double_in_range(.001,.999)), prelu_(rnd.get_double_in_range(.001,.999)))
    //, net(relu_(rnd.get_double_in_range(.001,.999)), relu_(rnd.get_double_in_range(.001,.999)))
    , trainer( net )
{

    
    

}


void Snake::init( int wx, int wy, const char* snake_net_file )
{
    m_moves_left = 500;
    m_moves = 0;
    
    m_snake.clear();
    
    
    m_snake.push_back( part(2+std::rand()%wx, 2+std::rand()%wy) );
    m_snake.push_back( part( m_snake.front().x+1, m_snake.front().y ) );

   
    if( snake_net_file != NULL ) 
    try
    {
        dlib::deserialize( snake_net_file ) >> m_fnet;    
        net = m_fnet;

    } catch(...){};
    
}


Snake* Snake::procreate( Snake* m )
{

    gather_dna( m_fx, m );
    gather_dna( m_fy, this );

    float dna[10000];

    combine( m_fx, m_fy, dna );
    mutate( dna, 5 );

    Snake* baby = new Snake();
    place_dna( dna, baby );


    return baby;
    
}


void Snake::combine( float* x, float* y, float* z )
{

    //::srand( time( NULL ) );

    for( int i=0; i < 10000; i++ )
    {
        z[i] = x[i];
    }

    for( int i=0; i < 10000; i++ )
    {
        if( ( ::rand() % 2 ) == 0 )
		z[i] = y[i];
    }

}

void Snake::mutate( float* dna, int percent )
{

    for( int i=0; i < 10000; i++ )
    {
        if( (::rand() % (100 / percent) ) == 0 )
            dna[i] = rnd.get_double_in_range( .0, .9999 );
    }

}


int Snake::read_snake( const char* snake_file, char* membuf )
{
    int x=0;

    FILE* fp = fopen( snake_file, "r+" );

    for( int i=0; feof( fp ) == 0; i++ ) 
	x += ::fread( &membuf[i], 1, 1, fp );

    fclose( fp );
    return x;
}


void Snake::write_snake( const char* snake_file, const char* membuf, int len )
{

   FILE* fp = fopen( snake_file, "w+" );

   for( int i=0; i < len; i++ )
      ::fwrite( membuf, 1, 1, fp );

   fclose( fp );
}


int Snake::gather_dna( float* dna, Snake* s )
{

    int x=0;

    auto w1 = layer< tag1 >( s->get_net() ).subnet().layer_details().get_weights();
    int s1 = w1.size();
    for( int i=0; i < s1; i++ )
       dna[x++] = w1.host()[i];

    auto w2 = layer< tag2 >( s->get_net() ).subnet().layer_details().get_weights();
    int s2 = w2.size();
    for( int i=0; i < s2; i++ )
       dna[x++] = w2.host()[i];

    auto w3 = layer< tag3 >( s->get_net() ).subnet().layer_details().get_weights();
    int s3 = w3.size();
    for( int i=0; i < s3; i++ )
       dna[x++] = w3.host()[i];


    return x;
}


int Snake::place_dna( float* dna, Snake* s )
{

    int x=0;

    auto w1 = layer< tag1 >( s->get_net() ).subnet().layer_details().get_weights();
    int s1 = w1.size();
    for( int i=0; i < s1; i++ )
       w1.host()[i] = dna[x++];

    auto w2 = layer< tag2 >( s->get_net() ).subnet().layer_details().get_weights();
    int s2 = w2.size();
    for( int i=0; i < s2; i++ )
       w2.host()[i] = dna[x++];

    auto w3 = layer< tag3 >( s->get_net() ).subnet().layer_details().get_weights();
    int s3 = w3.size();
    for( int i=0; i < s3; i++ )
       w3.host()[i] = dna[x++];


    return x;
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

        //cerr << "look() error" << endl;

        return make_pair( -1, 0.0 );
    };


    int views[][2] = {
	{ 0, -1 }, // up
	{ 0,  1 }, // down
	{-1,  0 }, // left
	{ 1,  0 }, // right 
	{-1, -1 }, // up / left
	{ 1, -1 }, // up / right
	{-1,  1 }, // down / left
	{ 1,  1 }  // down / right
    };

    for( int d=0; d < 8; d++ )
    {
        auto saw = look( views[d][0], views[d][1] );
        m_imat(0,d) = ( saw.first == 1 ? 1.0 : 0.0 ); // body
        m_imat(1,d) = ( saw.first == 2 ? 1.0 : 0.0 ); // food
        m_imat(2,d) = 1.0 / saw.second; // distance
    }
    
    std::vector< input_matrix_type > vecMats = { m_imat };
    direction = net( vecMats )[0];
   
    
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
    if( hit_wall( p ) || hit_body( p ) || m_moves_left <= 0)
    {
        m_dead = true;
        return;
    }
    else if( p.x == fx && p.y == fy )
    {
        m_got_food = true;
        m_moves_left += 100;
    }
    else
        m_snake.pop_back();
    
    m_snake.push_front( p );
 
}


