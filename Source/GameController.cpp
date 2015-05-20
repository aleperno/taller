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
	delete this->_mainScreen;
	_mainScreen = NULL;
	delete this->_ventana;
	_ventana = NULL;
	delete this->_personaje1;
	_personaje1 = NULL;
	delete this->_personaje2;
	_personaje2 = NULL;
	delete _instance;
	_instance = NULL;
	TTF_Quit();
	Logger::Instance()->log(DEBUG,"Se destruye instancia de GameController");
}

GameController::GameController(Parser* parser)
{
	minimizado = false;
	enMainScreen = true;
	screen = MAINSCREEN_INTRO;
	modeSelected = SELECTED_PVP;
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
	TTF_Init();
	_ventana = GameController::getVentana(parser);
	_escenario = GameController::getEscenario(parser);
	_capas = GameController::getCapas(_ventana,parser,_escenario);
	_personaje1 = GameController::getPersonaje(_ventana,parser,_escenario,true);
	_personaje2 = GameController::getPersonaje(_ventana,parser,_escenario,false);
	_hud = GameController::getHud(_ventana, _personaje1, _personaje2);
	_end_of_game = false;
	iniciarEstructuraPerSelect();
	_mainScreen = new MainScreen(_ventana,&perSelect);
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

void GameController::iniciarEstructuraPerSelect() {
	for (int i=0; i<3; i++) {
		vector<int> vectorLocal(4);
		vectorLocal.at(0) = LIUKANG;
		vectorLocal.at(1) = SCORPION;
		vectorLocal.at(2) = LIUKANG;
		vectorLocal.at(3) = SCORPION;
		this->perSelect.push_back(vectorLocal);
	}
	filaP1 = 0;
	columnaP1 = 0;
	filaP2 = 0;
	columnaP2 = 3;
	textFocus = TEXT_NO_FOCUS;
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

	//cout << clock() << endl;
	this->_hud->printHUD(tiempoRemanente);

	//ActualizoPantalla
	this->_ventana->updateScreen();
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

bool GameController::actualizarGanador() {
	bool flag = false;
	if (tiempoRemanente <= 0) {
		if (_personaje1->healthPoints < _personaje2->healthPoints) {
			Logger::Instance()->log(WARNING,"Gano personaje 2.");
			flag = true;
		} else {
			if (_personaje1->healthPoints > _personaje2->healthPoints) {
				Logger::Instance()->log(WARNING,"Gano personaje 1.");
				flag = true;
			} else {
				Logger::Instance()->log(WARNING,"Empate!");
				flag = true;
			}
		}
	} else {
		if (this->_personaje1->healthPoints <= 5) {
			this->_personaje1->freeze();
			this->_personaje1->dizzy();
			if (this->_personaje1->healthPoints <= 0) {
				Logger::Instance()->log(WARNING,"Gano personaje 2.");
				this->_personaje2->freeze();
				this->_personaje2->winingPosition();
				//personaje1->viewDead();
				flag = true;
			}
		} else {
			if (this->_personaje2->healthPoints <= 5) {
				this->_personaje2->freeze();
				this->_personaje2->dizzy();
				if (this->_personaje2->healthPoints <= 0) {
					Logger::Instance()->log(WARNING,"Gano personaje 1.");
					this->_personaje1->freeze();
					this->_personaje1->winingPosition();
					//personaje2->viewDead();
					flag = true;
				}
			}
		}
	}
	return flag;	
}

void GameController::procesarBotones(SDL_Event* e) {
	Logger::Instance()->log(DEBUG,"Joystick # " + StringUtil::int2string(e->jdevice.which) + " pressed " + StringUtil::int2string(e->jbutton.button));
	if (e->jdevice.which == 0 && this->_personaje1->canMove()) {
			this->_personaje1->evaluarAccion(e->jbutton.button);
	}
	else if (e->jdevice.which == 1 && this->_personaje2->canMove()) {
			this->_personaje2->evaluarAccion(e->jbutton.button);
	}
}

void GameController::procesarEventosMainScreenIntro(SDL_Event* e) {
	switch (e->type) {
		case SDL_KEYDOWN:
			if (e->key.keysym.sym == SDLK_ESCAPE) this->_end_of_game = true;
			else if (e->key.keysym.sym == SDLK_m) this->screen = MAINSCREEN_MODE_SELECT;
			break;
		case SDL_MOUSEBUTTONDOWN:
			this->screen = MAINSCREEN_MODE_SELECT;
			break;
		case SDL_WINDOWEVENT:
			if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) minimizado = true;
			else if (e->window.event == SDL_WINDOWEVENT_RESTORED) minimizado = false;
			break;
	}
}

void GameController::procesarEventosMainScreenModeSelect(SDL_Event* e) {
	int mode;
	switch (e->type) {
		case SDL_KEYDOWN:
			if (e->key.keysym.sym == SDLK_ESCAPE) this->_end_of_game = true;
			else if (e->key.keysym.sym == SDLK_DOWN) this->modeSelected = (modeSelected + 1) % 3;
			else if (e->key.keysym.sym == SDLK_UP) this->modeSelected = (modeSelected + 2) % 3;
			else if (e->key.keysym.sym == SDLK_RETURN) {
				switch (modeSelected) {
				case SELECTED_PVP:
					this->screen = MAINSCREEN_PVP;
					break;
				case SELECTED_PVE:
					this->screen = MAINSCREEN_PVE;
					break;
				case SELECTED_TRAINING:
					this->screen = MAINSCREEN_TRAINING;
					break;
				}
			}
			break;

		case SDL_MOUSEMOTION:
			mode = this->_mainScreen->mouseOverMode();
			if (mode != NOT_SELECTED)	this->modeSelected = mode;
			break;

		case SDL_MOUSEBUTTONDOWN:
			mode = this->_mainScreen->mouseOverMode();
			switch (mode) {
			case SELECTED_PVP:
				this->screen = MAINSCREEN_PVP;
				break;
			case SELECTED_PVE:
				this->screen = MAINSCREEN_PVE;
				break;
			case SELECTED_TRAINING:
				this->screen = MAINSCREEN_TRAINING;
				break;
			}
			break;

		case SDL_WINDOWEVENT:
			if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) minimizado = true;
			else if (e->window.event == SDL_WINDOWEVENT_RESTORED) minimizado = false;
			break;
	}
}

