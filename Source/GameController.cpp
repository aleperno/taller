/*
 * GameController.cpp
 *
 *  Created on: Apr 3, 2015
 *      Author: neo
 */
#include <GameController.h>
#define MOV_FACTOR 300;		//Fraccion de la capa que se mueve por ciclo
#define MOV_FACTOR2 0.8
#define MOV_FACTOR_JMP 2
#define MOVE_P_FACTOR 1
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

	delete this->_ventana;
	_ventana = NULL;

	for(std::size_t i=0; i<this->_capas.size(); ++i) {
		delete this->_capas[i];
	}
	_capas.clear();

	delete this->_fightTimer;
	_fightTimer = NULL;
	delete this->_bufferTimer;
	_bufferTimer = NULL;
	delete this->_mainScreen;
	_mainScreen = NULL;
	delete this->_hud;
	_hud = NULL;
	if (comboAUX) delete this->comboAUX;
	comboAUX = NULL;
	
	delete this->_jugador1liukang;
	_jugador1liukang = NULL;
	delete this->_jugador1scorpion;
	_jugador1scorpion = NULL;
	delete this->_jugador2liukang;
	_jugador2liukang = NULL;
	delete this->_jugador2liukangColor;
	_jugador2liukangColor = NULL;
	delete this->_jugador2scorpion;
	_jugador2scorpion = NULL;
	delete this->_jugador2scorpionColor;
	_jugador2scorpionColor = NULL;
	_personaje1 = NULL;
	_personaje2 = NULL;
	
	delete _instance;
	_instance = NULL;
	TTF_Quit();
	Logger::Instance()->log(DEBUG,"Se destruye instancia de GameController");
}

GameController::GameController(Parser* parser)
{
	_end_of_game = false;
	minimizado = false;
	enMainScreen = true;
	partidaPreparada = false;
	screen = MAINSCREEN_INTRO;
	modeSelected = SELECTED_PVP;
	botonSeleccionadoEnModo = PLAY_BOTON;
	lastJoyValue1X = JOYSTICK_DEAD_ZONE;
	lastJoyValue1Y = JOYSTICK_DEAD_ZONE;
	lastJoyValue2X = JOYSTICK_DEAD_ZONE;
	lastJoyValue2Y = JOYSTICK_DEAD_ZONE;
	
	this->_joystickOne = NULL;
	this->_joystickTwo = NULL;
	this->_hayPlayer1 = false;
	this->_hayPlayer2 = false;
	if(!iniciarSDL())
		Logger::Instance()->log(ERROR,"SDL could not initialize!");
	Logger::Instance()->log(ERROR,"Joysticks detectados: " + StringUtil::int2string(this->_numJoysticks));

	TTF_Init();

	_ventana = GameController::getVentana(parser);
	_escenario = GameController::getEscenario(parser);
	_capas = GameController::getCapas(_ventana,parser,_escenario);
	getPersonajes(_ventana,parser,_escenario);
	InicializarAI(DIFFICULTY);
	this->musica = new SoundHandler();
	_personaje1 = this->_jugador1scorpion;
	_personaje2 = this->_jugador2liukangColor;
	_hud = new Hud(_ventana, &nombreP1, &nombreP2);

	iniciarEstructuraPerSelect();
	vector<Personaje*> punteros(6);
	punteros.at(0) = _jugador1liukang;
	punteros.at(1) = _jugador1scorpion;
	punteros.at(2) = _jugador2liukang;
	punteros.at(3) = _jugador2liukangColor;
	punteros.at(4) = _jugador2scorpion;
	punteros.at(5) = _jugador2scorpionColor;
	_mainScreen = new MainScreen(_ventana,&perSelect,&punteros);
	_fightTimer = new Temporizador();
	_bufferTimer = new Temporizador();
	comboAUX = new vector<string>();
	Logger::Instance()->log(DEBUG,"Se crea instancia de GameController");
}

void GameController::InicializarAI(int _difficulty)
{
	ai_handler = new AI(_difficulty);
}

bool GameController::iniciarSDL() {
	bool flag =  (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) >= 0 );
	if(flag) {
		if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
           		Logger::Instance()->log(ERROR,"No se ha iniciado el mixer de sonido");
            		flag = false;
        	}
	}
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

void GameController::getPersonajes(Ventana* ventana,Parser* parser, EscenarioData escenario)
{
	PersonajeData jugador1liukang;
	jugador1liukang.llenarDatos(&(parser->jugador1),&(parser->liukang));
	_jugador1liukang = new Personaje(ventana, jugador1liukang, escenario, true, false);

	PersonajeData jugador1scorpion;
	jugador1scorpion.llenarDatos(&(parser->jugador1),&(parser->scorpion));
	_jugador1scorpion = new Personaje(ventana, jugador1scorpion, escenario, true, false);

	PersonajeData jugador2liukang;
	jugador2liukang.llenarDatos(&(parser->jugador2),&(parser->liukang));
	_jugador2liukang = new Personaje(ventana, jugador2liukang, escenario, false, false);

	PersonajeData jugador2scorpion;
	jugador2scorpion.llenarDatos(&(parser->jugador2),&(parser->scorpion));
	_jugador2scorpion = new Personaje(ventana, jugador2scorpion, escenario, false, false);

	_jugador2liukangColor = new Personaje(ventana, jugador2liukang, escenario, false, true);

	_jugador2scorpionColor = new Personaje(ventana, jugador2scorpion, escenario, false, true);
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
			Logger::Instance()->log(WARNING,"Round ganado por personaje 2.");
			personaje2Wins++;
			flag = true;
		} else {
			if (_personaje1->healthPoints > _personaje2->healthPoints) {
				Logger::Instance()->log(WARNING,"Round ganado por personaje 1.");
				personaje1Wins++;
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
				Logger::Instance()->log(WARNING,"Round ganado por personaje 2.");
				this->_personaje2->freeze();
				this->_personaje2->winingPosition();
				personaje2Wins++;
				//personaje1->viewDead();
				flag = true;
			}
		} else {
			if (this->_personaje2->healthPoints <= 5) {
				this->_personaje2->freeze();
				this->_personaje2->dizzy();
				if (this->_personaje2->healthPoints <= 0) {
					Logger::Instance()->log(WARNING,"Round ganado por personaje 1.");
					this->_personaje1->freeze();
					this->_personaje1->winingPosition();
					personaje1Wins++;
					//personaje2->viewDead();
					flag = true;
				}
			}
		}
	}
	return flag;	
}

