

//class NeuralNetworkView : public Fl_Double_Window

#include <iostream>
#include "neuralnetwork.h"


//std::vector< la::la_vector<double> > vecs;

NeuralNetworkView::NeuralNetworkView( int x, int y, int w, int h )
	: Fl_Double_Window( x,y,w,h,0)//, thx( NeuralNetworkView::showoff, this ) 
{




	//thx.detach();

}


int NeuralNetworkView::handle( int e )
{
	

	switch( e )
	{
	case FL_PUSH:
		m_mx = Fl::event_x();
		m_my = Fl::event_y();
		return 1;
	case FL_RELEASE:
		return 1;
	case FL_DRAG:
        m_mx = Fl::event_x();
		m_my = Fl::event_y();
		this->redraw();
		return 1;
	default:
		return Fl_Double_Window::handle(e);
	}

}


void NeuralNetworkView::draw()
{


	//for(int b = 1; b < 100; b++ )
	{

		//fl_color( FL_BLACK );
		fl_draw_box( FL_DOWN_BOX, 0, 0, w(), h(), FL_WHITE );


	}

	//this->do_callback();

	//Fl_Double_Window::draw();
}

