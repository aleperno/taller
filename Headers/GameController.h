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


#define DEF_SLEEP_TIME 20
#define WINDOW_MARGIN_TOLERANCE 10

//Analog joystick dead zone
#define JOYSTICK_DEAD_ZONE 8000
//Font path
#define FONT_PATH "Images/ardestine.ttf"

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
		TTF_Font* font;
		struct hudPersonaje {
			SDL_Rect externo;
			SDL_Rect externoIlum;
			SDL_Rect interno;
			SDL_Rect healthIlum;
			SDL_Rect health;
			TextureHandler* nombreTexture;
			SDL_Rect nombre;
		};
		hudPersonaje hud1;
		hudPersonaje hud2;
		bool _end_of_game;
		SDL_Joystick* _joystickOne;
		SDL_Joystick* _joystickTwo;
		const char* _joystickOneID;
		const char* _joystickTwoID;
		int _numJoysticks;

		//Metodos
		bool hayColision( SDL_Rect boundingBox_1, SDL_Rect boundingBox_2 );
		static GameController* _instance;
		static Ventana* getVentana(Parser* parser);
		static EscenarioData getEscenario(Parser* parser);
		static vector<Capa*> getCapas(Ventana* ventana,Parser* parser, EscenarioData escenario);
		static Personaje* getPersonaje(Ventana* ventana,Parser* parser, EscenarioData escenario, int numero);
		void printLayers();
		void prepararHUD();
		void actualizarHealthbars();
		void printHUD();
		bool endOfGame(SDL_Event* e);
		void close();
		void reloadConfig();
		void getKeys();
		void moveLayersRight(float factor);
		void moveLayersLeft(float factor);
		void moveLayers();
		bool iniciarSDL();
		void setEndOfGame(bool value);
		void actualizarGanador();
		void procesarJoystick(SDL_Event* e);
		void procesarAxis(SDL_Event* e);
		void procesarBotones(SDL_Event* e);
		//Testing
		void viewWindowPosition();

	public:
		static GameController* Instance(Parser* parser = 0);
		void run(int sleep_time = DEF_SLEEP_TIME);
		void KillController(); //Destructor
};

#endif /* HEADERS_GAMECONTROLLER_H_ */