void GameController::procesarEventosMainScreenPVP(SDL_Event* e) {
	switch (textFocus) {
	case TEXT_NO_FOCUS:
		switch (e->type) {
			case SDL_KEYDOWN:
				if (e->key.keysym.sym == SDLK_ESCAPE) this->_end_of_game = true;
				else if (e->key.keysym.sym == SDLK_b) this->screen = MAINSCREEN_MODE_SELECT;
				else if ((e->key.keysym.sym == SDLK_DOWN) && (filaP1 < 2)) filaP1++;
				else if ((e->key.keysym.sym == SDLK_UP) && (filaP1 > 0)) filaP1--;
				else if ((e->key.keysym.sym == SDLK_LEFT) && (columnaP1 > 0)) columnaP1--;
				else if ((e->key.keysym.sym == SDLK_RIGHT) && (columnaP1 < 3)) columnaP1++;
				else if ((e->key.keysym.sym == SDLK_PAGEDOWN) && (filaP2 < 2)) filaP2++;
				else if ((e->key.keysym.sym == SDLK_PAGEUP) && (filaP2 > 0)) filaP2--;
				else if ((e->key.keysym.sym == SDLK_INSERT) && (columnaP2 > 0)) columnaP2--;
				else if ((e->key.keysym.sym == SDLK_DELETE) && (columnaP2 < 3)) columnaP2++;
				else if (e->key.keysym.sym == SDLK_F1) {
					textFocus = TEXT_FOCUS_P1;
					SDL_StartTextInput();
				}
				else if (e->key.keysym.sym == SDLK_F2) {
					textFocus = TEXT_FOCUS_P2;
					SDL_StartTextInput();
				}
				else if (e->key.keysym.sym == SDLK_RETURN) {
					this->screen = NO_MAINSCREEN;
					this->enMainScreen = false;
					this->textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();

					//testing
					switch (perSelect.at(filaP1).at(columnaP1)) {
					case LIUKANG:	cout << "P1 selecciono 'liukang' y el nombre es '" << nombreP1 << "'" << endl; break;
					case SCORPION:	cout << "P1 selecciono 'scorpion' y el nombre es '" << nombreP1 << "'" << endl; break;
					}
					switch (perSelect.at(filaP2).at(columnaP2)) {
					case LIUKANG:	cout << "P2 selecciono 'liukang' y el nombre es '" << nombreP2 << "'" << endl; break;
					case SCORPION:	cout << "P2 selecciono 'scorpion' y el nombre es '" << nombreP2 << "'" << endl; break;
					}
				}
				break;
			case SDL_WINDOWEVENT:
				if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) minimizado = true;
				else if (e->window.event == SDL_WINDOWEVENT_RESTORED) minimizado = false;
		}
		break;

	case TEXT_FOCUS_P1:
		switch (e->type) {
			case SDL_TEXTINPUT:
				nombreP1 += e->text.text;
				break;
			case SDL_KEYDOWN:
				if (e->key.keysym.sym == SDLK_ESCAPE) this->_end_of_game = true;
				else if ((e->key.keysym.sym == SDLK_PAGEDOWN) && (filaP2 < 2)) filaP2++;
				else if ((e->key.keysym.sym == SDLK_PAGEUP) && (filaP2 > 0)) filaP2--;
				else if ((e->key.keysym.sym == SDLK_INSERT) && (columnaP2 > 0)) columnaP2--;
				else if ((e->key.keysym.sym == SDLK_DELETE) && (columnaP2 < 3)) columnaP2++;
				else if ((e->key.keysym.sym == SDLK_BACKSPACE) && (nombreP1.length() > 0)) nombreP1.pop_back();
				else if (e->key.keysym.sym == SDLK_F1) {
					textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
				}
				else if (e->key.keysym.sym == SDLK_RETURN) {
					this->screen = NO_MAINSCREEN;
					this->enMainScreen = false;
					this->textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();

					//testing
					switch (perSelect.at(filaP1).at(columnaP1)) {
					case LIUKANG:	cout << "P1 selecciono 'liukang' y el nombre es '" << nombreP1 << "'" << endl; break;
					case SCORPION:	cout << "P1 selecciono 'scorpion' y el nombre es '" << nombreP1 << "'" << endl; break;
					}
					switch (perSelect.at(filaP2).at(columnaP2)) {
					case LIUKANG:	cout << "P2 selecciono 'liukang' y el nombre es '" << nombreP2 << "'" << endl; break;
					case SCORPION:	cout << "P2 selecciono 'scorpion' y el nombre es '" << nombreP2 << "'" << endl; break;
					}
				}
				break;
			case SDL_WINDOWEVENT:
				if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) minimizado = true;
				else if (e->window.event == SDL_WINDOWEVENT_RESTORED) minimizado = false;
		}
		break;

	case TEXT_FOCUS_P2:
		switch (e->type) {
			case SDL_TEXTINPUT:
				nombreP2 += e->text.text;
				break;
			case SDL_KEYDOWN:
				if (e->key.keysym.sym == SDLK_ESCAPE) this->_end_of_game = true;
				else if ((e->key.keysym.sym == SDLK_DOWN) && (filaP1 < 2)) filaP1++;
				else if ((e->key.keysym.sym == SDLK_UP) && (filaP1 > 0)) filaP1--;
				else if ((e->key.keysym.sym == SDLK_LEFT) && (columnaP1 > 0)) columnaP1--;
				else if ((e->key.keysym.sym == SDLK_RIGHT) && (columnaP1 < 3)) columnaP1++;
				else if ((e->key.keysym.sym == SDLK_BACKSPACE) && (nombreP2.length() > 0)) nombreP2.pop_back();
				else if (e->key.keysym.sym == SDLK_F2) {
					textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
				}
				else if (e->key.keysym.sym == SDLK_RETURN) {
					this->screen = NO_MAINSCREEN;
					this->enMainScreen = false;
					this->textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();

					//testing
					switch (perSelect.at(filaP1).at(columnaP1)) {
					case LIUKANG:	cout << "P1 selecciono 'liukang' y el nombre es '" << nombreP1 << "'" << endl; break;
					case SCORPION:	cout << "P1 selecciono 'scorpion' y el nombre es '" << nombreP1 << "'" << endl; break;
					}
					switch (perSelect.at(filaP2).at(columnaP2)) {
					case LIUKANG:	cout << "P2 selecciono 'liukang' y el nombre es '" << nombreP2 << "'" << endl; break;
					case SCORPION:	cout << "P2 selecciono 'scorpion' y el nombre es '" << nombreP2 << "'" << endl; break;
					}
				}
				break;
			case SDL_WINDOWEVENT:
				if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) minimizado = true;
				else if (e->window.event == SDL_WINDOWEVENT_RESTORED) minimizado = false;
		}
		break;
	}
}

