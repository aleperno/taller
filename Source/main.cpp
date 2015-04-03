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
#include <Log.h>
#include <Parser.h>

int main( int argc, char* args[] )
{
	Log logger = *Log::Instance(WARNING); //Instancio Log con nivel DEBUG
	logger.log(DEBUG,"hola esto es un DEBUG"); //Pruebo loggear un debug, no deberia aparecer
	return 0;
	if (argc == 1) {
		Parser::Initialize();
	} else {
		Parser::Initialize(args[1]);
	}

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

	Parser::KillInstance();

	return 0;
}
