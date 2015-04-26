/*
 * Ventana.cpp
 *
 *  Created on: Apr 3, 2015
 *      Author: neo
 */
#include <Ventana.h>

Ventana::Ventana(int ancho_px, int alto_px, float ancho_log, float alto_log, EscenarioData escenario)
{
	this-> _ancho_px = ancho_px;
	this-> _alto_px = alto_px;
	this-> _ancho_log = ancho_log;
	this-> _alto_log = alto_log;
	this-> _pos_log_x = (escenario.ancho - _ancho_log) / 2;
	this->_escenario = escenario;
	this->_isShaking = false;
	int x,y;
	//Inicializacion ventana
	this -> _gWindow = SDL_CreateWindow( "Mortal Kombat - Taller", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _ancho_px, _alto_px, SDL_WINDOW_SHOWN );
	this -> _gRenderer = SDL_CreateRenderer( this->_gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	int imgFlags = IMG_INIT_PNG;
	IMG_Init( imgFlags );

	SDL_GetWindowPosition(_gWindow,&x,&y);
	_window_x = x;
	_window_y = y;

	this->_shakeIntensity = SHAKE_INTENSITY;
	this ->_shakeLenght = SHAKE_LENGTH;
}
Ventana::~Ventana()
{
	//cout << "Destroying window" << endl;
	SDL_DestroyRenderer( _gRenderer );
	SDL_DestroyWindow( _gWindow );
	//delete _gRenderer;
	//delete _gWindow;
	_gRenderer = NULL;
	_gWindow = NULL;
	IMG_Quit();
	SDL_Quit();
}

void Ventana::view()
{
	SDL_RenderPresent( this->_gRenderer );
}

void Ventana::clearScreen()
{
	SDL_SetRenderDrawColor( _gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( _gRenderer );
}

void Ventana::updateScreen()
{
	if(_isShaking)
	{
		this->shake();
	}
	SDL_RenderPresent( _gRenderer );
}

void Ventana::moveLeft(float factor)
{
	float new_x = this->_pos_log_x - factor;
	if (new_x <= 0)
	{
		new_x = 0;
	}
	_pos_log_x = new_x;
}

void Ventana::moveRight(float factor)
{
	float new_x = this->_pos_log_x + factor;
	if (new_x + this->_ancho_log >= _escenario.ancho)
	{
		new_x = _escenario.ancho - _ancho_log;
	}
	_pos_log_x = new_x;
}

void Ventana::toggleShake()
{
	this->_isShaking = true;
}

void Ventana::shake()
{
	int x = rand() % _shakeIntensity;
	int y = rand() % _shakeIntensity;
	SDL_SetWindowPosition(_gWindow,_window_x + x,_window_y + y);
	_shakeLenght--;
	if(_shakeLenght == 0)
	{
		SDL_SetWindowPosition(_gWindow,_window_x,_window_y);
		_shakeLenght = SHAKE_LENGTH;
		_isShaking = false;
	}
}
