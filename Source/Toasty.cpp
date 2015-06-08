/*
 * Toasty.cpp
 *
 *  Created on: Jun 8, 2015
 *      Author: neo
 */

#include <Toasty.h>

Toasty::Toasty(Ventana* ventana, ToastyData data)
{
	float factor = data.factor / 100;
	this->_ancho_log = ventana->_ancho_log * factor;
	this->_alto_log = (data.alto * _ancho_log)/data.ancho;

	this->_ancho_px = this->getWidth(ventana,_ancho_log);
	this->_alto_px = this->getHeight(ventana,_alto_log);

	this->_ventana = ventana;
	this->_handler = new TextureHandler(ventana->_gRenderer);
	//cout << "El path es " << data.Imgpath << endl;
	this->_handler->loadFromFile(data.Imgpath,false,0,0,0,true);

	this->_pos_y = 0;
	this->_pos_x = ventana->_pos_log_x + ventana->_ancho_log;

	this->active = false;
	this->rotate = false;

	this->rate = (data.rate / 100) * _ancho_log;

}

void Toasty::setActive()
{
	double r = ((double) rand() / (RAND_MAX));
	if (r < TOASTY_PROB)
	{
		this->active = true;
		this->rotate = false;
	}
}

bool Toasty::view()
{
	//renderScaled( int x, int y, int width, int height );
	//cout << "Muestro Toasty" << endl;
	bool play = false;
	float newx =0;
	//float step = 1;
	float step = this->rate;
	if(!rotate)
	{
		//Primera etapa animacion
		newx = _pos_x - step;
		if (newx + _ancho_log <= _ventana->_pos_log_x + _ventana->_ancho_log)
			{
				//Debo realizar la segunda etapa
				this->rotate = true;
				newx = _ventana->_pos_log_x + _ventana->_ancho_log - _ancho_log;
				play = true;
			}
	}
	else
	{
		newx = _pos_x + step;
		if (newx >= _ventana->_pos_log_x + _ventana->_ancho_log)
		{
			//Fin de la animacion
			this->active = false;
			newx = _ventana->_pos_log_x + _ventana->_ancho_log;
		}
	}
	this->_pos_x = newx;
	int x = this->get_x_px();
	int y = this->get_y_px();
	this->_handler->renderScaled(x,y,_ancho_px,_alto_px);
	return play;
}

int Toasty::get_x_px()
{
    float pos_r = 0; //La posicion en pixeles en reales
    int pos_e = 0; //La posicion en pixeles verdadera (entero)
    float pos_rel = _pos_x - _ventana->_pos_log_x; //Posicion logica relativa a la ventana
    pos_r = ( pos_rel * _ancho_px) / _ancho_log;
    //pos_r = ( _pos_x * _ancho_px) / _ancho_log;
    pos_e = (int) pos_r;
    return pos_e;
}

int Toasty::get_y_px()
{
    float pos_r = 0; //La posicion en pixeles en reales
    int pos_e = 0; //La posicion en pixeles verdadera (entero)
    pos_r = ( (_ventana->_alto_log - _pos_y) * _ventana->_alto_px) / _ventana->_alto_log;
    pos_e = (int) pos_r;
    //cout << pos_r << endl;
    pos_e =  pos_e -_alto_px;
    return pos_e;
}

int Toasty::getWidth(Ventana* ventana, float ancho_log_capa)
{
    int ancho = 0;
    ancho = (float)(ancho_log_capa * ventana->_ancho_px) / ventana->_ancho_log ;
    return ancho;
}

int Toasty::getHeight(Ventana* ventana, float alto_log_capa)
{
    int alto = 0;
    alto = (float)(alto_log_capa * ventana->_alto_px) / ventana->_alto_log ;
    //cout << alto << endl;
    return alto;
}