bool GameController::actualizarGanadorTraining() {
	bool flag = false;
	if (this->_personaje1->healthPoints <= 5) {
		this->_personaje1->freeze();
		this->_personaje1->dizzy();
		if (this->_personaje1->healthPoints <= 0) {
			Logger::Instance()->log(WARNING,"Murio personaje 2.");
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
				Logger::Instance()->log(WARNING,"Murio personaje 1.");
				this->_personaje1->freeze();
				this->_personaje1->winingPosition();
				//personaje2->viewDead();
				flag = true;
			}
		}
	}
	return flag;	
}

void GameController::procesarBotones(SDL_Event* e) {
	Logger::Instance()->log(DEBUG,"Joystick # " + StringUtil::int2string(e->jdevice.which) + " pressed " + StringUtil::int2string(e->jbutton.button));

	if (e->jbutton.button == 9) this->toMainScreen();
	else if (e->jbutton.button == 8 && this->estoyEnTraining()) this->resetearVentanaPersonajes();
	else if (e->jdevice.which == 0 && this->_personaje1->canMove()) {
		this->_personaje1->evaluarAccion(e->jbutton.button,this->estoyEnPVE(),this->estoyEnTraining());
	} else if (e->jdevice.which == 1 && this->_personaje2->canMove()) {
		this->_personaje2->evaluarAccion(e->jbutton.button);
	}
}

void GameController::procesarEventosMainScreenIntro(SDL_Event* e) {
	switch (e->type) {
		case SDL_KEYDOWN:
			if (e->key.keysym.sym == SDLK_ESCAPE) this->_end_of_game = true;
			break;
		case SDL_JOYBUTTONDOWN:
			if ((e->jdevice.which == 0) && (e->jbutton.button == 9)) {
				this->screen = MAINSCREEN_MODE_SELECT;
				this->_mainScreen->posicionarGate();
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			this->screen = MAINSCREEN_MODE_SELECT;
			this->_mainScreen->posicionarGate();
			break;
		case SDL_WINDOWEVENT:
			if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) minimizado = true;
			else if (e->window.event == SDL_WINDOWEVENT_RESTORED) minimizado = false;
			break;
	}
}

void GameController::actualizarPersonajes() {
	switch (perSelect.at(filaP1).at(columnaP1)) {
	case LIUKANG:
		_personaje1 = _jugador1liukang;
		break;
	case SCORPION:
		_personaje1 = _jugador1scorpion;
		break;
	}
	
	if (modeSelected == SELECTED_PVP) {
		switch (perSelect.at(filaP2).at(columnaP2)) {
		case LIUKANG:
			if (perSelect.at(filaP1).at(columnaP1) == LIUKANG)
				_personaje2 = _jugador2liukangColor;
			else
				_personaje2 = _jugador2liukang;
			break;
		case SCORPION:
			if (perSelect.at(filaP1).at(columnaP1) == SCORPION)
				_personaje2 = _jugador2scorpionColor;
			else
				_personaje2 = _jugador2scorpion;
			break;
		}
	} else {
		int random = rand() % 2;
		if (random == 0) {
			if (_personaje1 == _jugador1liukang)	_personaje2 = _jugador2liukangColor;
			else	_personaje2 = _jugador2liukang;
		} else if (random == 1) {
			if (_personaje1 == _jugador1scorpion)	_personaje2 = _jugador2scorpionColor;
			else	_personaje2 = _jugador2scorpion;
		}
	}
}

