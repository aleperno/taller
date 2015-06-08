/*
 * GameController.h
 *
 *  Created on: Apr 3, 2015
 *      Author: neo
 */

#ifndef HEADERS_GAMECONTROLLER_H_
#define HEADERS_GAMECONTROLLER_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;
#include <json.h>
#include <Logger.h>
#include <Parser.h>
#include <Ventana.h>
#include <Capa.h>
#include <Personaje.h>
#include <PersonajeData.h>
#include <StringUtil.h>
#include <HUD.h>
#include <MainScreen.h>
#include <Temporizador.h>
#include <AI.h>
#include <SoundHandler.h>
#include <AccionesTracking.h>
#include <Carteles.h>


#define DEF_SLEEP_TIME 20
#define FIGHT_TIME_COUNTDOWN 200
#define BUFFER_WAIT_TIME 3

#define NO_MAINSCREEN 0
#define MAINSCREEN_INTRO 1
#define MAINSCREEN_MODE_SELECT 2
#define MAINSCREEN_PVP 3
#define MAINSCREEN_PVE 4
#define MAINSCREEN_TRAINING 5

#define OK 0
#define BACK 1
#define NAME 2
#define ERASE 3

#define MAX_NAME_LENGTH 30

//Analog joystick dead zone
#define JOYSTICK_DEAD_ZONE 8000

enum GameMode {
	PVP,
	PVE,
	TRAINING
};

class GameController
{
	protected:
		GameController(Parser* parser);

	private:
		//Atributos
		Carteles* _carteles;
		SoundHandler* musica;
		AI* ai_handler;
		GameMode tipo_juego;
		static GameController* _instance;
		MainScreen* _mainScreen;
		Ventana* _ventana;
		EscenarioData _escenario;
		vector<Capa*> _capas;

		//Esos son siempre iguales a unos 2 de abajo
		Personaje* _personaje1;
		Personaje* _personaje2;

		Personaje* _jugador1liukang;
		Personaje* _jugador1scorpion;
		Personaje* _jugador2liukang;
		Personaje* _jugador2liukangColor;
		Personaje* _jugador2scorpion;
		Personaje* _jugador2scorpionColor;

		bool _beginRound;
		bool _end_of_game;
		bool musica_pelea;
		SDL_Joystick* _joystickOne;
		SDL_Joystick* _joystickTwo;
		Sint16 lastJoyValue1X;
		Sint16 lastJoyValue1Y;
		Sint16 lastJoyValue2X;
		Sint16 lastJoyValue2Y;
		int _numJoysticks;
		bool _hayPlayer1;
		bool _hayPlayer2;
		Hud* _hud;
		bool minimizado;
		bool enMainScreen;
		bool partidaPreparada;
		int screen;
		int modeSelected;
		vector< vector<int> > perSelect;
		Temporizador* _fightTimer;
		int tiempoRemanente;
		int round;
		int personaje1Wins;
		int personaje2Wins;
		int tiempoRemanenteBuffer;
		Temporizador* _bufferTimer;
		int tiempoRemanenteTomaValida;
		Temporizador* _tomaValidaTimer;
		vector<string>* comboAUX;
		bool hayCombo;
		string nombreCombo;

		//Metodos
		static Ventana* getVentana(Parser* parser);
		static EscenarioData getEscenario(Parser* parser);
		static vector<Capa*> getCapas(Ventana* ventana,Parser* parser, EscenarioData escenario);
		void getPersonajes(Ventana* ventana, Parser* parser, EscenarioData escenario);
		void InicializarAI(int _difficulty);
		void prepararPartida();
		void prepararPartidaTraining();
		void actualizarPersonajes();
		void runPVP();
		void runPVE();
		void runTraining();
		void printLayers();
		bool endOfGame(SDL_Event* e);
		void toMainScreen();
		void resetearVentanaPersonajes();

		void getKeys();
		void moveLayersRight(float factor);
		void moveLayersLeft(float factor);
		void moveLayers(Personaje* pers, Personaje* otherPers);
		bool iniciarSDL();
		void iniciarEstructuraPerSelect();
		void setEndOfGame(bool value);
		bool actualizarGanador();
		bool actualizarGanadorTraining();
		void actualizarPartida();
		void continuarAccionesYMoverCapas();

		void procesarEventosMainScreenIntro(SDL_Event* e);
		void procesarEventosMainScreenModeSelect(SDL_Event* e);
		void procesarEventosMainScreenPVP(SDL_Event* e);
		void procesarEventosMainScreenPVE(SDL_Event* e);
		void procesarEventosMainScreenTraining(SDL_Event* e);

		void procesarEventos(SDL_Event* e);
		void procesarBotones(SDL_Event* e);
		void procesarAxis(SDL_Event* e);
		void procesarMovimientoJoystick();

		bool hayPlayer1();
		bool hayPlayer2();
		void setPlayer1(bool value);
		void setPlayer2(bool value);
		void getKeysPlayer1();
		void getKeysPlayer2();

		int filaP1;
		int columnaP1;
		int filaP2;
		int columnaP2;
		int textFocus;
		int botonSeleccionadoEnModo;

		string nombreP1;
		string nombreP2;

		void procesamientoMainScreenIntro();
		void procesamientoMainScreenModeSelect();
		void procesamientoMainScreenPVP();
		void procesamientoMainScreenPVE();
		void procesamientoMainScreenTraining();

		//Testing
		void viewWindowPosition();
		bool canMoveRight(Personaje* pers, Personaje* otherPers);
		bool canMoveLeft(Personaje* pers, Personaje* otherPers);

	public:
		static GameController* Instance(Parser* parser = 0);
		void run();
		void KillController(); //Destructor
		bool estoyEnPVP() { return (this->tipo_juego == PVP);}
		bool estoyEnPVE() { return (this->tipo_juego == PVE);}
		bool estoyEnTraining() { return (this->tipo_juego == TRAINING);}
};

#endif /* HEADERS_GAMECONTROLLER_H_ */

