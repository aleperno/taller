/*
 * Capa.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: neo
 */
#include <Capa.h>

Capa::Capa(Ventana* ventana, CapaData data, EscenarioData escenario)
{
	this->_ventana = ventana;
	this->_handler = new TextureHandler(ventana->_gRenderer);
	this->_alto_log = _ventana->_alto_log;
	this->_ancho_log = data.ancho;
	string path = data.imagen_fondo;
	this->_handler->loadFromFile(path,true);
	this-> _escenario = escenario;
	//cout << "Se crea capa con imagen " << path << endl;
	this-> _factor_escala = escenario.ancho / this->_ancho_log;
	//cout << "El factor de escala es " << _factor_escala << endl;
	this-> _pos_y = 0;
	//this-> _pos_x = ((- _ancho_log * _factor_escala)+ventana->_ancho_log) /2;
	this-> _pos_x = ((- _ancho_log)+ventana->_ancho_log) /2;
	//this-> _pos_x = -100;
	this->_ancho_px = Capa::getWidth(ventana,_ancho_log);
	this->_alto_px = Capa::getHeight(ventana,_alto_log);
}

Capa::~Capa()
{
	//delete this;
	cout << "destruyo capa" << endl;
	delete _handler;
}

void Capa::view()
{
	int x = get_x_px();
	int y = get_y_px();
	//cout << "La posicion logica en x es " << _pos_x << " y en px es " << x << endl;
	//cout << "La posicion logica en y es " << _pos_y << " y en px es " << y << endl;
	//_handler->renderCut(x,y,_ancho_px * _factor_escala,_alto_px);
	_handler->renderCut(x,y,_ancho_px,_alto_px);
	//_handler->render(0,0);
	//cout << "Se intenta dibujar capa " << endl;
}

int Capa::getWidth(Ventana* ventana, float ancho_log_capa)
{
	int ancho = 0;
	ancho = (float)(ancho_log_capa * ventana->_ancho_px) / ventana->_ancho_log ;
	return ancho;
}

int Capa::getHeight(Ventana* ventana, float alto_log_capa)
{
	int alto = 0;
	alto = (float)(alto_log_capa * ventana->_alto_px) / ventana->_alto_log ;
	//cout << alto << endl;
	return alto;
}

int Capa::get_x_px()
{
	float pos_r = 0; //La posicion en pixeles en reales
	int pos_e = 0; //La posicion en pixeles verdadera (entero)
	pos_r = ( _pos_x * _ancho_px) / _ancho_log;
	pos_e = (int) pos_r;
	return pos_e;
}

int Capa::get_y_px()
{
	float pos_r = 0; //La posicion en pixeles en reales
	int pos_e = 0; //La posicion en pixeles verdadera (entero)
	pos_r = ( (_alto_log - _pos_y) * _alto_px) / _alto_log;
	pos_e = (int) pos_r;
	pos_e =  pos_e -_alto_px;
	return pos_e;
}

void Capa::moveLeft(float factor)
{
	//cout << "La pos actual es " << _pos_x << " el ancho logico es " << _ancho_log;
	float new_x = _pos_x - ((_ancho_log / factor) + getAlpha(factor)) ;
	//cout << " me intento mover a " << new_x << endl;
	if (new_x + _ancho_log >= _ventana->_ancho_log)
	{
		_pos_x = new_x;
	}else{
		_pos_x = -(this->_ancho_log - _ventana->_ancho_log );
	}
}

void Capa::moveRight(float factor)
{
	float new_x = _pos_x + (_ancho_log / factor) + getAlpha(factor);
	if (new_x <= 0)
	{
		_pos_x = new_x;
	}else{
		_pos_x = 0;
	}
}

float Capa::getAlpha(float factor)
{
	float ancho_escenario = this->_escenario.ancho;
	float ancho_capa = this->_ancho_log;
	float ancho_ventana = this->_ventana->_ancho_log;
	//int ancho_escenario = 10000;
	//int ancho_capa = 6000;
	//int ancho_ventana = 600;
	float alpha = ((( ancho_escenario * (ancho_capa - ancho_ventana) ) / (ancho_escenario - ancho_ventana)) - ancho_capa ) / factor;
	//cout << "ALPHA VALEEEEE " << alpha << endl;
	return alpha;
}
