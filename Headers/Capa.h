/*
 * Capa.h
 *
 *  Created on: Apr 5, 2015
 *      Author: neo
 */

#ifndef CAPA_H_
#define CAPA_H_
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <json.h>
#include <Logger.h>
//#include <TextureHandler.h>
#include <Ventana.h>
#include <CapaData.h>
#include <EscenarioData.h>
#define ANIMATED_SPEED  10;
class Capa
{
	private:
		//Atributos
		TextureHandler* _handler;
		Ventana* _ventana;
		EscenarioData _escenario;
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
		int cantSprites;
		int animDelay;
		bool animated;
		void viewAnimated();
		SDL_Rect* sprites;
		SDL_Rect* loadMedia(CapaData data, int height);
		int _lastFrame;

	public:
		Capa(Ventana* ventana, CapaData data, EscenarioData escenario);
		~Capa();
		void view();
		void moveLeft(float factor);
		void moveRight(float factor);
		void resetPos();
};



#endif /*CAPA_H_ */
