/*
 * main.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: neo
 */
#include <SDL.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <json.h>
#include <Logger.h>
#include <Parser.h>
#include <GameController.h>
#include <Ventana.h>

int main( int argc, char* args[] )
{
	Logger logger = *Logger::Instance(ERROR); //Instancio Logger con nivel ERROR
	logger.banner();
	//logger.log(WARNING,"hola esto es un WARNING"); //Pruebo Loggergear un debug, no deberia aparecer
	if (argc == 1) {
		Parser::Initialize();
	} else {
		Parser::Initialize(args[1]);
	}
	Parser* parser = Parser::Instance();
	GameController* controlador = GameController::Instance(parser);
	controlador->run();
	logger.log(DEBUG,"Fin del programa");
	logger.KillLogger();
	Parser::KillInstance();
	controlador->KillController();
	return 0;
}
