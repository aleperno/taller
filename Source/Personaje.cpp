/*
 * Personaje.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: neo
 */
#include <Personaje.h>

Personaje::Personaje(Ventana* ventana, PersonajeData data, EscenarioData escenario)
{
	this->_ventana = ventana;
	this->_handler = new TextureHandler(ventana->_gRenderer);
	this->_alto_log = data.alto;
	this->_ancho_log = data.ancho;
	string path = data.imgPath;
	this->_handler->loadFromFile(path,true);
	this-> _escenario = escenario;
	this-> _factor_escala = escenario.ancho / this->_ancho_log;
	this->_pos_y = escenario.y_piso;
	this->_pos_x = ((- _ancho_log)+ventana->_ancho_log) /2;
	this->_ancho_px = Personaje::getWidth(ventana,_ancho_log);
	this->_alto_px = Personaje::getHeight(ventana,_alto_log);
	this->_zIndex = data.z_index;
	//cout << _pos_x << endl;
}

Personaje::~Personaje()
{
	//delete this;
	cout << "destruyo capa" << endl;
	delete _handler;
}

void Personaje::view()
{
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
}

int Personaje::get_x_px()
{
	float pos_r = 0; //La posicion en pixeles en reales
	int pos_e = 0; //La posicion en pixeles verdadera (entero)
	pos_r = ( _pos_x * _ancho_px) / _ancho_log;
	pos_e = (int) pos_r;
	return pos_e;
}

int Personaje::get_y_px()
{
	float pos_r = 0; //La posicion en pixeles en reales
	int pos_e = 0; //La posicion en pixeles verdadera (entero)
	pos_r = ( (_ventana->_alto_log - _pos_y) * _ventana->_alto_px) / _ventana->_alto_log;
	pos_e = (int) pos_r;
	//cout << pos_r << endl;
	pos_e =  pos_e -_alto_px;
	return pos_e;
}

int Personaje::getWidth(Ventana* ventana, float ancho_log_capa)
{
	int ancho = 0;
	ancho = (float)(ancho_log_capa * ventana->_ancho_px) / ventana->_ancho_log ;
	return ancho;
}

int Personaje::getHeight(Ventana* ventana, float alto_log_capa)
{
	int alto = 0;
	alto = (float)(alto_log_capa * ventana->_alto_px) / ventana->_alto_log ;
	//cout << alto << endl;
	return alto;
}

bool Personaje::moveLeft(float factor)
{
	//cout << "Me intento mover " << factor << "y beta vale "<< getBeta(factor) << endl;
	//cout << "La pos actual es " << _pos_x << " el ancho logico es " << _ancho_log;
	//float new_x = _pos_x - ((_ancho_log / factor) + getAlpha(factor)) ; //viejo
	//cout << getBeta(factor) << endl;
	float new_x = _pos_x - ((factor) + getBeta(factor));
	//cout << " me intento mover a " << new_x << endl;
	if (new_x >= 0)
	{
		_pos_x = new_x;
		return false;
	}else{
		_pos_x = 0;
		return true;
	}
}

bool Personaje::moveRight(float factor)
{
	//cout << "Me intento mover " << factor << "y beta vale "<< getBeta(factor) << endl;
	//float new_x = _pos_x + (_ancho_log / factor) + getAlpha(factor); //viejo
	float new_x = _pos_x + factor + getBeta(factor);
	if (new_x + this->_ancho_log <= this->_ventana->_ancho_log)
	{
		_pos_x = new_x;
		return false;
	}else{
		_pos_x = this->_ventana->_ancho_log - this->_ancho_log;
		return true;
	}
}

float Personaje::getBeta(float factor)
{
	float ancho_escenario = this->_escenario.ancho;
	float ancho_capa = this->_ancho_log;
	float ancho_ventana = this->_ventana->_ancho_log;
	float beta = factor * (((ancho_capa - ancho_ventana)/(ancho_escenario - ancho_ventana) ) -1);
	//return beta;
	return 0;
}
