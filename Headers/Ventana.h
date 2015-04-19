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
#include <TextureHandler.h>
#include <EscenarioData.h>

class Ventana
{
	public:
		int _ancho_px;
		int _alto_px;
		//float _pos_x;
		float _ancho_log;
		float _alto_log;
		float _pos_log_x;
		Ventana(int ancho_px, int alto_px, float ancho_log, float alto_log, EscenarioData escenario);
		void view();
		~Ventana(); //Destructor
		void clearScreen();
		void updateScreen();
		SDL_Window* _gWindow;
		SDL_Renderer* _gRenderer;
		void moveLeft(float factor);
		void moveRight(float factor);
	private:
		EscenarioData _escenario;
};




#endif /* VENTANA_H_ */
