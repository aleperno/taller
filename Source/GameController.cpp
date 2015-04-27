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
	delete this->_personaje1;
	_personaje1 = NULL;
	delete this->_personaje2;
	_personaje2 = NULL;
	delete _instance;
	_instance = NULL;
	Logger::Instance()->log(DEBUG,"Se destruye instancia de GameController");
}

void GameController::prepararHUD()
{
	int hudExternH = _ventana->_alto_px/15;
	int hudExternW = _ventana->_ancho_px*2/5;
	int hudInternH = _ventana->_alto_px/20;
	int hudInternW = _ventana->_ancho_px*20/53;

	this->hud1.externo.h = hudExternH;
	this->hud1.externo.w = hudExternW;
	this->hud1.externo.x = 0;
	this->hud1.externo.y = 0;

	this->hud2.externo.h = hudExternH;
	this->hud2.externo.w = hudExternW;
	this->hud2.externo.x = _ventana->_ancho_px - hudExternW;
	this->hud2.externo.y = 0;

	this->hud1.interno.h = hudInternH;
	this->hud1.interno.w = hudInternW;
	this->hud1.interno.x = (hudExternW-hudInternW)/2;
	this->hud1.interno.y = (hudExternH-hudInternH)/2;

	this->hud2.interno.h = hudInternH;
	this->hud2.interno.w = hudInternW;
	this->hud2.interno.x = _ventana->_ancho_px - (hudExternW-hudInternW)/2 - hudInternW;
	this->hud2.interno.y = (hudExternH-hudInternH)/2;

	this->hud1.health = this->hud1.interno;
	this->hud2.health = this->hud2.interno;
}

GameController::GameController(Parser* parser)
{
	//Initialize SDL
	if(!iniciarSDL())
		Logger::Instance()->log(ERROR,"SDL could not initialize!");
	else
		Logger::Instance()->log(DEBUG,"Joysticks detectados: " + StringUtil::int2string(SDL_NumJoysticks()));
	_ventana = GameController::getVentana(parser);
	_escenario = GameController::getEscenario(parser);
	_capas = GameController::getCapas(_ventana,parser,_escenario);
	_personaje1 = GameController::getPersonaje(_ventana,parser,_escenario,1);
	_personaje2 = GameController::getPersonaje(_ventana,parser,_escenario,2);
	this->prepararHUD();
	_end_of_game = false;
	Logger::Instance()->log(DEBUG,"Se crea instancia de GameController");
}

bool GameController::iniciarSDL() {
	return (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) >= 0 );
}

Personaje* GameController::getPersonaje(Ventana* ventana,Parser* parser, EscenarioData escenario, int numero)
{
	Personaje* pers;
	switch (numero) {
	case 1:
		pers = new Personaje(ventana,parser->personaje1,escenario);
		break;
	case 2:
		pers = new Personaje(ventana,parser->personaje2,escenario);
		break;
	}
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

void GameController::actualizarHealthbars() {
	this->hud1.health.w = this->hud1.interno.w * _personaje1->healthPoints / 100;
	this->hud2.health.w = this->hud2.interno.w * _personaje2->healthPoints / 100;
}

void GameController::printHUD() {

	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0xAA, 0xAA, 0xAA, 0xFF );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.externo) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.externo) );

	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0x00, 0x00, 0x00, 0xFF );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.interno) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.interno) );

	this->actualizarHealthbars();

	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0xFF, 0x00, 0x00, 0xFF );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.health) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.health) );

}

void GameController::printLayers()
{
	//LimpioPantalla
	this->_ventana->clearScreen();
	//this->_ventana->view();
	for (unsigned int i=0; i<_capas.size(); i++)
	{
		_capas[i]->view();
		if (this->_personaje1->_zIndex == (i))
		{
			this->_personaje1->view();
		}
	}
	if (this->_personaje1->_zIndex >= _capas.size())
	{
		this->_personaje1->view();
	}

	this->printHUD();

	//Solo para pruebas
	/*SDL_RenderDrawRect( _ventana->_gRenderer, &_personaje1->boundingBox );

	SDL_Rect wall;
    wall.x = 300;
    wall.y = 300;
    wall.w = 100;
    wall.h = 100;
	SDL_RenderDrawRect( _ventana->_gRenderer, &wall );

	bool colisiona = hayColision(_personaje1->boundingBox, wall);
	if(colisiona)
	{
		//Pruebo colision en una accion
		if(_personaje1->_isJumping)
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

bool GameController::endOfGame(SDL_Event* e)
{
	bool end_of_game = false;
	while( SDL_PollEvent( e ) != 0 )
	{
		//User requests quit
		if( e->type == SDL_QUIT )
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
		SDL_Event* e = new SDL_Event();
		this->printLayers();
		_end_of_game = this->endOfGame(e);
		this->getKeys();
		//SDL_Delay(sleep_time);
		//printf("La posicion de la ventana es %0.2f \n",_ventana->_pos_log_x);
		//viewWindowPosition();
		delete e;
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

	if(!iniciarSDL())
		Logger::Instance()->log(ERROR,"SDL could not initialize!");
	else
		Logger::Instance()->log(DEBUG,"Joysticks detectados: " + StringUtil::int2string(SDL_NumJoysticks()));
	_ventana = GameController::getVentana(parser);
	_escenario = GameController::getEscenario(parser);
	_capas = GameController::getCapas(_ventana,parser,_escenario);
	_personaje1 = GameController::getPersonaje(_ventana,parser,_escenario,1);
	_personaje2 = GameController::getPersonaje(_ventana,parser,_escenario,2);
	this->prepararHUD();
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
		this->_personaje1->jumpLeft(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_UP ] && currentKeyStates[ SDL_SCANCODE_RIGHT ])
	{
		this->_personaje1->jumpRight(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_UP ])
	{
		this->_personaje1->jump(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_DOWN ])
	{
		this->_personaje1->duck();
	}
	else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
	{
		//this-> moveLayersRight();
		if ( !this->_personaje1->isJumping() && !this->_personaje1->isFalling() )
		{
			this->_personaje1->moveLeft(MOV_FACTOR2);
		}
	}
	else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
	{
		//this-> moveLayersLeft();
		if ( !this->_personaje1->isJumping() && !this->_personaje1->isFalling() )
		{
			this->_personaje1->moveRight(MOV_FACTOR2);

		}
	}	else if( currentKeyStates[ SDL_SCANCODE_S ] )
	{
		_ventana->toggleShake();
	}
	else
	{
		_personaje1->idle();
	}
	_personaje1->continueAction(MOV_FACTOR_JMP,JMP_FACTOR);
	//Veo si debo correr las capas
	this->moveLayers();
}

void GameController::moveLayers()
{
	//Veo si debo mover las capas
	if( this->_personaje1->isRightMargin(WINDOW_MARGIN_TOLERANCE) && _personaje1->isWalking() )
	{
		this->moveLayersLeft(MOV_FACTOR2);
	}
	else if ( this->_personaje1->isRightMargin(WINDOW_MARGIN_TOLERANCE) && _personaje1->isJumpingRight() )
	{
		this->moveLayersLeft(MOV_FACTOR_JMP);
	}
	else if ( this->_personaje1->isLeftMargin(WINDOW_MARGIN_TOLERANCE) && _personaje1->isWalking() )
	{
		this-> moveLayersRight(MOV_FACTOR2);
	}
	else if ( this->_personaje1->isLeftMargin(WINDOW_MARGIN_TOLERANCE) && _personaje1->isJumpingLeft() )
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
