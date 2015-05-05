/*
 * GameController.cpp
 *
 *  Created on: Apr 3, 2015
 *      Author: neo
 */
#include <GameController.h>
#define MOV_FACTOR 300;   //Fraccion de la capa que se mueve por ciclo
#define MOV_FACTOR2 0.8   //
#define MOV_FACTOR_JMP 2
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

GameController::GameController(Parser* parser)
{
	this->_joystickOne = NULL;
	this->_joystickTwo = NULL;
	this->_hayPlayer1 = false;
	this->_hayPlayer2 = false;
	if(!iniciarSDL()) {
		Logger::Instance()->log(ERROR,"SDL could not initialize!");
		Logger::Instance()->log(ERROR,"Joysticks detectados: " + StringUtil::int2string(this->_numJoysticks));
	} else {
		Logger::Instance()->log(DEBUG,"Joysticks detectados: " + StringUtil::int2string(this->_numJoysticks));
	}
	_ventana = GameController::getVentana(parser);
	_escenario = GameController::getEscenario(parser);
	_capas = GameController::getCapas(_ventana,parser,_escenario);
	_personaje1 = GameController::getPersonaje(_ventana,parser,_escenario,true);
	_personaje2 = GameController::getPersonaje(_ventana,parser,_escenario,false);
	_hud = GameController::getHud(_ventana, _personaje1, _personaje2);
	_end_of_game = false;
	Logger::Instance()->log(DEBUG,"Se crea instancia de GameController");
}

bool GameController::iniciarSDL() {
	bool flag =  (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) >= 0 );
	this->_numJoysticks = SDL_NumJoysticks();
	this->_joystickOne = NULL;
	this->_joystickTwo = NULL;
	if (this->_numJoysticks > 0 ) {
				this->_joystickOne = SDL_JoystickOpen(0);
				if (this->_joystickOne) {
					this->_hayPlayer1 = true;
				} else {
					this->_hayPlayer1 = false;
					Logger::Instance()->log(ERROR,"No se ha iniciado el Joystick del player 1");
				}
			}
			if (this->_numJoysticks == 2) {
				this->_joystickTwo = SDL_JoystickOpen(1);
				if (this->_joystickTwo) {
					this->_hayPlayer2 = true;
				} else {
					this->_hayPlayer2 = false;
					Logger::Instance()->log(ERROR,"No se ha iniciado el Joystick del Player 2");
				}
			}
	return flag;
}

Hud* GameController::getHud(Ventana* ventana, Personaje* personaje1, Personaje* personaje2)
{
	Hud* hud = new Hud(ventana, personaje1, personaje2);
	return hud;
}

