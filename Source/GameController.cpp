/*
 * GameController.cpp
 *
 *  Created on: Apr 3, 2015
 *      Author: neo
 */
#include <GameController.h>
#define MOV_FACTOR 300;   //Fraccion de la capa que se mueve por ciclo
#define MOV_FACTOR2 0.8   //
#define MOV_FACTOR_JMP 1.2
#define MOVE_P_FACTOR 1 //
#define JMP_FACTOR 2

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
	Logger::Instance()->log(DEBUG,"Se destruye instancia de GameController");
}

GameController::GameController(Parser* parser)
{
	_ventana = GameController::getVentana(parser);
	_escenario = GameController::getEscenario(parser);
	_capas = GameController::getCapas(_ventana,parser,_escenario);
	_personaje = GameController::getPersonaje(_ventana,parser,_escenario);
	_end_of_game = false;
	Logger::Instance()->log(DEBUG,"Se crea instancia de GameController");
}

Personaje* GameController::getPersonaje(Ventana* ventana,Parser* parser, EscenarioData escenario)
{
	Personaje* pers = new Personaje(ventana,parser->personaje1,escenario);
	return pers;
}

Ventana* GameController::getVentana(Parser* parser)
{
	int ancho_px = parser->ventana.ancho_px;
	int alto_px = parser->ventana.alto_px;
	float ancho_log = parser->ventana.ancho;
	float alto_log = parser->escenario.alto;
	Ventana* vent = new Ventana(ancho_px,alto_px,ancho_log,alto_log,parser->escenario);
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

	//Solo para pruebas
	/*SDL_RenderDrawRect( _ventana->_gRenderer, &_personaje->boundingBox );

	SDL_Rect wall;
    wall.x = 300;
    wall.y = 300;
    wall.w = 100;
    wall.h = 100;
	SDL_RenderDrawRect( _ventana->_gRenderer, &wall );

	bool colisiona = hayColision(_personaje->boundingBox, wall);
	if(colisiona)
	{
		//Pruebo colision en una accion
		if(_personaje->_isJumping)
			printf("X ");
	}*/
	//Fin solo para pruebas


	//ActualizoPantalla
	this->_ventana->updateScreen();
}


bool GameController::hayColision( SDL_Rect boundingBox_1, SDL_Rect boundingBox_2 )
{
	float factor_cercania = 0.25f;
	bool colision = true;

	//Calculo los lados de cada bounding box, izquierda, derecha, arriba y abajo
    int izqBB1 = boundingBox_1.x;
	int izqBB2 = boundingBox_2.x;
    int rgtBB1 = boundingBox_1.x + boundingBox_1.w;
	int rgtBB2 = boundingBox_2.x + boundingBox_2.w;;
    
	int upBB1 = boundingBox_1.y;
	int upBB2 = boundingBox_2.y;
	int downBB1 = boundingBox_1.y + boundingBox_1.h;
    int downBB2 = boundingBox_2.y + boundingBox_2.h;

	//Chequeo las condiciones de no-colision
    if( downBB1 <= upBB2 + factor_cercania * boundingBox_2.h )
        colision = false;
    if( upBB1 + factor_cercania * boundingBox_2.h >= downBB2 )
        colision = false;
    if( rgtBB1 <= izqBB2 + factor_cercania * boundingBox_2.w )
        colision = false;
    if( izqBB1 + factor_cercania * boundingBox_2.w >= rgtBB2 )
        colision = false;

	//Si no se da ninguna condicion de no-colision, entonces hay colision
    return colision;
}



void GameController::viewWindowPosition()
{
	int x,y;
	SDL_GetWindowPosition(_ventana->_gWindow,&x,&y);
	printf("La ventana esta en %d, %d \n",x,y);
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
	Logger::Instance()->log(DEBUG,"Comienzo ciclo de Juego");
	while (! _end_of_game)
	{
		SDL_Event e = *(new SDL_Event());
		this->printLayers();
		_end_of_game = this->endOfGame(e);
		this->getKeys();
		//SDL_Delay(sleep_time);
		//printf("La posicion de la ventana es %0.2f \n",_ventana->_pos_log_x);
		//viewWindowPosition();
	}
	this->close();
	Logger::Instance()->log(DEBUG,"Finaliza ciclo de Juego");
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
	Logger::Instance()->log(DEBUG,"Se recarga la Configuracion");
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
	else if(currentKeyStates[ SDL_SCANCODE_UP ] && currentKeyStates[ SDL_SCANCODE_LEFT ])
	{
		this->_personaje->jumpLeft(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_UP ] && currentKeyStates[ SDL_SCANCODE_RIGHT ])
	{
		this->_personaje->jumpRight(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_UP ])
	{
		this->_personaje->jump(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_DOWN ])
	{
		this->_personaje->duck();
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
	}	else if( currentKeyStates[ SDL_SCANCODE_S ] )
	{
		_ventana->toggleShake();
	}
	else
	{
		_personaje->idle();
	}
	_personaje->continueAction(MOV_FACTOR_JMP,JMP_FACTOR);
	//Veo si debo correr las capas
	this->moveLayers();
}

void GameController::moveLayers()
{
	//Veo si debo mover las capas
	if( this->_personaje->isRightMargin(WINDOW_MARGIN_TOLERANCE) && _personaje->isWalking() )
	{
		this->moveLayersLeft(MOV_FACTOR2);
	}
	else if ( this->_personaje->isRightMargin(WINDOW_MARGIN_TOLERANCE) && _personaje->isJumpingRight() )
	{
		this->moveLayersLeft(MOV_FACTOR_JMP);
	}
	else if ( this->_personaje->isLeftMargin(WINDOW_MARGIN_TOLERANCE) && _personaje->isWalking() )
	{
		this-> moveLayersRight(MOV_FACTOR2);
	}
	else if ( this->_personaje->isLeftMargin(WINDOW_MARGIN_TOLERANCE) && _personaje->isJumpingLeft() )
	{
		this-> moveLayersRight(MOV_FACTOR_JMP);
	}
}

void GameController::moveLayersRight(float factor)
{
	for (unsigned int i=0; i<_capas.size(); i++)
	{
		_capas[i]->moveRight(factor);
	}
	this->_ventana->moveLeft(factor);
}

void GameController::moveLayersLeft(float factor)
{
	for (unsigned int i=0; i<_capas.size(); i++)
	{
		_capas[i]->moveLeft(factor);
	}
	this->_ventana->moveRight(factor);
}
