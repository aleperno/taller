/*
 * Ventana.h
 *
 *  Created on: Apr 3, 2015
 *      Author: neo
 */

#ifndef VENTANA_H_
#define VENTANA_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <json.h>
#include <Logger.h>
#include "TextureHandler.h"
class Ventana
{
	public:
		int _ancho_px;
		int _alto_px;
		float _ancho_log;
		float _alto_log;
		Ventana(int ancho_px, int alto_px, float ancho_log, float alto_log);
		void view();
		~Ventana(); //Destructor
		void clearScreen();
		void updateScreen();
		SDL_Window* _gWindow;
		SDL_Renderer* _gRenderer;
};




#endif /* VENTANA_H_ */
