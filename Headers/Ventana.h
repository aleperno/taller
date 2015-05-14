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
#include <cstdlib>
using namespace std;
#include <json.h>
#include <Logger.h>
#include <TextureHandler.h>
#include <EscenarioData.h>

#define SHAKE_INTENSITY 20
#define SHAKE_LENGTH 20
#define WINDOW_MARGIN_TOLERANCE 1

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
		void toggleShake();
		bool isShaking();
		int shake();
		void setShakeIntensity();

	private:
		EscenarioData _escenario;
		bool _isShaking;
		int _shakeLenght;
		int _shakeIntensity;
		int _window_x,_window_y;
		int _shakeInt;
};




#endif /* VENTANA_H_ */
