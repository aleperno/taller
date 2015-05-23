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
#include <Arma.h>

#define SPEED 3;
#define JMP_SPEED 10;
#define JMP_SPEED2 5;
#define JMP_SPEED_X 3;
//#define ARMA_SPEED 2.5;

#define POS_FILA_WALK			0	// OK
#define POS_FILA_IDLE			1	// OK
#define POS_FILA_JUMP			2	// OK
#define POS_FILA_JMPF			3	// OK
#define POS_FILA_JMPB			4	// OK
#define POS_FILA_DUCK			5	// OK
#define POS_FILA_BLOCK			6	// OK
#define POS_FILA_BLOCKDUCK		7	// OK
#define POS_FILA_DIZZY			8	// OK
#define POS_FILA_HITTED_DUCK 	9	// OK
#define POS_FILA_HITTED 		10	// OK
#define POS_FILA_HIKICK_DUCK 	11	// OK
#define POS_FILA_HIKICK 		12	// OK
#define POS_FILA_LOKICK_DUCK	13	// OK
#define POS_FILA_LOKICK			14	// OK
#define POS_FILA_AIRKICK		15	// OK
#define POS_FILA_HIPUNCH		16	// OK
#define POS_FILA_LOPUNCH		17	// OK
#define POS_FILA_AIRPUNCH		18	// OK
#define POS_FILA_FALLDEAD		19
#define POS_FILA_FALL			20
#define POS_FILA_GANCHO			21	// OK
#define POS_FILA_BARRIDO		22
#define POS_FILA_ARMA			23	// OK
#define POS_FILA_LOPUNCH_DUCK	24	// OK

class Personaje
{
	private:
		//Atributos
		TextureHandler* _handler;
		Ventana* _ventana;
		EscenarioData _escenario;
		PersonajeData _data;
		Arma* arma;
		float arma_speed;
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
		vector<SDL_Rect*> vectorSprites;
		vector<SDL_Rect*> loadVectorMedia(PersonajeData data);
		int _lastFrame;

		bool _isWalking;
		bool _isDucking;
		bool _isJumping;
		bool _isJumpingRight;
		bool _isJumpingLeft;
		bool _isFalling;
		bool _isFallingRight;
		bool _isFallingLeft;
		bool _canMove;
		bool _isHiKicking;
		bool _isLoKicking;
		bool _isHiPunching;
		bool _isLoPunching;
		bool _isBarriendo;
		bool _isBlocking;
		bool _isDizzy;
		bool _isHitFalling;
		

		// For throwable weapon.
		bool _isThrowing;
		bool _weaponInAir;
		int _timesThrow;

		int pos_last_action;
		bool _beingHit;

		void viewWalking();
		void viewDuck();
		void viewDizzy();
		void viewBlock();
		void viewBlockDuck();
		void viewJump();
		void viewJumpRight();
		void viewJumpLeft();

		bool viewHiKick();
		bool viewHiPunch();
		bool viewLoKick();
		bool viewLoPunch();
		bool viewPunchAir();
		bool viewKickAir();
		bool viewBarrido();

		void viewHit();
		void viewFall();
		void viewShotWeapon(size_t posicion);
		void setBoundingBox();
		void resetearArma();

	public:
		bool isMaxPushLeft();
		bool isMaxPushRight();

		bool _orientacion;
		bool hayColision( SDL_Rect boundingBox_1, SDL_Rect boundingBox_2 );
		Personaje(Ventana* ventana, PersonajeData data, EscenarioData escenario, bool pers_ppal, bool cambiarColor);
		~Personaje();
		int healthPoints;
		void downLife(int cantidad);
		PersonajeData _personajeData;
		void view(Personaje* otherPlayer);

		void resetear(bool pers_ppal);

		void moveLeft(float factor);
		void moveRight(float factor);

		void duck();
		void jump(float factor);
		void jumpRight(float factor);
		void jumpLeft(float factor);
		void idle();
		void block();
		void blockDuck();
		void dizzy();
		void winingPosition();
		void arrojarArma();
		void fall(int life);
		void dead();
		void barrer();
		void golpeBajo();
		void golpeAlto();
		void patadaBaja();
		void patadaAlta();
		void hit(int life);

		unsigned int _zIndex;
		void showIdle();
		bool viewDead();
		bool isBlocking();
		bool isLeftMargin();
		bool isRightMargin();
		bool isJumping();
		bool isFalling();
		bool isMovingInJump();
		bool isWalking();
		bool isJumpingRight();
		bool isJumpingLeft();
		bool isDucking();
		bool isHitting();

		void evaluarAccion (int accion);
		bool canMove();
		void freeze();
		void unFreeze();

		PersonajeData* getData();
		void continueAction(float factor_x, float factor_y, Personaje* otherPers);
		void setOrientacion(bool orientacion);
		SDL_Rect boundingBox;
		void lanzarArma();
};



#endif /*PERSONAJE_H_ */