void GameController::procesarEventosMainScreenModeSelect(SDL_Event* e) {
	int mode;
	switch (e->type) {
		case SDL_KEYDOWN:
			if (e->key.keysym.sym == SDLK_ESCAPE) this->_end_of_game = true;
			break;

		case SDL_JOYBUTTONDOWN:
			if ((e->jdevice.which == 0) && (e->jbutton.button == OK))
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
			break;

		case SDL_JOYAXISMOTION:
			if ((e->jdevice.which == 0) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue1Y < JOYSTICK_DEAD_ZONE) && (e->jaxis.axis == 1)) {
				this->modeSelected = (modeSelected + 1) % 3;
				lastJoyValue1Y = e->jaxis.value;
			} else if ((e->jdevice.which == 0) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue1Y > -JOYSTICK_DEAD_ZONE) && (e->jaxis.axis == 1)) {
				this->modeSelected = (modeSelected + 2) % 3;
				lastJoyValue1Y = e->jaxis.value;
			} else if (e->jdevice.which == 0)
				lastJoyValue1Y = e->jaxis.value;
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
	this->tipo_juego = PVP;
	int backORplay;
	switch (e->type) {

	case SDL_WINDOWEVENT:
		if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) minimizado = true;
		else if (e->window.event == SDL_WINDOWEVENT_RESTORED) minimizado = false;
	break;

	case SDL_MOUSEMOTION:
		backORplay = this->_mainScreen->mouseOverBackOrPlay();
		if (backORplay != NINGUNO)	this->botonSeleccionadoEnModo = backORplay;
	break;

	case SDL_MOUSEBUTTONDOWN:
		backORplay = this->_mainScreen->mouseOverBackOrPlay();
		switch (backORplay) {

		case BACK_BOTON:
			this->screen = MAINSCREEN_MODE_SELECT;
			this->textFocus = TEXT_NO_FOCUS;
			SDL_StopTextInput();
		break;

		case PLAY_BOTON:
			this->screen = NO_MAINSCREEN;
			this->enMainScreen = false;
			this->textFocus = TEXT_NO_FOCUS;
			SDL_StopTextInput();
		break;

		case NINGUNO:
			if (e->button.button == SDL_BUTTON_LEFT)
				if (this->_mainScreen->clickOnTextCamp() == TEXT_FOCUS_P1) {
					textFocus = TEXT_FOCUS_P1;
					SDL_StartTextInput();
				} else {
					textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
					if ((this->_mainScreen->faceSelected().first >= 0) && (this->_mainScreen->faceSelected().second >= 0)) {
						this->filaP1 = this->_mainScreen->faceSelected().first;
						this->columnaP1 = this->_mainScreen->faceSelected().second;
					}
				}
			else if (e->button.button == SDL_BUTTON_RIGHT) {
				if (this->_mainScreen->clickOnTextCamp() == TEXT_FOCUS_P2) {
					textFocus = TEXT_FOCUS_P2;
					SDL_StartTextInput();
				} else {
					textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
					if ((this->_mainScreen->faceSelected().first >= 0) && (this->_mainScreen->faceSelected().second >= 0)) {
						this->filaP2 = this->_mainScreen->faceSelected().first;
						this->columnaP2 = this->_mainScreen->faceSelected().second;
					}
				}
			}
		break;
		}
	break;

	case SDL_TEXTINPUT:
		if ((textFocus == TEXT_FOCUS_P1) && (nombreP1.length() < MAX_NAME_LENGTH))	nombreP1 += e->text.text;
		else if ((textFocus == TEXT_FOCUS_P2) && (nombreP2.length() < MAX_NAME_LENGTH))	nombreP2 += e->text.text;
	break;

	case SDL_KEYDOWN:
		if (e->key.keysym.sym == SDLK_ESCAPE)	this->_end_of_game = true;
		else if ((e->key.keysym.sym == SDLK_BACKSPACE) && (textFocus == TEXT_FOCUS_P1) && (nombreP1.length() > 0))
			nombreP1.pop_back();
		else if ((e->key.keysym.sym == SDLK_BACKSPACE) && (textFocus == TEXT_FOCUS_P2) && (nombreP2.length() > 0))
			nombreP2.pop_back();
	break;

	case SDL_JOYBUTTONDOWN:
		switch (textFocus) {

			case TEXT_NO_FOCUS:
				if ((e->jdevice.which == 0) && (e->jbutton.button == OK)) {
					this->screen = NO_MAINSCREEN;
					this->enMainScreen = false;
				} else if ((e->jdevice.which == 0) && (e->jbutton.button == BACK)) {
					this->screen = MAINSCREEN_MODE_SELECT;
				} else if ((e->jdevice.which == 0) && (e->jbutton.button == NAME)) {
					textFocus = TEXT_FOCUS_P1;
					SDL_StartTextInput();
				} else if ((e->jdevice.which == 1) && (e->jbutton.button == NAME)) {
					textFocus = TEXT_FOCUS_P2;
					SDL_StartTextInput();
				}
			break;

			case TEXT_FOCUS_P1:
				if ((e->jdevice.which == 0) && (e->jbutton.button == OK)) {
					this->screen = NO_MAINSCREEN;
					this->enMainScreen = false;
					this->textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
				} else if ((e->jdevice.which == 0) && (e->jbutton.button == NAME)) {
					textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
				} else if ((e->jdevice.which == 0) && (e->jbutton.button == ERASE) && (nombreP1.length() > 0)) {
					nombreP1.pop_back();
				}
			break;

			case TEXT_FOCUS_P2:
				if ((e->jdevice.which == 0) && (e->jbutton.button == OK)) {
					this->screen = NO_MAINSCREEN;
					this->enMainScreen = false;
					this->textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
				} else if ((e->jdevice.which == 1) && (e->jbutton.button == NAME)) {
					textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
				} else if ((e->jdevice.which == 1) && (e->jbutton.button == ERASE) && (nombreP2.length() > 0)) {
					nombreP2.pop_back();
				}
			break;
		}
	break;

	case SDL_JOYAXISMOTION:
		switch (textFocus) {

			case TEXT_NO_FOCUS:
				if ((e->jaxis.which == 0) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue1Y < JOYSTICK_DEAD_ZONE) && (filaP1 < 2) && (e->jaxis.axis == 1)) {
					filaP1++;
					lastJoyValue1Y = e->jaxis.value;
				} else if ((e->jaxis.which == 0) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue1Y > -JOYSTICK_DEAD_ZONE) && (filaP1 > 0) && (e->jaxis.axis == 1)) {
					filaP1--;
					lastJoyValue1Y = e->jaxis.value;
				} else if ((e->jaxis.which == 0) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue1X < JOYSTICK_DEAD_ZONE) && (columnaP1 < 3) && (e->jaxis.axis == 0)) {
					columnaP1++;
					lastJoyValue1X = e->jaxis.value;
				} else if ((e->jaxis.which == 0) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue1X > -JOYSTICK_DEAD_ZONE) && (columnaP1 > 0) && (e->jaxis.axis == 0)) {
					columnaP1--;
					lastJoyValue1X = e->jaxis.value;
				} else if ((e->jaxis.which == 0) && (e->jaxis.axis == 1)) {
					lastJoyValue1Y = e->jaxis.value;
				} else if ((e->jaxis.which == 0) && (e->jaxis.axis == 0)) {
					lastJoyValue1X = e->jaxis.value;
				} else if ((e->jaxis.which == 1) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue2Y < JOYSTICK_DEAD_ZONE) && (filaP2 < 2) && (e->jaxis.axis == 1)) {
					filaP2++;
					lastJoyValue2Y = e->jaxis.value;
				} else if ((e->jaxis.which == 1) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue2Y > -JOYSTICK_DEAD_ZONE) && (filaP2 > 0) && (e->jaxis.axis == 1)) {
					filaP2--;
					lastJoyValue2Y = e->jaxis.value;
				} else if ((e->jaxis.which == 1) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue2X < JOYSTICK_DEAD_ZONE) && (columnaP2 < 3) && (e->jaxis.axis == 0)) {
					columnaP2++;
					lastJoyValue2X = e->jaxis.value;
				} else if ((e->jaxis.which == 1) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue2X > -JOYSTICK_DEAD_ZONE) && (columnaP2 > 0) && (e->jaxis.axis == 0)) {
					columnaP2--;
					lastJoyValue2X = e->jaxis.value;
				} else if ((e->jaxis.which == 1) && (e->jaxis.axis == 1)) {
					lastJoyValue2Y = e->jaxis.value;
				} else if ((e->jaxis.which == 1) && (e->jaxis.axis == 0)) {
					lastJoyValue2X = e->jaxis.value;
				}
			break;

			case TEXT_FOCUS_P1:
				if ((e->jaxis.which == 1) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue2Y < JOYSTICK_DEAD_ZONE) && (filaP2 < 2) && (e->jaxis.axis == 1)) {
					filaP2++;
					lastJoyValue2Y = e->jaxis.value;
				} else if ((e->jaxis.which == 1) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue2Y > -JOYSTICK_DEAD_ZONE) && (filaP2 > 0) && (e->jaxis.axis == 1)) {
					filaP2--;
					lastJoyValue2Y = e->jaxis.value;
				} else if ((e->jaxis.which == 1) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue2X < JOYSTICK_DEAD_ZONE) && (columnaP2 < 3) && (e->jaxis.axis == 0)) {
					columnaP2++;
					lastJoyValue2X = e->jaxis.value;
				} else if ((e->jaxis.which == 1) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue2X > -JOYSTICK_DEAD_ZONE) && (columnaP2 > 0) && (e->jaxis.axis == 0)) {
					columnaP2--;
					lastJoyValue2X = e->jaxis.value;
				} else if ((e->jaxis.which == 1) && (e->jaxis.axis == 1)) {
					lastJoyValue2Y = e->jaxis.value;
				} else if ((e->jaxis.which == 1) && (e->jaxis.axis == 0)) {
					lastJoyValue2X = e->jaxis.value;
				}
			break;

			case TEXT_FOCUS_P2:
				if ((e->jaxis.which == 0) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue1Y < JOYSTICK_DEAD_ZONE) && (filaP1 < 2) && (e->jaxis.axis == 1)) {
					filaP1++;
					lastJoyValue1Y = e->jaxis.value;
				} else if ((e->jaxis.which == 0) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue1Y > -JOYSTICK_DEAD_ZONE) && (filaP1 > 0) && (e->jaxis.axis == 1)) {
					filaP1--;
					lastJoyValue1Y = e->jaxis.value;
				} else if ((e->jaxis.which == 0) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue1X < JOYSTICK_DEAD_ZONE) && (columnaP1 < 3) && (e->jaxis.axis == 0)) {
					columnaP1++;
					lastJoyValue1X = e->jaxis.value;
				} else if ((e->jaxis.which == 0) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue1X > -JOYSTICK_DEAD_ZONE) && (columnaP1 > 0) && (e->jaxis.axis == 0)) {
					columnaP1--;
					lastJoyValue1X = e->jaxis.value;
				} else if ((e->jaxis.which == 0) && (e->jaxis.axis == 1)) {
					lastJoyValue1Y = e->jaxis.value;
				} else if ((e->jaxis.which == 0) && (e->jaxis.axis == 0)) {
					lastJoyValue1X = e->jaxis.value;
				}
			break;
		}
	break;
	}
}

