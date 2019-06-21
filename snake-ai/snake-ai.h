
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

using net_type = loss_multiclass_log<
                                fc<4,        
                                dropout< prelu<fc<18,   
                                dropout< prelu<fc<18,  
                                input<  input_matrix_type
                                >>>>>>>>>;
                                
                                
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
    
	//thread  thx; //( void(*), Cool* );
public:
	Snake(  );
	void init( int wx, int wy, const char* snake_net_file );
	void movesnake( int x, int y, int fx, int fy );

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
    
    void serialize( const char* fn )
    {
        try{
            net_type fnet( net );
            //fnet.clean();
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
    
    double score()
    {
        return 1000 * m_snake.size() + m_moves;
    }

    bool dead() { return m_dead; }
    bool got_food() { return m_got_food; }
    bool print( bool p ) { m_print = p; }
    
};