void GameController::procesarEventosMainScreenPVE(SDL_Event* e) {
	procesarEventosMainScreenTraining(e);
}

void GameController::procesarEventosMainScreenTraining(SDL_Event* e) {
	switch (textFocus) {
	case TEXT_NO_FOCUS:
		switch (e->type) {
			case SDL_KEYDOWN:
				if (e->key.keysym.sym == SDLK_ESCAPE) this->_end_of_game = true;
				else if (e->key.keysym.sym == SDLK_b) this->screen = MAINSCREEN_MODE_SELECT;
				else if ((e->key.keysym.sym == SDLK_DOWN) && (filaP1 < 2)) filaP1++;
				else if ((e->key.keysym.sym == SDLK_UP) && (filaP1 > 0)) filaP1--;
				else if ((e->key.keysym.sym == SDLK_LEFT) && (columnaP1 > 0)) columnaP1--;
				else if ((e->key.keysym.sym == SDLK_RIGHT) && (columnaP1 < 3)) columnaP1++;
				else if (e->key.keysym.sym == SDLK_TAB) {
					textFocus = TEXT_FOCUS_P1;
					SDL_StartTextInput();
				}
				else if (e->key.keysym.sym == SDLK_RETURN) {
					this->screen = NO_MAINSCREEN;
					this->enMainScreen = false;
					this->textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();

					//testing
					switch (perSelect.at(filaP1).at(columnaP1)) {
					case LIUKANG:	cout << "se selecciono 'liukang' y el nombre es '" << nombreP1 << "'" << endl; break;
					case SCORPION:	cout << "se selecciono 'scorpion' y el nombre es '" << nombreP1 << "'" << endl; break;
					}
				}
				break;
			case SDL_WINDOWEVENT:
				if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) minimizado = true;
				else if (e->window.event == SDL_WINDOWEVENT_RESTORED) minimizado = false;
		}
		break;
	case TEXT_FOCUS_P1:
		switch (e->type) {
			case SDL_TEXTINPUT:
				nombreP1 += e->text.text;
				break;
			case SDL_KEYDOWN:
				if (e->key.keysym.sym == SDLK_ESCAPE) this->_end_of_game = true;
				else if ((e->key.keysym.sym == SDLK_BACKSPACE) && (nombreP1.length() > 0)) nombreP1.pop_back();
				else if (e->key.keysym.sym == SDLK_TAB) {
					textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
				}
				else if (e->key.keysym.sym == SDLK_RETURN) {
					this->screen = NO_MAINSCREEN;
					this->enMainScreen = false;
					this->textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();

					//testing
					switch (perSelect.at(filaP1).at(columnaP1)) {
					case LIUKANG:	cout << "se selecciono 'liukang' y el nombre es '" << nombreP1 << "'" << endl; break;
					case SCORPION:	cout << "se selecciono 'scorpion' y el nombre es '" << nombreP1 << "'" << endl; break;
					}
				}
				break;
			case SDL_WINDOWEVENT:
				if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) minimizado = true;
				else if (e->window.event == SDL_WINDOWEVENT_RESTORED) minimizado = false;
		}
		break;
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
			//DEBUGEAR PERSONAJE CON TECLADO
			else if (e->key.keysym.sym == SDLK_u) this->_personaje1->patadaBaja();
			else if (e->key.keysym.sym == SDLK_j) this->_personaje1->patadaAlta();
			else if (e->key.keysym.sym == SDLK_i) this->_personaje1->golpeBajo();
			else if (e->key.keysym.sym == SDLK_k) this->_personaje1->golpeAlto();
			else if (e->key.keysym.sym == SDLK_t) this->_personaje1->barrer();
			break;
		case SDL_WINDOWEVENT:
			if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) {
				minimizado = true;
				pauseTime = clock();
			} else if (e->window.event == SDL_WINDOWEVENT_RESTORED) {
				minimizado = false;
				pauseAccumulator = pauseAccumulator + (clock() - pauseTime);
			}
			break;
		}
}

