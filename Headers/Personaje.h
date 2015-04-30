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

#define SPEED 3;
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
		float _alto_log;
		float _ancho_log;
		float _pos_x;
		float _pos_y;
		float _alto_px;
		float _ancho_px;
		int get_x_px();
		int get_y_px();
		float _factor_escala; //Factor de escala
		static int getWidth(Ventana* ventana,float ancho_log_capa);
		static int getHeight(Ventana* ventana, float alto_log_capa);
		float getAlpha(float factor);
		float getBeta(float factor);	
		SDL_Rect* sprites;
		SDL_Rect* loadMedia(PersonajeData data);
		int _lastFrame;		
		bool _isWalking;
		bool _isDucking;
		bool _isJumping;
		bool _isJumpingRight;
		bool _isJumpingLeft;
		bool _isFalling;
		bool _isFallingRight;
		bool _isFallingLeft;
		void viewWalking();
		void viewDuck();
		void viewJump();
		void viewJumpRight();
		void viewJumpLeft();
		void setBoundingBox();

	public:
		Personaje(Ventana* ventana, PersonajeData data, EscenarioData escenario);
		~Personaje();
		int healthPoints;
		PersonajeData _personajeData;
		void view();
		void moveLeft(float factor);
		void moveRight(float factor);
		void duck();
		void jump(float factor);
		void jumpRight(float factor);
		void jumpLeft(float factor);
		void idle();
		unsigned int _zIndex;
		void showIdle();
		bool isLeftMargin(float tolerance);
		bool isRightMargin(float tolerance);
		bool isJumping();
		bool isFalling();
		bool isMovingInJump();
		bool isWalking();
		bool isJumpingRight();
		bool isJumpingLeft();
		void continueAction(float factor_x, float factor_y);
		SDL_Rect boundingBox;
};



#endif /*PERSONAJE_H_ */