void GameController::procesarEventosMainScreenPVE(SDL_Event* e) {
	this->tipo_juego = PVE;
	int backORplay;
	switch (e->type) {

	case SDL_WINDOWEVENT:
		if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) minimizado = true;
		else if (e->window.event == SDL_WINDOWEVENT_RESTORED) minimizado = false;
	break;
		
	case SDL_MOUSEMOTION:
		backORplay = this->_mainScreen->mouseOverBackOrPlay();
		if (backORplay != NINGUNO)	this->botonSeleccionadoEnModo = backORplay;
	break;

	case SDL_MOUSEBUTTONDOWN:
		backORplay = this->_mainScreen->mouseOverBackOrPlay();
		switch (backORplay) {

		case BACK_BOTON:
			this->screen = MAINSCREEN_MODE_SELECT;
			this->textFocus = TEXT_NO_FOCUS;
			SDL_StopTextInput();
		break;

		case PLAY_BOTON:
			this->screen = NO_MAINSCREEN;
			this->enMainScreen = false;
			this->textFocus = TEXT_NO_FOCUS;
			SDL_StopTextInput();
		break;

		case NINGUNO:
			if (this->_mainScreen->clickOnTextCamp() == TEXT_FOCUS_P1) {
				textFocus = TEXT_FOCUS_P1;
				SDL_StartTextInput();
			} else {
				textFocus = TEXT_NO_FOCUS;
				SDL_StopTextInput();
				if ((this->_mainScreen->faceSelected().first >= 0) && (this->_mainScreen->faceSelected().second >= 0)) {
					this->filaP1 = this->_mainScreen->faceSelected().first;
					this->columnaP1 = this->_mainScreen->faceSelected().second;
				}
			}
		break;
		}
	break;

	case SDL_TEXTINPUT:
		if ((textFocus == TEXT_FOCUS_P1) && (nombreP1.length() < MAX_NAME_LENGTH))	nombreP1 += e->text.text;
	break;

	case SDL_KEYDOWN:
		if (e->key.keysym.sym == SDLK_ESCAPE)	this->_end_of_game = true;
		else if ((e->key.keysym.sym == SDLK_BACKSPACE) && (textFocus == TEXT_FOCUS_P1) && (nombreP1.length() > 0))
			nombreP1.pop_back();
	break;

	case SDL_JOYBUTTONDOWN:
		switch (textFocus) {

			case TEXT_NO_FOCUS:
				if ((e->jdevice.which == 0) && (e->jbutton.button == OK)) {
					this->screen = NO_MAINSCREEN;
					this->enMainScreen = false;
				} else if ((e->jdevice.which == 0) && (e->jbutton.button == BACK)) {
					this->screen = MAINSCREEN_MODE_SELECT;
				} else if ((e->jdevice.which == 0) && (e->jbutton.button == NAME)) {
					textFocus = TEXT_FOCUS_P1;
					SDL_StartTextInput();
				}
			break;

			case TEXT_FOCUS_P1:
				if ((e->jdevice.which == 0) && (e->jbutton.button == OK)) {
					this->screen = NO_MAINSCREEN;
					this->enMainScreen = false;
					this->textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
				} else if ((e->jdevice.which == 0) && (e->jbutton.button == NAME)) {
					textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
				} else if ((e->jdevice.which == 0) && (e->jbutton.button == ERASE) && (nombreP1.length() > 0)) {
					nombreP1.pop_back();
				}
			break;
		}
	break;

	case SDL_JOYAXISMOTION:
		if (textFocus == TEXT_NO_FOCUS) {
			if ((e->jaxis.which == 0) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue1Y < JOYSTICK_DEAD_ZONE) && (filaP1 < 2) && (e->jaxis.axis == 1)) {
				filaP1++;
				lastJoyValue1Y = e->jaxis.value;
			} else if ((e->jaxis.which == 0) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue1Y > -JOYSTICK_DEAD_ZONE) && (filaP1 > 0) && (e->jaxis.axis == 1)) {
				filaP1--;
				lastJoyValue1Y = e->jaxis.value;
			} else if ((e->jaxis.which == 0) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue1X < JOYSTICK_DEAD_ZONE) && (columnaP1 < 3) && (e->jaxis.axis == 0)) {
				columnaP1++;
				lastJoyValue1X = e->jaxis.value;
			} else if ((e->jaxis.which == 0) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue1X > -JOYSTICK_DEAD_ZONE) && (columnaP1 > 0) && (e->jaxis.axis == 0)) {
				columnaP1--;
				lastJoyValue1X = e->jaxis.value;
			} else if ((e->jaxis.which == 0) && (e->jaxis.axis == 1)) {
				lastJoyValue1Y = e->jaxis.value;
			} else if ((e->jaxis.which == 0) && (e->jaxis.axis == 0)) {
				lastJoyValue1X = e->jaxis.value;
			}
		}
	break;
	}
}

