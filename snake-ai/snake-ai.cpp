

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
    //cout << " $ ";
}

Snake::Snake( Snake* s )
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

    net = s->net;
    //cout << " n ";

}

Snake::~Snake()
{
   // cout << " * ";
}

void Snake::init( int wx, int wy, int x, int y )
{

    m_wx = wx;
    m_wy = wy;

    if( x < 0 )
        x = ::rand()%wx;

    if( y < 0 )
        y = ::rand()%wy;

    m_moves_left = 500;
    m_moves = 0;
    
    m_snake.clear();

    //cout << x << " : " << y << endl;    
 
    m_snake.push_back( part(x,y) );

    if( x <= 1 ) x++;
    else x--;

    if( y <= 1 ) y++;
    else y--;

    m_snake.push_back( part(x,y) );


    set_food( ::rand()%wx, ::rand()%wy );

}


Snake* Snake::clone()
{

    Snake* snake = new Snake( this );

    return snake;

}

Snake* Snake::procreate( Snake* m )
{

    int len = gather_dna( m_fx, m );
    gather_dna( m_fy, this );

    float dna[len];

    combine( m_fx, m_fy, dna, len );
    mutate( dna, 5, len );

    Snake* baby = new Snake();
    place_dna( dna, baby );


    return baby;
    
}


void Snake::combine( float* x, float* y, float* z, int len )
{

    //::srand( time( NULL ) );

//    int r = ::rand() % len;

    for( int i=0; i < len; i++ )
        z[i] = x[i];
/*
    for( int i=0; i < len; i++ )
        if( (::rand()%3) == 0 )
		z[i] = y[i];
*/

    int i,r,c,rR,rC;

    i=0; r=4; c=18;
    rR = ::rand()%r;
    rC = ::rand()%c;
    for( int j=0; j<r; j++ )
       for( int k=0; k<c; k++ )
          if((j < rR) || (j == rR && k <= rC))
             z[i+(r*j+k)] = y[i+(r*j+k)];

    i+=4*18; r=18; c=18;
    rR = ::rand()%r;
    rC = ::rand()%c;
    for( int j=0; j<r; j++ )
       for( int k=0; k<c; k++ )
          if((j < rR) || (j == rR && k <= rC))
            z[i+(r*j+k)] = y[i+(r*j+k)];

    i+=18*18; r=18; c=24;
    rR = ::rand()%r;
    rC = ::rand()%c;
    for( int j=0; j<r; j++ )
       for( int k=0; k<c; k++ )
          if((j < rR) || (j == rR && k <= rC))
            z[i+(r*j+k)] = y[i+(r*j+k)];

}

void Snake::mutate( float* dna, int percent, int len )
{

    for( int i=0; i < len; i++ )
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
    //cout << "(" << w1.nr() << "," << w1.nc() << ":" << w1.size() << ")" << w1.k() << " ";
    int s1 = w1.size();
    //int x1 = w1.k();
    //int y1 = s1 / x1;
    for( int i=0; i < s1; i++ )
       dna[x++] = w1.host()[i];

    auto w2 = layer< tag2 >( s->get_net() ).subnet().layer_details().get_weights();
    //cout << "(" << w2.nr() << "," << w2.nc() << ":" << w2.size() << ")" << w2.k() << " ";
    int s2 = w2.size();
    for( int i=0; i < s2; i++ )
       dna[x++] = w2.host()[i];

    auto w3 = layer< tag3 >( s->get_net() ).subnet().layer_details().get_weights();
    //cout << "(" << w3.nr() << "," << w3.nc() << ":" << w3.size() << ")" << w3.k() << " ";
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





void Snake::move()
{
    int wx = m_wx;
    int wy = m_wy;
    
    m_moves++;
    m_moves_left--;
    m_got_food = false;
    
    unsigned int direction;// = ::rand() % 4;
    //sqrt( (x2 − x1)2 + (y2 − y1)2 )
    
    part p = m_snake.front();

    
    auto hit_wall = [&] (const part& h) -> bool {

        
        if( h.x <= 0 )
            return true;
        else if( h.y <= 0 )
            return true;
        else if( h.x >= wx )
            return true;
        else if( h.y >= wy )
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

            
            if( lx == m_food.x && ly == m_food.y )
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
   
   
    //cout << " " << m_snake.size() << endl;
 
    part back_part( m_snake.back() );


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
    else if( p.x == m_food.x && p.y == m_food.y )
    {
        m_got_food = true;
        m_moves_left += 100;
        set_food();
    }
    else
        m_snake.pop_back();
    
    m_snake.push_front( p );
 
}

void Snake::set_food( int x, int y )
{
    if( x < 0 )
        x = ::rand() % m_wx;

    if( y < 0 )
        y = ::rand() % m_wy;

    m_food.x = x;
    m_food.y = y;
}

void Snake::show()
{
    ::clear();
    ::refresh();

    do
    {
        mvaddch( m_food.x, m_food.y, '#' );

        mvaddch( m_oldpart.x, m_oldpart.y, ' ' );

        for( auto p : m_snake )
            mvaddch( p.x, p.y, '0' );

        ::refresh();

        dlib::sleep( 60 );

        move();

    } while( !dead() );
}


