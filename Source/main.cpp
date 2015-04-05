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
	/*
	cout << Parser::Instance()->ventana.ancho_px << endl;
	cout << Parser::Instance()->ventana.alto_px << endl;
	cout << Parser::Instance()->ventana.ancho << endl << endl;

	cout << Parser::Instance()->escenario.ancho << endl;
	cout << Parser::Instance()->escenario.alto << endl;
	cout << Parser::Instance()->escenario.y_piso << endl << endl;

	cout << Parser::Instance()->personaje.ancho << endl;
	cout << Parser::Instance()->personaje.alto << endl;
	cout << Parser::Instance()->personaje.z_index << endl;
	cout << Parser::Instance()->personaje.orientacion << endl << endl;;

	cout << Parser::Instance()->spriteSheetPath << endl << endl;

	for (unsigned int i=0; i<Parser::Instance()->capas.size(); i++) {
		cout << Parser::Instance()->capas[i].imagen_fondo << endl;
		cout << Parser::Instance()->capas[i].ancho << endl;
	}

	cout << endl;
	cout << Parser::Instance()->personaje.height << endl;
	cout << Parser::Instance()->personaje.width << endl;
	cout << Parser::Instance()->personaje.walk[1] << endl;

	Parser::KillInstance();
	*/
	logger.log(DEBUG,"Fin del programa");
	return 0;
}