void GameController::procesarEventosMainScreenTraining(SDL_Event* e) {
	this->tipo_juego = TRAINING;
	int backORplay;
	switch (e->type) {

	case SDL_WINDOWEVENT:
		if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) minimizado = true;
		else if (e->window.event == SDL_WINDOWEVENT_RESTORED) minimizado = false;
	break;
		
	case SDL_MOUSEMOTION:
		backORplay = this->_mainScreen->mouseOverBackOrPlay();
		if (backORplay != NINGUNO)	this->botonSeleccionadoEnModo = backORplay;
	break;

	case SDL_MOUSEBUTTONDOWN:
		backORplay = this->_mainScreen->mouseOverBackOrPlay();
		switch (backORplay) {

		case BACK_BOTON:
			this->screen = MAINSCREEN_MODE_SELECT;
			this->textFocus = TEXT_NO_FOCUS;
			SDL_StopTextInput();
		break;

		case PLAY_BOTON:
			this->screen = NO_MAINSCREEN;
			this->enMainScreen = false;
			this->textFocus = TEXT_NO_FOCUS;
			SDL_StopTextInput();
		break;

		case NINGUNO:
			if (this->_mainScreen->clickOnTextCamp() == TEXT_FOCUS_P1) {
				textFocus = TEXT_FOCUS_P1;
				SDL_StartTextInput();
			} else {
				textFocus = TEXT_NO_FOCUS;
				SDL_StopTextInput();
				if ((this->_mainScreen->faceSelected().first >= 0) && (this->_mainScreen->faceSelected().second >= 0)) {
					this->filaP1 = this->_mainScreen->faceSelected().first;
					this->columnaP1 = this->_mainScreen->faceSelected().second;
				}
			}
		break;
		}
	break;

	case SDL_TEXTINPUT:
		if ((textFocus == TEXT_FOCUS_P1) && (nombreP1.length() < MAX_NAME_LENGTH))	nombreP1 += e->text.text;
	break;

	case SDL_KEYDOWN:
		if (e->key.keysym.sym == SDLK_ESCAPE)	this->_end_of_game = true;
		else if ((e->key.keysym.sym == SDLK_BACKSPACE) && (textFocus == TEXT_FOCUS_P1) && (nombreP1.length() > 0))
			nombreP1.pop_back();
	break;

	case SDL_JOYBUTTONDOWN:
		switch (textFocus) {

			case TEXT_NO_FOCUS:
				if ((e->jdevice.which == 0) && (e->jbutton.button == OK)) {
					this->screen = NO_MAINSCREEN;
					this->enMainScreen = false;
				} else if ((e->jdevice.which == 0) && (e->jbutton.button == BACK)) {
					this->screen = MAINSCREEN_MODE_SELECT;
				} else if ((e->jdevice.which == 0) && (e->jbutton.button == NAME)) {
					textFocus = TEXT_FOCUS_P1;
					SDL_StartTextInput();
				}
			break;

			case TEXT_FOCUS_P1:
				if ((e->jdevice.which == 0) && (e->jbutton.button == OK)) {
					this->screen = NO_MAINSCREEN;
					this->enMainScreen = false;
					this->textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
				} else if ((e->jdevice.which == 0) && (e->jbutton.button == NAME)) {
					textFocus = TEXT_NO_FOCUS;
					SDL_StopTextInput();
				} else if ((e->jdevice.which == 0) && (e->jbutton.button == ERASE) && (nombreP1.length() > 0)) {
					nombreP1.pop_back();
				}
			break;
		}
	break;

	case SDL_JOYAXISMOTION:
		if (textFocus == TEXT_NO_FOCUS) {
			if ((e->jaxis.which == 0) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue1Y < JOYSTICK_DEAD_ZONE) && (filaP1 < 2) && (e->jaxis.axis == 1)) {
				filaP1++;
				lastJoyValue1Y = e->jaxis.value;
			} else if ((e->jaxis.which == 0) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue1Y > -JOYSTICK_DEAD_ZONE) && (filaP1 > 0) && (e->jaxis.axis == 1)) {
				filaP1--;
				lastJoyValue1Y = e->jaxis.value;
			} else if ((e->jaxis.which == 0) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue1X < JOYSTICK_DEAD_ZONE) && (columnaP1 < 3) && (e->jaxis.axis == 0)) {
				columnaP1++;
				lastJoyValue1X = e->jaxis.value;
			} else if ((e->jaxis.which == 0) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue1X > -JOYSTICK_DEAD_ZONE) && (columnaP1 > 0) && (e->jaxis.axis == 0)) {
				columnaP1--;
				lastJoyValue1X = e->jaxis.value;
			} else if ((e->jaxis.which == 0) && (e->jaxis.axis == 1)) {
				lastJoyValue1Y = e->jaxis.value;
			} else if ((e->jaxis.which == 0) && (e->jaxis.axis == 0)) {
				lastJoyValue1X = e->jaxis.value;
			}
		}
	break;
	}
}

