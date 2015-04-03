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
using namespace std;
#include <json.h>
#include <Logger.h>
#include <Parser.h>
#include <Ventana.h>

#define DEF_SLEEP_TIME 50

class GameController
{
	protected:
		GameController(Parser* parser);
	private:
		static GameController* _instance;
		static Ventana* getVentana(Parser* parser);
		Ventana* _ventana;
		void printLayers();
		bool endOfGame(SDL_Event e);
		void close();
	public:
		static GameController* Instance(Parser* parser = 0);
		void run(int sleep_time = DEF_SLEEP_TIME);
		~GameController(); //Destructor
};

#endif /* HEADERS_GAMECONTROLLER_H_ */
