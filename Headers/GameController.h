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


#define DEF_SLEEP_TIME 50

class GameController
{
	protected:
		GameController(Parser* parser);
	private:
		//Atributos
		Ventana* _ventana;
		EscenarioData _escenario;
		vector<Capa*> _capas;
		Personaje* _personaje;
		bool _end_of_game;

		//Metodos
		static GameController* _instance;
		static Ventana* getVentana(Parser* parser);
		static EscenarioData getEscenario(Parser* parser);
		static vector<Capa*> getCapas(Ventana* ventana,Parser* parser, EscenarioData escenario);
		static Personaje* getPersonaje(Ventana* ventana,Parser* parser, EscenarioData escenario);
		void printLayers();
		bool endOfGame(SDL_Event e);
		void close();
		void reloadConfig();
		void getKeys();
		void moveLayersRight();
		void moveLayersLeft();

	public:
		static GameController* Instance(Parser* parser = 0);
		void run(int sleep_time = DEF_SLEEP_TIME);
		void KillController(); //Destructor
};

#endif /* HEADERS_GAMECONTROLLER_H_ */