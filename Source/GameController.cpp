/*
 * GameController.cpp
 *
 *  Created on: Apr 3, 2015
 *      Author: neo
 */
#include <GameController.h>
#define MOV_FACTOR 300; //Fraccion de la capa que se mueve por ciclo
#define MOV_FACTOR2 1.0//
#define MOVE_P_FACTOR 1.3//
#define JMP_FACTOR 3

GameController* GameController::_instance = 0;

GameController* GameController::Instance(Parser* parser)
{
	if (_instance == 0)
	{
		_instance = new GameController(parser);
	}
	return _instance;
}

void GameController::KillController()
{
	delete this->_ventana;
	_ventana = NULL;
	delete this->_personaje;
	_personaje = NULL;
	delete _instance;
	_instance = NULL;
}

GameController::GameController(Parser* parser)
{
	_ventana = GameController::getVentana(parser);
	_escenario = GameController::getEscenario(parser);
	_capas = GameController::getCapas(_ventana,parser,_escenario);
	_personaje = GameController::getPersonaje(_ventana,parser,_escenario);
	_end_of_game = false;
}

Personaje* GameController::getPersonaje(Ventana* ventana,Parser* parser, EscenarioData escenario)
{
	Personaje* pers = new Personaje(ventana,parser->personaje,escenario);
	return pers;
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

EscenarioData GameController::getEscenario(Parser* parser)
{
	return parser->escenario;
}

vector<Capa*> GameController::getCapas(Ventana* ventana,Parser* parser, EscenarioData escenario)
{
	vector<Capa*> capas;
	for (unsigned int i=0; i < parser->capas.size(); i++)
	{
		Capa* capa = new Capa(ventana,parser->capas[i],escenario);
		capas.push_back(capa);
	}
	return capas;
}

void GameController::printLayers()
{
	//LimpioPantalla
	this->_ventana->clearScreen();
	//this->_ventana->view();
	for (unsigned int i=0; i<_capas.size(); i++)
	{
		_capas[i]->view();
		if (this->_personaje->_zIndex == (i))
		{
			this->_personaje->view();
		}
	}
	if (this->_personaje->_zIndex >= _capas.size())
	{
		this->_personaje->view();
	}
	//ActualizoPantalla
	this->_ventana->updateScreen();
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
		SDL_Event e = *(new SDL_Event());
		this->printLayers();
		_end_of_game = this->endOfGame(e);
		this->getKeys();
		//SDL_Delay(sleep_time);
	}
	this->close();
}

void GameController::close()
{
	//this->_ventana->~Ventana();
	delete this->_ventana;
	this->_ventana = NULL;

	for ( vector<Capa*>::iterator i = _capas.begin(); i != _capas.end(); i++)
	{
		//delete *i;
		(*i)->~Capa();
	}
	_capas.clear();
}

void GameController::reloadConfig()
{
	Parser::Instance()->reload();
	Parser* parser = Parser::Instance();
	this->close();
	_ventana = GameController::getVentana(parser);
	_escenario = GameController::getEscenario(parser);
	_capas = GameController::getCapas(_ventana,parser,_escenario);
	_personaje = GameController::getPersonaje(_ventana,parser,_escenario);
}

void GameController::getKeys()
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
	if( currentKeyStates[ SDL_SCANCODE_ESCAPE ] )
	{
		_end_of_game = true;
	}
	else if( currentKeyStates[ SDL_SCANCODE_R ] )
	{
		this->reloadConfig();
	}
	else if(currentKeyStates[ SDL_SCANCODE_UP ])
	{
		this->_personaje->jump(JMP_FACTOR);
	}
	else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
	{
		//this-> moveLayersRight();
		if ( !this->_personaje->isJumping() && !this->_personaje->isFalling() )
		{
			this->_personaje->moveLeft(MOV_FACTOR2);
		}
	}
	else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
	{
		//this-> moveLayersLeft();
		if ( !this->_personaje->isJumping() && !this->_personaje->isFalling() )
		{
			this->_personaje->moveRight(MOV_FACTOR2);

		}
	}
	else
	{
		_personaje->idle();
	}
	_personaje->continueAction(MOV_FACTOR2/2,JMP_FACTOR);
	//Veo si debo correr las capas
	this->moveLayers();
}

void GameController::moveLayers()
{
	//Veo si debo mover las capas
	if(this->_personaje->isRightMargin() && (_personaje->isWalking() || _personaje->isJumpingRight()))
	{
		this->moveLayersLeft();
	}
	else if (this->_personaje->isLeftMargin() && (_personaje->isWalking() || _personaje->isJumpingLeft()))
	{
		this-> moveLayersRight();
	}
}

void GameController::moveLayersRight()
{
	float factor = (float) MOV_FACTOR2;
	for (unsigned int i=0; i<_capas.size(); i++)
	{
		_capas[i]->moveRight(factor);
	}
}

void GameController::moveLayersLeft()
{
	float factor = (float) MOV_FACTOR2;
	for (unsigned int i=0; i<_capas.size(); i++)
	{
		_capas[i]->moveLeft(factor);
	}
}