void GameController::procesarAxis(SDL_Event* e) {
	if ((e->jaxis.which == 0) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue1Y < JOYSTICK_DEAD_ZONE) && (e->jaxis.axis == 1)) {
		this->_personaje1->getBufferTeclas()->push_back("DW");
		lastJoyValue1Y = e->jaxis.value;
	}
	else if ((e->jaxis.which == 0) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue1Y > -JOYSTICK_DEAD_ZONE) && (e->jaxis.axis == 1)) {
		this->_personaje1->getBufferTeclas()->push_back("UP");
		lastJoyValue1Y = e->jaxis.value;
	}
	else if ((e->jaxis.which == 0) && (e->jaxis.axis == 1)) {
		lastJoyValue1Y = e->jaxis.value;
	}

	if ((e->jaxis.which == 0) && (e->jaxis.value > JOYSTICK_DEAD_ZONE) && (lastJoyValue1X < JOYSTICK_DEAD_ZONE) && (e->jaxis.axis == 0)) {
		this->_personaje1->getBufferTeclas()->push_back("RT");
		lastJoyValue1X = e->jaxis.value;
	}
	else if ((e->jaxis.which == 0) && (e->jaxis.value < -JOYSTICK_DEAD_ZONE) && (lastJoyValue1X > -JOYSTICK_DEAD_ZONE) && (e->jaxis.axis == 0)) {
		this->_personaje1->getBufferTeclas()->push_back("LF");
		lastJoyValue1X = e->jaxis.value;
	}
	else if ((e->jaxis.which == 0) && (e->jaxis.axis == 0)) {
		lastJoyValue1X = e->jaxis.value;
	}

}

