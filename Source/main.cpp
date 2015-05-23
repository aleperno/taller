/*
 * main.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: neo
 */
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include <json.h>
#include <Logger.h>
#include <Parser.h>
#include <GameController.h>
#include <Ventana.h>

void parser(int argc, char* args[], int* lvl, string* path)
{
	string _path = DEFAULT_CONFIG_PATH;
	int _lvl = ERROR;
	for (int i=1; i< argc; ++i)
	{
		if (string(args[i]) == "--config")
		{
			//_path = (i+1 < argc) ? string(args[i++]) : DEFAULT_CONFIG_PATH;
			if (i+1 < argc)
			{
				_path = args[++i];
				//cout << _path << endl;
			}
		}else if(string(args[i]) == "--level")
		{
			//_lvl = (i+1 < argc) ? string(args[i++]) : ERROR;
			if (i+1 < argc)
			{
				string aux = string(args[++i]);
				if (aux == "ERROR" || aux == "error")
				{
					_lvl = ERROR;
				}else if (aux == "WARNING" || aux == "warning")
				{
					_lvl = WARNING;
				}else if (aux == "DEBUG" || aux == "debug")
				{
					_lvl = DEBUG;
				}
			}
		}
	}
	*path = _path;
	*lvl = _lvl;
}

int main( int argc, char* args[] )
{
	int lvl;
	string path;
	parser(argc,args,&lvl,&path);
	Logger logger = *Logger::Instance(lvl); //Instancio Logger con nivel ERROR
	logger.banner();
	logger.log(DEBUG,"Comienzo Programa");
	Parser::Initialize(path);
	Parser* parser = Parser::Instance();
	GameController* controlador = GameController::Instance(parser);
	controlador->run();
	logger.log(DEBUG,"Fin del programa");
	logger.KillLogger();
	Parser::KillInstance();
	controlador->KillController();
	return 0;
}