Personaje* GameController::getPersonaje(Ventana* ventana,Parser* parser, EscenarioData escenario, bool pers_ppal)
{
	Personaje* pers;
	if (pers_ppal)
	{
		pers = new Personaje(ventana,parser->personaje1,escenario,pers_ppal,false);
	}else{
		if (parser->personaje1.nombre == parser->personaje2.nombre)
			pers = new Personaje(ventana,parser->personaje2,escenario,pers_ppal,true);
		else
			pers = new Personaje(ventana,parser->personaje2,escenario,pers_ppal,false);
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

void GameController::printLayers()
{
	//LimpioPantalla
	this->_ventana->clearScreen();
	//this->_ventana->view();
	if (_ventana->isShaking())
		_ventana->setShakeIntensity();

	for (unsigned int i=0; i<_capas.size(); i++)
	{
		_capas[i]->view();
		if (this->_personaje1->_zIndex == (i))
		{
			this->_personaje1->view(_personaje2);
			this->_personaje2->view(_personaje1);
		}
	}
	if (this->_personaje1->_zIndex >= _capas.size())
	{
		this->_personaje1->view(_personaje2);
		this->_personaje2->view(_personaje1);
	}

	this->_hud->printHUD();

	//Solo para pruebas
	/*SDL_RenderDrawRect( _ventana->_gRenderer, &_personaje1->boundingBox );
    SDL_RenderDrawRect( _ventana->_gRenderer, &_personaje2->boundingBox );
	
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

void GameController::setEndOfGame(bool value) {
	 this->_end_of_game = value;
}

void GameController::actualizarGanador() {
	if (this->_personaje1->healthPoints <= 0) {
		Logger::Instance()->log(WARNING,"Gano personaje 2.");
		this->reloadConfig();
	} else {
		if (this->_personaje2->healthPoints <= 0) {
			Logger::Instance()->log(WARNING,"Gano personaje 1.");
			this->reloadConfig();
		}
	}
}

void GameController::procesarBotones(SDL_Event* e) {
	Logger::Instance()->log(DEBUG,"Joystick # " + StringUtil::int2string(e->jdevice.which) + " pressed " + StringUtil::int2string(e->jbutton.button));
		if (e->jdevice.which == 0) {
		switch (e->jbutton.button) {
		case 0 :
			cout << "Player 1 apreto 0" << endl;
			break;
		case 1 :
			cout << "Player 1 apreto 1" << endl;
			break;
		case 2 :
			cout << "Player 1 apreto 2" << endl;
			break;
		case 3 :
			cout << "Player 1 apreto 3" << endl;
			break;
		case 4 :
			cout << "Player 1 apreto 4" << endl;
			break;
		case 5 :
			cout << "Player 1 apreto 5" << endl;
			break;
		}
	}
	else if (e->jdevice.which == 1) {
		switch (e->jbutton.button) {
				case 0 :
					cout << "Player 2 apreto 0" << endl;
					break;
				case 1 :
					cout << "Player 2 apreto 1" << endl;
					break;
				case 2 :
					cout << "Player 2 apreto 2" << endl;
					break;
				case 3 :
					cout << "Player 2 apreto 3" << endl;
					break;
				case 4 :
					cout << "Player 2 apreto 4" << endl;
					break;
				case 5 :
					cout << "Player 2 apreto 5" << endl;
					break;
				}
	}
}

void GameController::procesarEventos(SDL_Event* e) {
		switch (e->type) {
		case SDL_JOYBUTTONDOWN:
			this->procesarBotones(e);
			break;
		case SDL_JOYDEVICEADDED:
			Logger::Instance()->log(DEBUG,"Se ha enchufado un Joystick");
			break;
		case SDL_JOYDEVICEREMOVED:
			Logger::Instance()->log(ERROR,"Se ha desenchufado un Joystick");
			break;
		case SDL_KEYDOWN:
			if (e->key.keysym.sym == SDLK_r) this->reloadConfig();
			else if (e->key.keysym.sym == SDLK_ESCAPE) this->_end_of_game = true;
			else if (e->key.keysym.sym == SDLK_v) this->_ventana->toggleShake();
			else if (e->key.keysym.sym == SDLK_1) this->_personaje1->healthPoints -= 10;
			else if (e->key.keysym.sym == SDLK_2) this->_personaje2->healthPoints -= 10;
			break;
 	}
}

void GameController::procesarMovimientoJoystick() {
	if (this->hayPlayer1()) {
		const Sint16 AXSP1 = SDL_JoystickGetAxis(this->_joystickOne,0);
		const Sint16 AYSP1 = SDL_JoystickGetAxis(this->_joystickOne,1);

		if (AXSP1 < 0 && AYSP1 < 0) {
			this->_personaje1->jumpLeft(JMP_FACTOR);
 		}
		else if (AXSP1 > 0 && AYSP1 < 0) {
			this->_personaje1->jumpRight(JMP_FACTOR);
		}
		else if (AYSP1 < 0) {
			this->_personaje1->jump(JMP_FACTOR);
		}
		else if (AYSP1 > 0) {
			this->_personaje1->duck();
		}
		else if (AXSP1 < 0) {
			if ( !this->_personaje1->isJumping() && !this->_personaje1->isFalling() && canMoveLeft(_personaje1,_personaje2) ) {
				this->_personaje1->moveLeft(MOV_FACTOR2);
 			}
		}
		else if (AXSP1 > 0) {
			if ( !this->_personaje1->isJumping() && !this->_personaje1->isFalling() && canMoveRight(_personaje1,_personaje2) ) {
				this->_personaje1->moveRight(MOV_FACTOR2);
			}
		} else {
			this->_personaje1->idle();
		}
	}
	if (this->hayPlayer2()) {
		const Sint16 AXSP2 = SDL_JoystickGetAxis(this->_joystickTwo,0);
		const Sint16 AYSP2 = SDL_JoystickGetAxis(this->_joystickTwo,1);
		if (AXSP2 < 0 && AYSP2 < 0) {
			this->_personaje2->jumpLeft(JMP_FACTOR);
		}
		else if (AXSP2 > 0 && AYSP2 < 0) {
			this->_personaje2->jumpRight(JMP_FACTOR);
		}
		else if (AYSP2 < 0) {
			this->_personaje2->jump(JMP_FACTOR);
		}
		else if (AYSP2 > 0) {
			this->_personaje2->duck();
		}
		else if (AXSP2 < 0) {
			if ( !this->_personaje2->isJumping() && !this->_personaje2->isFalling() && canMoveLeft(_personaje2,_personaje1)) {
				this->_personaje2->moveLeft(MOV_FACTOR2);
 			}
 		}
		else if (AXSP2 > 0) {
			if ( !this->_personaje2->isJumping() && !this->_personaje2->isFalling() && canMoveRight(_personaje2,_personaje1)) {
				this->_personaje2->moveRight(MOV_FACTOR2);
			}
		} else {
			this->_personaje2->idle();
		}
 	}
}

bool GameController::canMoveLeft(Personaje* pers, Personaje* otherPers)
{
	if(otherPers->isRightMargin() && pers->isLeftMargin())
		return false;
	return true;
}

bool GameController::canMoveRight(Personaje* pers, Personaje* otherPers)
{
	return !(otherPers->isLeftMargin() && pers->isRightMargin());
}


void GameController::run(int sleep_time)
{
	SDL_Event e;
	Logger::Instance()->log(DEBUG,"Comienzo ciclo de Juego");
	while (!this->_end_of_game)
	{
		while( SDL_PollEvent(&e) != 0 )
		{
			if( e.type == SDL_QUIT ) this->setEndOfGame(true);
			this->procesarEventos(&e);
		}
		this->procesarMovimientoJoystick();
 		this->getKeys();
		_personaje1->continueAction(MOV_FACTOR_JMP,JMP_FACTOR,_personaje2);
		_personaje2->continueAction(MOV_FACTOR_JMP,JMP_FACTOR,_personaje1);
		this->moveLayers(_personaje1,_personaje2);
		this->moveLayers(_personaje2,_personaje1);
		this->actualizarGanador();
		this->printLayers();
	}
	this->close();
	Logger::Instance()->log(DEBUG,"Finaliza ciclo de Juego");
}

bool GameController::hayPlayer1() {
	return this->_hayPlayer1;
}

void GameController::setPlayer1(bool value) {
	this->_hayPlayer1 = value;
}

void GameController::setPlayer2(bool value) {
	this->_hayPlayer2 = value;
}

bool GameController::hayPlayer2() {
	return this->_hayPlayer2;
}

void GameController::close()
{
	if (hayPlayer1()) {
		SDL_JoystickClose(this->_joystickOne);
		this->_joystickOne = NULL;
		this->setPlayer1(false);
	}
	if (hayPlayer2()) {
		SDL_JoystickClose(this->_joystickTwo);
		this->_joystickTwo = NULL;
		this->setPlayer2(false);
	}

	delete(_hud);

	delete this->_ventana;
	this->_ventana = NULL;
	
	delete this->_personaje1;
	this->_personaje1 = NULL;
	delete this->_personaje2;
	this->_personaje2 = NULL;

	for(std::size_t i=0; i<this->_capas.size(); ++i) {
		delete this->_capas[i];
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
	_personaje1 = GameController::getPersonaje(_ventana,parser,_escenario,true);
	_personaje2 = GameController::getPersonaje(_ventana,parser,_escenario,false);
	_hud = GameController::getHud(_ventana, _personaje1, _personaje2);
}

void GameController::getKeysPlayer2() {

	const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
	if(currentKeyStates[ SDL_SCANCODE_W ] && currentKeyStates[ SDL_SCANCODE_A ])
	{
		this->_personaje2->jumpLeft(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_W ] && currentKeyStates[ SDL_SCANCODE_D ])
	{
		this->_personaje2->jumpRight(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_9 ])
	{
		this->_personaje2->dizzy();
	}
	else if(currentKeyStates[ SDL_SCANCODE_W ])
	{
		this->_personaje2->jump(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_S] && currentKeyStates[ SDL_SCANCODE_E ])
	{
		this->_personaje2->blockDuck();
	}
	else if(currentKeyStates[ SDL_SCANCODE_S ])
	{
		this->_personaje2->duck();
	}
	else if(currentKeyStates[ SDL_SCANCODE_E ])
	{
		this->_personaje2->block();
	}
	else if( currentKeyStates[ SDL_SCANCODE_A ] )
	{
		if ( !this->_personaje2->isJumping() && !this->_personaje2->isFalling() && canMoveLeft(_personaje2,_personaje1))
		{
			this->_personaje2->moveLeft(MOV_FACTOR2);
		}
	}
	else if( currentKeyStates[ SDL_SCANCODE_D ] )
	{
		if ( !this->_personaje2->isJumping() && !this->_personaje2->isFalling() && canMoveRight(_personaje2,_personaje1) )
		{
			this->_personaje2->moveRight(MOV_FACTOR2);
		}
	} else 	{
		if (!this->hayPlayer2())_personaje2->idle();
	}
}

void GameController::getKeysPlayer1() {

	const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
	if(currentKeyStates[ SDL_SCANCODE_UP ] && currentKeyStates[ SDL_SCANCODE_LEFT ])
	{
		this->_personaje1->jumpLeft(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_UP ] && currentKeyStates[ SDL_SCANCODE_RIGHT ])
	{
		this->_personaje1->jumpRight(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_0 ])
	{
		this->_personaje1->dizzy();
	}
	else if(currentKeyStates[ SDL_SCANCODE_UP ])
	{
		this->_personaje1->jump(JMP_FACTOR);
	}
	//Prueba de lanzamiento
	else if(currentKeyStates[ SDL_SCANCODE_L ])
	{
		this->_personaje1->lanzarArma();
	}
	//Fin prueba lanzamiento
	else if(currentKeyStates[ SDL_SCANCODE_DOWN ] && currentKeyStates[ SDL_SCANCODE_B ])
	{
		this->_personaje1->blockDuck();
	}
	else if(currentKeyStates[ SDL_SCANCODE_DOWN ])
	{
		this->_personaje1->duck();
	}
	else if(currentKeyStates[ SDL_SCANCODE_B ])
	{
		this->_personaje1->block();
	}
	else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
	{
		if ( !this->_personaje1->isJumping() && !this->_personaje1->isFalling() && canMoveLeft(_personaje1,_personaje2) )
		{
			this->_personaje1->moveLeft(MOV_FACTOR2);
		}
	}
	else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
	{
		if ( !this->_personaje1->isJumping() && !this->_personaje1->isFalling() && canMoveRight(_personaje1,_personaje2))
		{
			this->_personaje1->moveRight(MOV_FACTOR2);
		}
	} else 	{
		if (!this->hayPlayer1())_personaje1->idle();
	}
}

/*
* Se mantiene el Keyboard scan para testing, en caso de no tener Joystick
* Solo se mapean acciones de Player1
*/

void GameController::getKeys()
{
	this->getKeysPlayer1();
	this->getKeysPlayer2();
}

void GameController::moveLayers(Personaje* pers, Personaje* otherPers)
{
	//Veo si debo mover las capas
	if( pers->isRightMargin() && pers->isWalking() && !otherPers->isLeftMargin() )
	{
		this->moveLayersLeft(MOV_FACTOR2);
	}
	else if ( pers->isRightMargin() && pers->isJumpingRight() && !otherPers->isLeftMargin() )
	{
		this->moveLayersLeft(MOV_FACTOR_JMP);
	}
	else if ( pers->isLeftMargin() && pers->isWalking() && !otherPers->isRightMargin() )
	{
		this-> moveLayersRight(MOV_FACTOR2);
	}
	else if ( pers->isLeftMargin() && pers->isJumpingLeft() && !otherPers->isRightMargin())
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