void GameController::procesarEventos(SDL_Event* e) {
		switch (e->type) {
		case SDL_JOYBUTTONDOWN:
			this->procesarBotones(e);
			break;
		case SDL_JOYAXISMOTION:
			if (this->estoyEnTraining()) this->procesarAxis(e);
			break;
		case SDL_JOYDEVICEADDED:
			Logger::Instance()->log(DEBUG,"Se ha enchufado un Joystick");
			break;
		case SDL_JOYDEVICEREMOVED:
			Logger::Instance()->log(ERROR,"Se ha desenchufado un Joystick");
			break;
		case SDL_KEYDOWN:
			if (e->key.keysym.sym == SDLK_r) this->toMainScreen();
			else if (e->key.keysym.sym == SDLK_ESCAPE) this->_end_of_game = true;
			else if (e->key.keysym.sym == SDLK_z && this->estoyEnTraining()) this->resetearVentanaPersonajes();
			else if (e->key.keysym.sym == SDLK_v) this->_ventana->toggleShake();
			else if (e->key.keysym.sym == SDLK_1) this->_personaje1->healthPoints -= 10;
			else if (e->key.keysym.sym == SDLK_2) this->_personaje2->healthPoints -= 10;
			//DEBUGEAR PERSONAJE CON TECLADO
			else if (e->key.keysym.sym == SDLK_u) this->_personaje1->patadaBaja();
			else if (e->key.keysym.sym == SDLK_j) this->_personaje1->patadaAlta();
			else if (e->key.keysym.sym == SDLK_i) this->_personaje1->golpeBajo();
			else if (e->key.keysym.sym == SDLK_k) this->_personaje1->golpeAlto();
			else if (e->key.keysym.sym == SDLK_t) this->_personaje1->barrer();
			
			//Prueba caida de barrida, luego se borra
			else if (e->key.keysym.sym == SDLK_q) this->_personaje1->fallSwep(10);
			else if (e->key.keysym.sym == SDLK_7) this->_personaje1->barrer();
			
			break;
		case SDL_WINDOWEVENT:
			if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) {
				minimizado = true;
				this->_fightTimer->pause();
			} else if (e->window.event == SDL_WINDOWEVENT_RESTORED) {
				minimizado = false;
				this->_fightTimer->unpause();
			}
			break;
		}
}