void GameController::procesarMovimientoJoystick() {
	if (this->hayPlayer1()) {
		if (this->_personaje1->canMove()) {
		const Sint16 AXSP1 = SDL_JoystickGetAxis(this->_joystickOne,0);
		const Sint16 AYSP1 = SDL_JoystickGetAxis(this->_joystickOne,1);
		const Uint8 BLBTP1 = SDL_JoystickGetButton(this->_joystickOne,this->_personaje1->getData()->defensa);

		if (BLBTP1 == 1) {
			if (this->_personaje1->isDucking()) {
				this->_personaje1->blockDuck();
			} else {
				this->_personaje1->block();
			}
		}
		else if (AXSP1 < 0 && AYSP1 < 0) {
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
	}
	
	if (this->hayPlayer2()) {
		if (this->_personaje2->canMove()) {
		const Sint16 AXSP2 = SDL_JoystickGetAxis(this->_joystickTwo,0);
		const Sint16 AYSP2 = SDL_JoystickGetAxis(this->_joystickTwo,1);
		const Uint8 BLBTP2 = SDL_JoystickGetButton(this->_joystickTwo,this->_personaje2->getData()->defensa);

		if (BLBTP2 == 1) {
			if (this->_personaje2->isDucking()) {
				this->_personaje2->blockDuck();
			} else {
				this->_personaje2->block();
			}
		}
		else if (AXSP2 < 0 && AYSP2 < 0) {
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
}

bool GameController::canMoveLeft(Personaje* pers, Personaje* otherPers)
{
	if(otherPers->isRightMargin() && pers->isLeftMargin())
		return false;
	if(otherPers->hayColision(otherPers->boundingBox, pers->boundingBox))
	{
		if(pers->_orientacion)
		{	
			if(!otherPers->isBlocking() && !otherPers->isMaxPushLeft())
			{
				otherPers->moveLeft(MOV_FACTOR2);
				if(otherPers->isLeftMargin())
				{
					this->moveLayersRight(MOV_FACTOR2);
				}
			}
			else
				return false;
		}
	}
	return true;
}

bool GameController::canMoveRight(Personaje* pers, Personaje* otherPers)
{
	if(otherPers->isLeftMargin() && pers->isRightMargin())
	{
		//cout << "No me puedo mover mas" << endl;
		return false;
	}
	if(otherPers->hayColision(otherPers->boundingBox, pers->boundingBox))
	{
		if(!pers->_orientacion)
		{	
			if(!otherPers->isBlocking() && !otherPers->isMaxPushRight())
			{
				otherPers->moveRight(MOV_FACTOR2);
				if(otherPers->isRightMargin())
				{
					this->moveLayersLeft(MOV_FACTOR2);
				}
			}
			else
			{
				//cout << "No me puedo mover mas2" << endl;
				return false;
			}
		}
	}
	return true;
}

void GameController::procesamientoMainScreenIntro() {
	SDL_Event e;
	while( SDL_PollEvent(&e) != 0 )
	{
		if( e.type == SDL_QUIT ) this->setEndOfGame(true);
		this->procesarEventosMainScreenIntro(&e);
	}

	if (!this->minimizado)
		this->_mainScreen->showIntro();
	else
		SDL_Delay(DEF_SLEEP_TIME);
}

void GameController::procesamientoMainScreenModeSelect() {
	SDL_Event e;
	while( SDL_PollEvent(&e) != 0 )
	{
		if( e.type == SDL_QUIT ) this->setEndOfGame(true);
		this->procesarEventosMainScreenModeSelect(&e);
	}

	if (!this->minimizado)
		this->_mainScreen->showModeSelect(modeSelected);
	else
		SDL_Delay(DEF_SLEEP_TIME);
}

void GameController::procesamientoMainScreenPVP() {
	SDL_Event e;
	while( SDL_PollEvent(&e) != 0 )
	{
		if( e.type == SDL_QUIT ) this->setEndOfGame(true);
		this->procesarEventosMainScreenPVP(&e);
	}

	startTime = clock();

	if (!this->minimizado)
		this->_mainScreen->showPVP(filaP1,columnaP1,filaP2,columnaP2,textFocus,nombreP1,nombreP2);
	else
		SDL_Delay(DEF_SLEEP_TIME);
}

void GameController::procesamientoMainScreenPVE() {
	SDL_Event e;
	while( SDL_PollEvent(&e) != 0 )
	{
		if( e.type == SDL_QUIT ) this->setEndOfGame(true);
		this->procesarEventosMainScreenPVE(&e);
	}

	startTime = clock();

	if (!this->minimizado)
		this->_mainScreen->showPVE(filaP1,columnaP1,textFocus,nombreP1);
	else
		SDL_Delay(DEF_SLEEP_TIME);
}

void GameController::procesamientoMainScreenTraining() {
	SDL_Event e;
	while( SDL_PollEvent(&e) != 0 )
	{
		if( e.type == SDL_QUIT ) this->setEndOfGame(true);
		this->procesarEventosMainScreenTraining(&e);
	}

	startTime = clock();

	if (!this->minimizado)
		this->_mainScreen->showTraining(filaP1,columnaP1,textFocus,nombreP1);
	else
		SDL_Delay(DEF_SLEEP_TIME);
}

void GameController::run()
{
	SDL_Event e;
	Logger::Instance()->log(DEBUG,"Comienzo ciclo de Juego");
	pauseAccumulator = 0;
	while (!this->_end_of_game)
	{
		if (enMainScreen) {
			switch (screen) {
			case MAINSCREEN_INTRO:
				procesamientoMainScreenIntro();
				break;
			case MAINSCREEN_MODE_SELECT:
				procesamientoMainScreenModeSelect();
				break;
			case MAINSCREEN_PVP:
				procesamientoMainScreenPVP();
				break;
			case MAINSCREEN_PVE:
				procesamientoMainScreenPVE();
				break;
			case MAINSCREEN_TRAINING:
				procesamientoMainScreenTraining();
				break;
			}

		} else {
			while( SDL_PollEvent(&e) != 0 )
			{
				if( e.type == SDL_QUIT ) this->setEndOfGame(true);
				this->procesarEventos(&e);
			}
			if (!this->minimizado) {
				this->procesarMovimientoJoystick();
 				this->getKeys();
				_personaje1->continueAction(MOV_FACTOR_JMP,JMP_FACTOR,_personaje2);
				_personaje2->continueAction(MOV_FACTOR_JMP,JMP_FACTOR,_personaje1);
				this->moveLayers(_personaje1,_personaje2);
				this->moveLayers(_personaje2,_personaje1);
				//tiempoRemanente = (int)ceil(FIGHT_TIME_COUNTDOWN - ((float)clock() - startTime - pauseAccumulator)/1000);
				if (this->actualizarGanador()) {
					this->reloadConfig();		
				}
				this->printLayers();
			} else { SDL_Delay(DEF_SLEEP_TIME); }
		}
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
	startTime = clock();
	pauseAccumulator = 0;
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

	else if(currentKeyStates[ SDL_SCANCODE_W ])
	{
		this->_personaje2->jump(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_S] && currentKeyStates[ SDL_SCANCODE_E ])
	{
		this->_personaje2->blockDuck();
	}
	else if(currentKeyStates[ SDL_SCANCODE_O ])
	{
		this->_personaje2->lanzarArma();
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
	else if(currentKeyStates[ SDL_SCANCODE_UP ])
	{
		this->_personaje1->jump(JMP_FACTOR);
	}
	else if(currentKeyStates[ SDL_SCANCODE_L ])
	{
		this->_personaje1->lanzarArma();
	}
	else if(currentKeyStates[ SDL_SCANCODE_DOWN ] && currentKeyStates[ SDL_SCANCODE_B ])
	{
		this->_personaje1->blockDuck();
	}
	else if(currentKeyStates[ SDL_SCANCODE_DOWN ])
	{
		this->_personaje1->duck();
	}
	else if(currentKeyStates[ SDL_SCANCODE_B ] )
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
	if (this->_personaje1->canMove()) this->getKeysPlayer1();
	if (this->_personaje2->canMove()) this->getKeysPlayer2();
}

void GameController::moveLayers(Personaje* pers, Personaje* otherPers)
{
	//Veo si debo mover las capas
	if( pers->isRightMargin() && pers->isWalking() && !otherPers->isLeftMargin() )
	{
		//cout << "Muevo camara a la derecha" << endl;
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
