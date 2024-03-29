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
#include <PosicionesFilaSprites.h>
#include <CombosPersonaje.h>
#include <SoundHandler.h>
#include <AccionesTracking.h>
#include <Temporizador.h>

#define HEALTH 50;
#define SPEED 3;
#define JMP_SPEED 10;
#define JMP_SPEED2 5;
#define JMP_SPEED_X 3;
//#define ARMA_SPEED 2.5;

#define ITERACIONES_FATALITY 100;

class Personaje
{
	private:
		//Atributos
		SoundHandler* efectos_sonido;
		TextureHandler* _handler;
		TextureHandler* _handlerFatalities;
		TextureHandler* _handlerSkeleton;
		Ventana* _ventana;
		EscenarioData _escenario;
		PersonajeData _data;
		Arma* arma;
		float arma_speed;
		float _alto_log;
		unsigned int bufferTeclasSize;
		
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
		vector<SDL_Rect*> vectorSpritesFatalities;
		vector<SDL_Rect*> loadVectorMedia(PersonajeData data);
		void loadFatalitiesMedia();
		
		int _lastFrame;
		int _iterFatality;

		void setIdle();

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

		bool _isHitFalling;
		bool _isSweepFall;

		// For throwable weapon.
		bool _isThrowing;
		bool _weaponInAir;
		int _timesThrow;

		int pos_last_action;
		bool _beingHit;

		bool _receivingFatality;
		bool _apllyingFatality;
		bool _receivingBabality;
		bool _apllyingBabality;

		bool _fatalityApplied;
		bool _babalityApplied;

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
		void viewFallSweep();

		// VIEWs Fatalities
		void viewBaby(bool* flag);
		void viewBabyStance();
		void viewSkeleton(bool* flag);
		void viewSkeletonStance();

		void viewHit();
		void viewFall();
		void viewShotWeapon(size_t posicion);
		void setBoundingBox();
		void resetearArma();

		Temporizador* t_lanza_arma;

		//Testing
		bool first_cycle;
		void cancelActions();

	public:
		string nombreCombo;
		vector<string> bufferTeclas;
		Temporizador* bufferTimer;
		CombosPersonaje* combos;
		vector<string> track_movimientos;
		CombosPersonaje* getCombos() {return this->combos;}
		void actualizarBufferTeclas();
		float _ancho_log;
		float _pos_x;
		bool _isDizzy;
		bool _isIdle;

		bool isMaxPushLeft();
		bool isMaxPushRight();

		bool _orientacion;
		bool pers_ppal;
		bool hayColision( SDL_Rect boundingBox_1, SDL_Rect boundingBox_2 );
		Personaje(Ventana* ventana, PersonajeData data, EscenarioData escenario, bool pers_ppal, bool cambiarColor);
		~Personaje();
		int healthPoints;
		void downLife(int cantidad);
		PersonajeData _personajeData;
		void view(Personaje* otherPlayer);

		void posicionarParaMain();
		void resetear();

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
		void fall(int life);
		void fallSwep(int life);
		void dead();
		void barrer();
		void golpeBajo();
		void golpeAlto();
		void patadaBaja();
		void patadaAlta();
		void hit(int life);

		void applyFatality();
		bool receiveFatality();
		void applyBabality();
		bool receiveBabality();

		bool aplyingFatality();

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

		bool appliedFatality();
		bool appliedBabality();

		void evaluarAccion(int accion, bool enPVE);
		void evaluarAccion(int accion);
		bool evaluarCombo();
		bool canMove();
		void freeze();
		void unFreeze();

		PersonajeData* getData();
		void continueAction(float factor_x, float factor_y, Personaje* otherPers);
		void setOrientacion(bool orientacion);
		SDL_Rect boundingBox;
		void lanzarArma();

		bool canMakeFatality;
};



#endif /*PERSONAJE_H_ */
