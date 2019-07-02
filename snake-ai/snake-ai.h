

#pragma once

#include <math.h>
#include <vector>
#include <deque>

#include <dlib/matrix.h>
#include <dlib/dnn.h>
#include <thread>
#include <chrono>
#include <sstream>
#include <curses.h>



using namespace dlib;
using namespace std;

/*
using net_type = loss_multiclass_log<
                                fc<4,        
                                relu<fc<18,   
                                relu<fc<18,  
                                input<matrix<float, 3, 8> 
                                >>>>>>>;


using net_type = loss_multiclass_log<
                                fc<4,        
                                prelu<fc<18,   
                                prelu<fc<18,  
                                input<matrix<float, 3, 8> 
                                >>>>>>>;
*/                                


using input_matrix_type = matrix<double, 3, 8>;

#if 0
using net_type = loss_multiclass_log<
                                fc<4,        
                                relu<fc<18,   
                                relu<fc<18,  
                                input<  input_matrix_type
                                >>>>>>>;


#elif 0
using net_type = loss_multiclass_log<
                                fc<4,        
                                tag1<relu<fc<18,   
                                tag2<relu<fc<18,  
                                input<  input_matrix_type
                                >>>>>>>>>;
#elif 0
using net_type = loss_multiclass_log<
                                tag1<fc<4,relu<
                                tag2<fc<18,relu<
                                tag3<fc<18,
                                input<  input_matrix_type
                                >>>>>>>>>>;
#else
using net_type = loss_multiclass_log<
                                tag1<fc<4,htan<
                                tag2<fc<18,htan<
                                tag3<fc<18,
                                input<  input_matrix_type
                                >>>>>>>>>>;
#endif                                
                                
class part
{
public:
    int x;
    int y;
    part(){};
    part(int x, int y) : x(x), y(y) {};
    part(const part& p) : x(p.x), y(p.y) {};
    part(const part* p) : x(p->x), y(p->y) {};
};




class Snake
{

    dlib::rand rnd;
    int m_wx;
    int m_wy;
    int m_mx;
    int m_my;
    int m_moves_left;
    int m_moves;
    part m_oldpart;
    part m_food;
    bool m_dead;
    bool m_print;
    bool m_got_food;

    int m_hiscore;
    int m_iterations;
    int m_generations;
    
    std::vector< input_matrix_type > m_last_good_observations; 
    std::vector< unsigned long > m_last_good_moves; 
    
    input_matrix_type m_imat;
    
    stringstream m_sstr;

    net_type net;
    net_type m_fnet;
    dnn_trainer<net_type> trainer;
    char m_sx[10000] = {0};
    char m_sy[10000] = {0};
    float m_fx[10000] = {0};
    float m_fy[10000] = {0};
    int m_isx = 0;
    int m_isy = 0;
    
	//thread  thx; //( void(*), Cool* );
public:
    Snake(  );
    Snake( Snake* s );
    ~Snake( );
    void init( int wx, int wy, int x=-1, int y=-1 );
    void move();
    Snake* clone();
    void combine( float*, float*, float*, int );
    void mutate( float*, int, int );
    Snake* procreate( Snake* m );
    int read_snake( const char* snake_file, char* membuf );
    void write_snake( const char* snake_file, const char* membuf, int len );
    int gather_dna( float* dna, Snake* s );
    int place_dna( float* dna, Snake* s );

    bool replay = 0;

    std::deque< part >  m_snake;
    
    part& oldpart() { return m_oldpart; }
    
    int moves()
    {
        return m_moves;
    }
    
    int moves_left()
    {
        return m_moves_left;
    }
   
    net_type& get_net()
    {
        return net;
    }
 
    void serialize( const char* fn )
    {
        try{
            net_type fnet( net );
            fnet.clean();
            dlib::serialize( fn ) << fnet;
        } catch(...) {}
    }
    
    void deserialize( const char* fn )
    {
        try{
            net_type fnet;
            dlib::deserialize( fn ) >> fnet;
            net = fnet;
        } catch(...) {}
    }
   
    Snake* procreate( Snake& m );
 
    double score()
    {
        return m_snake.size()-2;
    }

    double fitness()
    {
        //return 1000 * m_snake.size() + m_moves*m_moves;

        int scor = score();
        
        if( scor < 10 )
           return m_moves*m_moves*pow(2, scor );
        else
           return m_moves*m_moves*pow(2, scor )*(scor-9);
    }

    double calculateFitness()
    {
        return fitness();
    }

    bool dead() { return m_dead; }
    bool got_food() { return m_got_food; }
    bool print( bool p ) { m_print = p; }
    void set_food( int x=-1, int y=-1 );
    void show();
    void randomize( Snake* );

    
};
