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
#include <StringUtil.h>
#include <HUD.h>


#define DEF_SLEEP_TIME 20


//Analog joystick dead zone
#define JOYSTICK_DEAD_ZONE 8000
//Font path


class GameController
{
	protected:
		GameController(Parser* parser);
	private:
		//Atributos
		Ventana* _ventana;
		EscenarioData _escenario;
		vector<Capa*> _capas;
		Personaje* _personaje1;
		Personaje* _personaje2;
		bool _end_of_game;
		SDL_Joystick* _joystickOne;
		SDL_Joystick* _joystickTwo;
		const char* _joystickOneID;
		const char* _joystickTwoID;
		int _numJoysticks;
		bool _hayPlayer1;
		bool _hayPlayer2;
		Hud* _hud;

		//Metodos
		bool hayColision( SDL_Rect boundingBox_1, SDL_Rect boundingBox_2 );
		static GameController* _instance;
		static Ventana* getVentana(Parser* parser);
		static EscenarioData getEscenario(Parser* parser);
		static vector<Capa*> getCapas(Ventana* ventana,Parser* parser, EscenarioData escenario);
		static Personaje* getPersonaje(Ventana* ventana,Parser* parser, EscenarioData escenario, bool pers_ppal);
		static Hud* getHud(Ventana* ventana, Personaje* personaje1, Personaje* personaje2);
		void printLayers();
		bool endOfGame(SDL_Event* e);
		void close();
		void reloadConfig();
		void getKeys();
		void moveLayersRight(float factor);
		void moveLayersLeft(float factor);
		void moveLayers(Personaje* pers, Personaje* otherPers);
		bool iniciarSDL();
		void setEndOfGame(bool value);
		void actualizarGanador();
		void procesarEventos(SDL_Event* e);
		void procesarBotones(SDL_Event* e);
		void procesarMovimientoJoystick();
		bool hayPlayer1();
		bool hayPlayer2();
		void setPlayer1(bool value);
		void setPlayer2(bool value);
		//Testing
		void viewWindowPosition();
		bool canMoveRight(Personaje* pers, Personaje* otherPers);
		bool canMoveLeft(Personaje* pers, Personaje* otherPers);

	public:
		static GameController* Instance(Parser* parser = 0);
		void run(int sleep_time = DEF_SLEEP_TIME);
		void KillController(); //Destructor
};

#endif /* HEADERS_GAMECONTROLLER_H_ */