void GameController::procesarMovimientoJoystick() {
	bool estoyEnPVE = this->estoyEnPVE();

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
		else if (AXSP1 < -JOYSTICK_DEAD_ZONE && AYSP1 < -JOYSTICK_DEAD_ZONE) {
			this->_personaje1->jumpLeft(JMP_FACTOR);
				if (estoyEnPVE) this->_personaje1->track_movimientos.push_back(UP_LEFT);
 		}
		else if (AXSP1 > JOYSTICK_DEAD_ZONE && AYSP1 < -JOYSTICK_DEAD_ZONE) {
			this->_personaje1->jumpRight(JMP_FACTOR);
				if (estoyEnPVE) this->_personaje1->track_movimientos.push_back(UP_RIGHT);
		}
		else if (AYSP1 < -JOYSTICK_DEAD_ZONE) {
			this->_personaje1->jump(JMP_FACTOR);
				if (estoyEnPVE) this->_personaje1->track_movimientos.push_back(UP);
		}
		else if (AYSP1 > JOYSTICK_DEAD_ZONE) {
			this->_personaje1->duck();
				if (estoyEnPVE) this->_personaje1->track_movimientos.push_back(DUCK);
		}
		else if (AXSP1 < -JOYSTICK_DEAD_ZONE) {
			if ( !this->_personaje1->isJumping() && !this->_personaje1->isFalling() && canMoveLeft(_personaje1,_personaje2) ) {
				this->_personaje1->moveLeft(MOV_FACTOR2);
				if (estoyEnPVE) this->_personaje1->track_movimientos.push_back(LEFT);
 			}
		}
		else if (AXSP1 > JOYSTICK_DEAD_ZONE) {
			if ( !this->_personaje1->isJumping() && !this->_personaje1->isFalling() && canMoveRight(_personaje1,_personaje2) ) {
				this->_personaje1->moveRight(MOV_FACTOR2);
				if (estoyEnPVE) this->_personaje1->track_movimientos.push_back(RIGHT);
			}
		} else {
			this->_personaje1->idle();
			if (estoyEnPVE) this->_personaje1->track_movimientos.push_back(IDLE);
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
		else if (AXSP2 < -JOYSTICK_DEAD_ZONE && AYSP2 < -JOYSTICK_DEAD_ZONE) {
			this->_personaje2->jumpLeft(JMP_FACTOR);
		}
		else if (AXSP2 > JOYSTICK_DEAD_ZONE && AYSP2 < -JOYSTICK_DEAD_ZONE) {
			this->_personaje2->jumpRight(JMP_FACTOR);
		}
		else if (AYSP2 < -JOYSTICK_DEAD_ZONE) {
			this->_personaje2->jump(JMP_FACTOR);
		}
		else if (AYSP2 > JOYSTICK_DEAD_ZONE) {
			this->_personaje2->duck();
		}
		else if (AXSP2 < -JOYSTICK_DEAD_ZONE) {
			if ( !this->_personaje2->isJumping() && !this->_personaje2->isFalling() && canMoveLeft(_personaje2,_personaje1)) {
				this->_personaje2->moveLeft(MOV_FACTOR2);
 			}
 		}
		else if (AXSP2 > JOYSTICK_DEAD_ZONE) {
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

	if (!this->minimizado)
		this->_mainScreen->showPVP(make_pair(filaP1,columnaP1),make_pair(filaP2,columnaP2),textFocus,nombreP1,nombreP2,botonSeleccionadoEnModo);
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

	if (!this->minimizado)
		this->_mainScreen->showPVE(filaP1,columnaP1,textFocus,nombreP1,botonSeleccionadoEnModo);
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

	if (!this->minimizado)
		this->_mainScreen->showTraining(filaP1,columnaP1,textFocus,nombreP1,botonSeleccionadoEnModo);
	else
		SDL_Delay(DEF_SLEEP_TIME);
}

void GameController::runPVP() {
	
	if (!partidaPreparada) {
		prepararPartida();
		partidaPreparada = true;
	}
	//PROCESO DE INPUTS POR EVENTOS
	SDL_Event e;
	while( SDL_PollEvent(&e) != 0 ) {
		if( e.type == SDL_QUIT ) this->setEndOfGame(true);
		this->procesarEventos(&e);
	}

	if (!this->minimizado) {
		if(!this->musica_pelea)	{
			Mix_HaltMusic();
			this->musica_pelea = true;
			Mix_VolumeMusic(32);
			Mix_PlayMusic(this->musica->musicaPelea, -1);
		}
		if (this->estoyEnPVE()) {
			//Actualizo la lista de movimientos - Si tiene TRACK_MOV elementos elimino el primero
			if(this->_personaje1->track_movimientos.size() >= TRACK_MOV) {
				this->_personaje1->track_movimientos.erase(this->_personaje1->track_movimientos.begin(), this->_personaje1->track_movimientos.begin() + 1);
			}
		}
		//PROCESO DE INPUTS POR ESTADO
		this->procesarMovimientoJoystick();
		if (this->estoyEnPVE() && this->_personaje2->canMove()) {
			this->ai_handler->HandlePlayer(_personaje2, _personaje1);
		}
 		this->getKeys();
 		this->continuarAccionesYMoverCapas();

		tiempoRemanente = (int)ceil(FIGHT_TIME_COUNTDOWN - ((float)this->_fightTimer->getTimeInTicks())/1000);
		this->tiempoRemanenteBuffer = (int)ceil(BUFFER_WAIT_TIME - ((float)this->_bufferTimer->getTimeInTicks())/1000);

		if (this->estoyEnTraining()) {
			this->hayCombo = this->_personaje1->getCombos()->existeCombo(this->_personaje1->getBufferTeclas(),&comboAUX);
			if (this->actualizarGanadorTraining()) {
				this->resetearVentanaPersonajes();
			} else {
				//Si el tiempo de espera llega a cero, actualizo el timer
				if (this->tiempoRemanenteBuffer <= 0) this->_bufferTimer->reset();
			}
		} else { //If not in training
			if (this->actualizarGanador()) {
					this->actualizarPartida();
			}
		}

		this->_ventana->clearScreen();
		this->printLayers();
		if (this->estoyEnTraining()) {
			this->_hud->printHUD(this->_personaje1->getBufferTeclas(), this->hayCombo, comboAUX);
			this->_personaje1->actualizarBufferTeclas(this->tiempoRemanenteBuffer);
		} else {
			this->_hud->printHUD(tiempoRemanente);
		}
		this->_ventana->updateScreen();

	} else { SDL_Delay(DEF_SLEEP_TIME);} //IF (!minimizado)
}

void GameController::continuarAccionesYMoverCapas() {
	this->_personaje1->continueAction(MOV_FACTOR_JMP,JMP_FACTOR,this->_personaje2);
	this->_personaje2->continueAction(MOV_FACTOR_JMP,JMP_FACTOR,this->_personaje1);
	this->moveLayers(_personaje1,_personaje2);
	this->moveLayers(_personaje2,_personaje1);
}

void GameController::actualizarPartida() {
	if (personaje1Wins == 2) {
		Logger::Instance()->log(WARNING,"Partida ganada por personaje 1.");
		//IMPRIMIR CARTEL DE GANADOR EN PANTALLA
		this->toMainScreen();
		} else {
			if (personaje2Wins == 2) {
				Logger::Instance()->log(WARNING,"Partida ganada por personaje 2.");
				//IMPRIMIR CARTEL DE GANADOR EN PANTALLA
				this->toMainScreen();
			} else {
				round++;
				//LANZAR CARTEL DE QUE INICIA EL ROUND
				resetearVentanaPersonajes();
				_hud->actualizarRounds(round,personaje1Wins,personaje2Wins);
				this->_fightTimer->reset();
			}
		}
}

void GameController::runPVE() {
	//Es identico a PVP salvo que pelea AI como personaje 2
	runPVP();
}

void GameController::runTraining() {
	
	this->_personaje2->freeze();
	this->runPVP();
}

void GameController::prepararPartida() {
	prepararPartidaTraining();
	round = 1;
	personaje1Wins = 0;
	personaje2Wins = 0;
	_hud->actualizarRounds(round,personaje1Wins,personaje2Wins);
	this->_fightTimer->reset();
}

void GameController::prepararPartidaTraining() {
	actualizarPersonajes();
	resetearVentanaPersonajes();
	this->_bufferTimer->reset();
	this->_personaje1->getBufferTeclas()->erase(this->_personaje1->getBufferTeclas()->begin(),this->_personaje1->getBufferTeclas()->end());

	_hud->setearPersonajes(_personaje1, _personaje2);
	if (this->nombreP1.length() == 0)	this->nombreP1 = _personaje1->getData()->nombre;
	if (this->nombreP2.length() == 0)	this->nombreP2 = _personaje2->getData()->nombre;
	_hud->recargarNombres();
}

void GameController::run() {
	Logger::Instance()->log(DEBUG,"Comienzo ciclo de Juego");
	while (!this->_end_of_game)
	{
		if (enMainScreen) {
			if (Mix_PlayingMusic() == 0)
			{
				this->musica_pelea = false;
				Mix_VolumeMusic(64);
				Mix_PlayMusic(this->musica->musicaMenu, -1);
			}
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
			switch (modeSelected) {
			case SELECTED_PVP:
				runPVP();
				break;
			case SELECTED_PVE:
				runPVE();
				break;
			case SELECTED_TRAINING:
				runTraining();
				break;
			}
		}
	}
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

void GameController::resetearVentanaPersonajes() {
	this->_ventana->_pos_log_x = (this->_escenario.ancho - this->_ventana->_ancho_log) / 2;
	_personaje1->resetear();
	_personaje2->resetear();
}

void GameController::toMainScreen() {
	resetearVentanaPersonajes();
	Mix_HaltMusic();
	enMainScreen = true;
	screen = MAINSCREEN_MODE_SELECT;
	modeSelected = SELECTED_PVP;
	botonSeleccionadoEnModo = PLAY_BOTON;

	partidaPreparada = false;
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
		if(!this->estoyEnPVE())
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
