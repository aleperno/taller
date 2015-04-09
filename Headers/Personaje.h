/*
 * Capa.h
 *
 *  Created on: Apr 5, 2015
 *      Author: neo
 */

#ifndef PERSONAJE_H_
#define PERSONAJE_H_
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#include <json.h>
#include <Logger.h>
//#include <TextureHandler.h>
#include <Ventana.h>
#include <PersonajeData.h>
#include <EscenarioData.h>

#define SPEED 5;

class Personaje
{
	private:
		//Atributos
		TextureHandler* _handler;
		Ventana* _ventana;
		EscenarioData _escenario;
		PersonajeData _personajeData;
		float _alto_log;
		float _ancho_log;
		float _alto_px;
		float _ancho_px;
		float _factor_escala; //Factor de escala
		float _pos_x;
		float _pos_y;
		static int getWidth(Ventana* ventana,float ancho_log_capa);
		static int getHeight(Ventana* ventana, float alto_log_capa);
		int get_x_px();
		int get_y_px();
		float getAlpha(float factor);
		float getBeta(float factor);
		SDL_Rect* sprites;
		SDL_Rect* loadMedia(PersonajeData data);
		int _lastFrame;

	public:
		Personaje(Ventana* ventana, PersonajeData data, EscenarioData escenario);
		~Personaje();
		void view();
		void moveLeft(float factor);
		void moveRight(float factor);
		unsigned int _zIndex;
		void showIdle();
		bool isLeftMargin();
		bool isRightMargin();
};



#endif /*PERSONAJE_H_ */
