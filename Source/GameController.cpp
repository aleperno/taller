/*
 * GameController.cpp
 *
 *  Created on: Apr 3, 2015
 *      Author: neo
 */
#include <GameController.h>

GameController* GameController::_instance = 0;

GameController* GameController::Instance(Parser* parser)
{
	if (_instance == 0)
	{
		_instance = new GameController(parser);
	}
	return _instance;
}

GameController::GameController(Parser* parser)
{
	_ventana = GameController::getVentana(parser);
	_end_of_game = false;
}

Ventana* GameController::getVentana(Parser* parser)
{
	int ancho_px = parser->ventana.ancho_px;
	int alto_px = parser->ventana.alto_px;
	float ancho_log = parser->ventana.ancho;
	float alto_log = parser->escenario.alto;
	Ventana* vent = new Ventana(ancho_px,alto_px,ancho_log,alto_log);
	return vent;
}

void GameController::printLayers()
{
	this->_ventana->clearScreen();
	this->_ventana->view();
}

bool GameController::endOfGame(SDL_Event e)
{
	bool end_of_game = false;
	while( SDL_PollEvent( &e ) != 0 )
	{
		//User requests quit
		if( e.type == SDL_QUIT )
		{
			end_of_game = true;
		}
	}
	return end_of_game;
}

void GameController::run(int sleep_time)
{
	while (! _end_of_game)
	{
		SDL_Event e;
		this->printLayers();
		_end_of_game = this->endOfGame(e);
		this->getKeys();
		SDL_Delay(sleep_time);
	}
	this->close();
}

void GameController::close()
{
	this->_ventana->~Ventana();
	delete this->_ventana;
	this->_ventana = NULL;
}

void GameController::reloadConfig()
{
	Parser::Instance()->reload();
	Parser* parser = Parser::Instance();
	this->close();
	_ventana = GameController::getVentana(parser);
}
void GameController::getKeys()
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
	if( currentKeyStates[ SDL_SCANCODE_ESCAPE ] )
	{
		_end_of_game = true;
	}else if( currentKeyStates[ SDL_SCANCODE_R ] )
	{
		this->reloadConfig();
	}
}
