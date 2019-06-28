

#pragma once

#include <math.h>
#include <vector>
#include <deque>

#include <dlib/matrix.h>
#include <dlib/dnn.h>
#include <thread>
#include <chrono>
#include <sstream>


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
#else
using net_type = loss_multiclass_log<
                                tag1<fc<4,relu<
                                tag2<fc<18,relu<
                                tag3<relu<fc<18,
                                input<  input_matrix_type
                                >>>>>>>>>>>;
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
    int m_mx;
    int m_my;
    int m_moves_left;
    int m_moves;
    part m_oldpart;
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
    void init( int wx, int wy, const char* snake_net_file );
    void movesnake( int x, int y, int fx, int fy );

    void combine( float*, float*, float* );
    void mutate( float*, int );
    void procreate( Snake* m );
    Snake* give_birth();
    int read_snake( const char* snake_file, char* membuf );
    void write_snake( const char* snake_file, const char* membuf, int len );
    int gather_dna( float* dna, char* membuf );
    int place_dna( float* dna, char* membuf );

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
        //return 1000 * m_snake.size() + m_moves*m_moves;

        int scor = m_snake.size()-1;

        return m_moves*m_moves*pow(2, scor )*m_moves_left;
    }

    bool dead() { return m_dead; }
    bool got_food() { return m_got_food; }
    bool print( bool p ) { m_print = p; }
    
};
