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
#define JMP_SPEED 10;
#define JMP_SPEED2 5;
#define JMP_SPEED_X 3;

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
		bool _isWalking;
		bool _isJumping;
		bool _isJumpingRight;
		bool _isJumpingLeft;
		bool _isFalling;
		bool _isFallingRight;
		bool _isFallingLeft;
		bool _invert;
		void viewWalking();
		void viewJump();
		void viewJumpRight();
		void viewJumpLeft();

	public:
		Personaje(Ventana* ventana, PersonajeData data, EscenarioData escenario);
		~Personaje();
		void view();
		void moveLeft(float factor);
		void moveRight(float factor);
		void jump(float factor);
		void idle();
		unsigned int _zIndex;
		void showIdle();
		bool isLeftMargin();
		bool isRightMargin();
		bool isJumping();
		bool isFalling();
		bool isMovingInJump();
		bool isWalking();
		bool isJumpingRight();
		bool isJumpingLeft();
		void continueAction(float factor_x, float factor_y);
};



#endif /*PERSONAJE_H_ */
