#include "MainScreen.h"

MainScreen::~MainScreen()
{
	TTF_CloseFont( fontBig );
	fontBig = NULL;
	TTF_CloseFont( fontSmall );
	fontSmall = NULL;
	TTF_CloseFont( fontMenu );
	fontMenu = NULL;

	delete this->title;
	delete this->titleShadow;
	delete this->pressStart;

	delete this->modePVP_boton;
	delete this->modePVE_boton;
	delete this->modeTraining_boton;
	delete this->thisIsMenu;

	delete this->thisIsPVP;
	delete this->thisIsPVE;
	delete this->thisIsTraining;

	delete this->liukangface;
	delete this->scorpionface;

	delete this->nombreP1_boton;
	delete this->nombreP2_boton;

	delete this->back_boton;
	delete this->play_boton;

	delete this->wallElement;
	delete this->textCamp;
	delete this->selected;
	delete this->textCampSelected;
	delete this->descriptionPlace;

	delete this->gateLeft;
	delete this->gateRight;
}

MainScreen::MainScreen(Ventana* ventana, vector< vector<int> >* perSelect, vector<Personaje*>* punteros) {
	//atributos pasados como parametro
	this->_ventana = ventana;
	this->_perSelect = perSelect;
	this->punterosPersonajes._jugador1liukang = punteros->at(0);
	this->punterosPersonajes._jugador1scorpion = punteros->at(1);
	this->punterosPersonajes._jugador2liukang = punteros->at(2);
	this->punterosPersonajes._jugador2liukangColor = punteros->at(3);
	this->punterosPersonajes._jugador2scorpion = punteros->at(4);
	this->punterosPersonajes._jugador2scorpionColor = punteros->at(5);

	//fuentes
	this->fontBig = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/8);	//Titulo
	this->fontSmall = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/25);	//Descripciones y nombres
	this->fontMenu = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/15);

	//colores
	menuColor.r = 0xCC;
	menuColor.g = 0x00;
	menuColor.b = 0x00;
	menuColor.a = 0xFF;

	nameColor.r = 0xFF;
	nameColor.g = 0xFF;
	nameColor.b = 0xFF;
	nameColor.a = 0xFF;

	descriptionColor.r = 0x00;
	descriptionColor.g = 0x00;
	descriptionColor.b = 0x00;
	descriptionColor.a = 0xFF;

	shadowColor.r = 0x00;
	shadowColor.g = 0x00;
	shadowColor.b = 0x00;
	shadowColor.a = 0xFF;

	selected1Color.r = 0x00;
	selected1Color.g = 0x00;
	selected1Color.b = 0x99;
	selected1Color.a = 0xFF;

	selected2Color.r = 0x00;
	selected2Color.g = 0x99;
	selected2Color.b = 0x00;
	selected2Color.a = 0xFF;

	selectedBothColor.r = 0x00;
	selectedBothColor.g = 0x99;
	selectedBothColor.b = 0x99;
	selectedBothColor.a = 0xFF;

	//wall
	gateLeft = new TextureHandler( _ventana->_gRenderer );
	gateRight = new TextureHandler( _ventana->_gRenderer );
	gateLeft->loadFromFile(GATE_LEFT_PATH,false,0,0,0,true);
	gateRight->loadFromFile(GATE_RIGHT_PATH,false,0,0,0,true);
	
	gateLeftX = -_ventana->_ancho_px*GATE_ANCHO;
	gateRightX = _ventana->_ancho_px;

	wallElement = new TextureHandler( _ventana->_gRenderer );
	wallElement->loadFromFile(WALL_ELEMENT_PATH,false,0,0,0,true);
	textCamp = new TextureHandler( _ventana->_gRenderer );
	textCamp->loadFromFile(TEXT_CAMP_PATH,false,0,0,0,true);
	selected = new TextureHandler( _ventana->_gRenderer );
	selected->loadFromFile(SELECTED_PATH,false,0,0,0,true);
	textCampSelected = new TextureHandler( _ventana->_gRenderer );
	textCampSelected->loadFromFile(TEXT_CAMP_SELECTED_PATH,false,0,0,0,true);
	descriptionPlace = new TextureHandler( _ventana->_gRenderer );
	descriptionPlace->loadFromFile(DESCRIPTION_PLACE_PATH,false,0,0,0,true);

	element_w = _ventana->_ancho_px*2/8;
	element_h = _ventana->_alto_px*1/12;

	//showIntro
	title = new TextureHandler( _ventana->_gRenderer );
	titleShadow = new TextureHandler( _ventana->_gRenderer );
	pressStart = new TextureHandler( _ventana->_gRenderer );

	this->title->loadFromRenderedText("MORTAL TALLER", menuColor, fontBig);
	this->titleShadow->loadFromRenderedText("MORTAL TALLER", shadowColor, fontBig);
	this->pressStart->loadFromRenderedText("press start", descriptionColor, fontSmall);

	descriptionY = this->_ventana->_alto_px*11/12 + (this->_ventana->_alto_px*1/12 - pressStart->getHeight())/2;

	shakeCount = SHAKE_COUNT;
	randomX = 0;
	randomY = 0;

	titleX = _ventana->_ancho_px/2 - title->getWidth()/2;
	titleY = -title->getHeight();
	pressStartX = _ventana->_ancho_px/2 - pressStart->getWidth()/2;
	pressStartY = _ventana->_alto_px;
	
	//modeSelect
	TextureHandler* modePVP = new TextureHandler( _ventana->_gRenderer );	//Elemento de menu modo
	TextureHandler* modePVE = new TextureHandler( _ventana->_gRenderer );	//Elemento de menu modo
	TextureHandler* modeTraining = new TextureHandler( _ventana->_gRenderer );	//Elemento de menu modo

	modePVP->loadFromRenderedText("2 players", menuColor, fontMenu);
	modePVE->loadFromRenderedText("1 player", menuColor, fontMenu);
	modeTraining->loadFromRenderedText("training", menuColor, fontMenu);

	this->modePVP_boton = new Boton(this->_ventana, modePVP, wallElement, this->_ventana->_ancho_px/2, this->_ventana->_alto_px*2/12, element_w, element_h);
	this->modePVE_boton = new Boton(this->_ventana, modePVE, wallElement, this->_ventana->_ancho_px/2, this->_ventana->_alto_px*4/12, element_w, element_h);
	this->modeTraining_boton = new Boton(this->_ventana, modeTraining, wallElement, this->_ventana->_ancho_px/2, this->_ventana->_alto_px*6/12, element_w, element_h);

	thisIsMenu = new TextureHandler( _ventana->_gRenderer );	//Descripcion en menu modo
	thisIsMenu->loadFromRenderedText("[1] Confirm", descriptionColor, fontSmall);
	
	//perSelect
	liukangface = new TextureHandler( _ventana->_gRenderer );
	scorpionface = new TextureHandler( _ventana->_gRenderer );
	liukangface->loadFromFile(LIUKANG_FACE_PATH,false,0,0,0,true);
	scorpionface->loadFromFile(SCORPION_FACE_PATH,false,0,0,0,true);
	prepararPerSelect();
	
	//PVP
	thisIsPVP = new TextureHandler( _ventana->_gRenderer );	//Descripcion en PVP
	this->thisIsPVP->loadFromRenderedText("[3] Name focus, [4] Erase", descriptionColor, fontSmall);

	//PVE
	thisIsPVE = new TextureHandler( _ventana->_gRenderer );	//Descripcion en PVE
	this->thisIsPVE->loadFromRenderedText("[3] Name focus, [4] Erase", descriptionColor, fontSmall);

	//Training
	thisIsTraining = new TextureHandler( _ventana->_gRenderer );	//Descripcion en Training
	this->thisIsTraining->loadFromRenderedText("[3] Name focus, [4] Erase", descriptionColor, fontSmall);

	//nombres
	TextureHandler* nombreP1 = new TextureHandler( _ventana->_gRenderer );
	TextureHandler* nombreP2 = new TextureHandler( _ventana->_gRenderer );
	nombreP1->loadFromRenderedText(NOMBRE_VACIO, nameColor, fontSmall);
	nombreP2->loadFromRenderedText(NOMBRE_VACIO, nameColor, fontSmall);
	nombreP1_boton = new Boton(this->_ventana, textCamp, this->_ventana->_ancho_px*2/8, this->_ventana->_alto_px*8/12, element_w, element_h);
	nombreP2_boton = new Boton(this->_ventana, textCamp, this->_ventana->_ancho_px*6/8, this->_ventana->_alto_px*8/12, element_w, element_h);

	//botones 'back' y 'play'
	TextureHandler* back = new TextureHandler( _ventana->_gRenderer );
	TextureHandler* play = new TextureHandler( _ventana->_gRenderer );
	back->loadFromRenderedText("[2] back", menuColor, fontMenu);
	play->loadFromRenderedText("[1] play", menuColor, fontMenu);
	back_boton = new Boton(this->_ventana, back, wallElement, this->_ventana->_ancho_px*2/8, this->_ventana->_alto_px*10/12, element_w, element_h);
	play_boton = new Boton(this->_ventana, play, wallElement, this->_ventana->_ancho_px*6/8, this->_ventana->_alto_px*10/12, element_w, element_h);
}

void MainScreen::prepararPerSelect() {
	faceH = _ventana->_alto_px*2/12;
	faceW = _ventana->_ancho_px*1/8;
	topLeftX = _ventana->_ancho_px/2 - (faceW * _perSelect->at(0).size())/2;
	topLeftY = _ventana->_alto_px*1/12;

	for (unsigned int fila=0; fila<_perSelect->size(); fila++) {
		vector< pair<int, int> > vectorLocal(_perSelect->at(fila).size());
		for (unsigned int columna=0; columna<_perSelect->at(fila).size(); columna++)
			vectorLocal.at(columna) = make_pair(topLeftX + columna*faceW, topLeftY + fila*faceH);
		posicionesCaras.push_back(vectorLocal);
	}
}

void MainScreen::posicionarGate() {
	gateLeftX = 0;
	gateRightX = _ventana->_ancho_px*(1-GATE_ANCHO);
}

void MainScreen::actualizarPosiciones() {
	if (gateLeftX < 0) {
		gateLeftX = gateLeftX + _ventana->_ancho_px/GATE_SPEED;
		gateRightX = gateRightX - _ventana->_ancho_px/GATE_SPEED;
		if (gateLeftX > 0)	gateLeftX = 0;
		if (gateRightX < _ventana->_ancho_px*(1-GATE_ANCHO))	gateRightX = _ventana->_ancho_px*(1-GATE_ANCHO);
	} else {
		if (shakeCount != 0) {
			shakeCount--;
			randomX = rand() % (_ventana->_ancho_px/SHAKE_FACTOR) - _ventana->_ancho_px/SHAKE_FACTOR/2;
			randomY = rand() % (_ventana->_alto_px/SHAKE_FACTOR) - _ventana->_alto_px/SHAKE_FACTOR/2;
		} else {
			randomX = 0;
			randomY = 0;
		}
	}

	if (titleY < _ventana->_alto_px/5)
		titleY = titleY + _ventana->_alto_px/TITLE_SPEED;

	if (pressStartY > descriptionY) {
		pressStartY = pressStartY - _ventana->_alto_px/PRESS_START_SPEED;
		if (pressStartY < descriptionY)	pressStartY = descriptionY;
	}
}

void MainScreen::showIntro() {
	actualizarPosiciones();

	this->_ventana->clearScreen();

	this->gateLeft->renderScaled(gateLeftX, 0, _ventana->_ancho_px*GATE_ANCHO, _ventana->_alto_px);
	this->gateRight->renderScaled(gateRightX, 0, _ventana->_ancho_px*GATE_ANCHO, _ventana->_alto_px);

	this->titleShadow->render(titleX + _ventana->_ancho_px/300 + randomX, titleY + _ventana->_alto_px/300 + randomY);
	this->title->render(titleX + randomX, titleY + randomY);
	descriptionPlace->renderScaled(this->_ventana->_ancho_px*2/8, this->_ventana->_alto_px*11/12, element_w*2, element_h);
	this->pressStart->render(pressStartX + randomX, pressStartY + randomY);

	this->_ventana->updateScreen();
}

int MainScreen::mouseOverMode() {
	if (this->modePVP_boton->mouseOver()) return SELECTED_PVP;
	else if (this->modePVE_boton->mouseOver()) return SELECTED_PVE;
	else if (this->modeTraining_boton->mouseOver()) return SELECTED_TRAINING;
	else return NOT_SELECTED;
}

int MainScreen::mouseOverBackOrPlay() {
	if (this->back_boton->mouseOver()) return BACK_BOTON;
	else if (this->play_boton->mouseOver()) return PLAY_BOTON;
	else return NINGUNO;
}

int MainScreen::clickOnTextCamp() {
	if (this->nombreP1_boton->mouseOver()) return TEXT_FOCUS_P1;
	else if (this->nombreP2_boton->mouseOver()) return TEXT_FOCUS_P2;
	else return TEXT_NO_FOCUS;
}

pair<int,int> MainScreen::faceSelected() {
	int x,y;
	SDL_GetMouseState(&x, &y);
	pair<int,int> seleccionado = make_pair(-1,-1);

	for (unsigned int fila=0; fila<_perSelect->size(); fila++) {
		for (unsigned int columna=0; columna<_perSelect->at(fila).size(); columna++)
			if ((x >= posicionesCaras.at(fila).at(columna).first) &&
				(x <= posicionesCaras.at(fila).at(columna).first + faceW) &&
				(y >= posicionesCaras.at(fila).at(columna).second) &&
				(y <= posicionesCaras.at(fila).at(columna).second + faceH)) {
					seleccionado.first = fila;
					seleccionado.second = columna;
			}
	}
	return seleccionado;
}

void MainScreen::showModeSelect(int modeSelected) {
	this->_ventana->clearScreen();

	this->gateLeft->renderScaled(gateLeftX, 0, _ventana->_ancho_px*GATE_ANCHO, _ventana->_alto_px);
	this->gateRight->renderScaled(gateRightX, 0, _ventana->_ancho_px*GATE_ANCHO, _ventana->_alto_px);

	switch (modeSelected) {
	case SELECTED_PVP:
		this->modePVP_boton->surfaceTexture = selected;
		this->modePVE_boton->surfaceTexture = wallElement;
		this->modeTraining_boton->surfaceTexture = wallElement;
		break;
	case SELECTED_PVE:
		this->modePVP_boton->surfaceTexture = wallElement;
		this->modePVE_boton->surfaceTexture = selected;
		this->modeTraining_boton->surfaceTexture = wallElement;
		break;
	case SELECTED_TRAINING:
		this->modePVP_boton->surfaceTexture = wallElement;
		this->modePVE_boton->surfaceTexture = wallElement;
		this->modeTraining_boton->surfaceTexture = selected;
		break;
	}

	this->modePVP_boton->view();
	this->modePVE_boton->view();
	this->modeTraining_boton->view();

	descriptionPlace->renderScaled(this->_ventana->_ancho_px*2/8, this->_ventana->_alto_px*11/12, element_w*2, element_h);
	this->thisIsMenu->render(_ventana->_ancho_px/2 - thisIsMenu->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
}

void MainScreen::viewDemoUno(int fila, int columna) {
	if (this->_perSelect->at(fila).at(columna) == LIUKANG) {
		punterosPersonajes._jugador1liukang->posicionarParaMain();
		punterosPersonajes._jugador1liukang->view(punterosPersonajes._jugador2liukang);
	}
	if (this->_perSelect->at(fila).at(columna) == SCORPION) {
		punterosPersonajes._jugador1scorpion->posicionarParaMain();
		punterosPersonajes._jugador1scorpion->view(punterosPersonajes._jugador2liukang);
	}
}

void MainScreen::viewDemoDos(int fila1, int columna1, int fila2, int columna2) {
	if (this->_perSelect->at(fila1).at(columna1) == LIUKANG) {
		punterosPersonajes._jugador1liukang->posicionarParaMain();
		punterosPersonajes._jugador1liukang->view(punterosPersonajes._jugador2liukang);
	}
	if (this->_perSelect->at(fila1).at(columna1) == SCORPION) {
		punterosPersonajes._jugador1scorpion->posicionarParaMain();
		punterosPersonajes._jugador1scorpion->view(punterosPersonajes._jugador2liukang);
	}

	if (this->_perSelect->at(fila2).at(columna2) == LIUKANG) {
		if (this->_perSelect->at(fila2).at(columna2) != this->_perSelect->at(fila1).at(columna1)) {
			punterosPersonajes._jugador2liukang->posicionarParaMain();
			punterosPersonajes._jugador2liukang->view(punterosPersonajes._jugador1liukang);
		} else {
			punterosPersonajes._jugador2liukangColor->posicionarParaMain();
			punterosPersonajes._jugador2liukangColor->view(punterosPersonajes._jugador1liukang);
		}
	}
	if (this->_perSelect->at(fila2).at(columna2) == SCORPION) {
		if (this->_perSelect->at(fila2).at(columna2) != this->_perSelect->at(fila1).at(columna1)) {
			punterosPersonajes._jugador2scorpion->posicionarParaMain();
			punterosPersonajes._jugador2scorpion->view(punterosPersonajes._jugador1liukang);
		} else {
			punterosPersonajes._jugador2scorpionColor->posicionarParaMain();
			punterosPersonajes._jugador2scorpionColor->view(punterosPersonajes._jugador1liukang);
		}
	}
}

void MainScreen::veiwFaces() {
	for (unsigned int i=0; i<posicionesCaras.size(); i++) {
		for (unsigned int j=0; j<posicionesCaras.at(i).size(); j++) {
			switch (_perSelect->at(i).at(j)) {
			case LIUKANG:
				this->liukangface->renderScaled(posicionesCaras.at(i).at(j).first,posicionesCaras.at(i).at(j).second,faceW,faceH);
				break;
			case SCORPION:
				this->scorpionface->renderScaled(posicionesCaras.at(i).at(j).first,posicionesCaras.at(i).at(j).second,faceW,faceH);
				break;
			}
		}
	}
}

void MainScreen::showPVP(pair<int,int> pair1, pair<int,int> pair2, int textFocus, string nombre1, string nombre2, int boton) {
	this->_ventana->clearScreen();

	this->gateLeft->renderScaled(gateLeftX, 0, _ventana->_ancho_px*GATE_ANCHO, _ventana->_alto_px);
	this->gateRight->renderScaled(gateRightX, 0, _ventana->_ancho_px*GATE_ANCHO, _ventana->_alto_px);

	int fila1 = pair1.first;
	int columna1 = pair1.second;
	int fila2 = pair2.first;
	int columna2 = pair2.second;

	veiwFaces();
	viewDemoDos(fila1, columna1, fila2, columna2);

	if ((fila1 != fila2) || (columna1 != columna2)) {
		int selected1X = topLeftX + columna1*faceW;
		int selected1Y = topLeftY + fila1*faceH;
		SDL_Rect selected1a = { selected1X, selected1Y, faceW, faceH };
		SDL_Rect selected1b = { selected1X+1, selected1Y+1, faceW-2, faceH-2 };
		SDL_Rect selected1c = { selected1X+2, selected1Y+2, faceW-4, faceH-4 };
		SDL_SetRenderDrawColor( _ventana->_gRenderer, selected1Color.r, selected1Color.g, selected1Color.b, selected1Color.a );
		SDL_RenderDrawRect( _ventana->_gRenderer, &selected1a );
		SDL_RenderDrawRect( _ventana->_gRenderer, &selected1b );
		SDL_RenderDrawRect( _ventana->_gRenderer, &selected1c );

		int selected2X = topLeftX + columna2*faceW;
		int selected2Y = topLeftY + fila2*faceH;
		SDL_Rect selected2a = { selected2X, selected2Y, faceW, faceH };
		SDL_Rect selected2b = { selected2X+1, selected2Y+1, faceW-2, faceH-2 };
		SDL_Rect selected2c = { selected2X+2, selected2Y+2, faceW-4, faceH-4 };
		SDL_SetRenderDrawColor( _ventana->_gRenderer, selected2Color.r, selected2Color.g, selected2Color.b, selected2Color.a );
		SDL_RenderDrawRect( _ventana->_gRenderer, &selected2a );
		SDL_RenderDrawRect( _ventana->_gRenderer, &selected2b );
		SDL_RenderDrawRect( _ventana->_gRenderer, &selected2c );

	} else {
		int selected1X = topLeftX + columna1*faceW;
		int selected1Y = topLeftY + fila1*faceH;
		SDL_Rect selected1a = { selected1X, selected1Y, faceW, faceH };
		SDL_Rect selected1b = { selected1X+1, selected1Y+1, faceW-2, faceH-2 };
		SDL_Rect selected1c = { selected1X+2, selected1Y+2, faceW-4, faceH-4 };
		SDL_SetRenderDrawColor( _ventana->_gRenderer, selectedBothColor.r, selectedBothColor.g, selectedBothColor.b, selectedBothColor.a );
		SDL_RenderDrawRect( _ventana->_gRenderer, &selected1a );
		SDL_RenderDrawRect( _ventana->_gRenderer, &selected1b );
		SDL_RenderDrawRect( _ventana->_gRenderer, &selected1c );
	}

	if ((nombre1.length() > 0) && (nombre2.length() > 0)) {

		if (textFocus == TEXT_NO_FOCUS) {	//Ambos no nulos y no en foco
			viewName( nombreP1_boton, nombre1, false );
			viewName( nombreP2_boton, nombre2, false );
		} else if (textFocus == TEXT_FOCUS_P1) {	//Ambos no nulos, 1-ro en foco
			viewName( nombreP1_boton, nombre1, true );
			viewName( nombreP2_boton, nombre2, false );
		} else {	//Ambos no nulos, 2-do en foco
			viewName( nombreP1_boton, nombre1, false );
			viewName( nombreP2_boton, nombre2, true );
		}

	} else if ((nombre1.length() > 0) && (nombre2.length() == 0)) {

		if (textFocus == TEXT_NO_FOCUS) {	//2-do nulo, nadie en foco
			viewName( nombreP1_boton, nombre1, false );
			viewName( nombreP2_boton, NOMBRE_VACIO, false );
		} else if (textFocus == TEXT_FOCUS_P1) {	//2-do nulo, 1-ro en foco
			viewName( nombreP1_boton, nombre1, true );
			viewName( nombreP2_boton, NOMBRE_VACIO, false );
		} else {	//2-do nulo, 2-do en foco
			viewName( nombreP1_boton, nombre1, false );
			viewName( nombreP2_boton, NOMBRE_VACIO, true );
		}
	
	} else if ((nombre1.length() == 0) && (nombre2.length() > 0)) {

		if (textFocus == TEXT_NO_FOCUS) {	//1-ro nulo, nadie en foco
			viewName( nombreP1_boton, NOMBRE_VACIO, false );
			viewName( nombreP2_boton, nombre2, false );
		} else if (textFocus == TEXT_FOCUS_P1) {	//1-ro nulo, 1-ro en foco
			viewName( nombreP1_boton, NOMBRE_VACIO, true );
			viewName( nombreP2_boton, nombre2, false );
		} else {	//1-ro nulo, 2-do en foco
			viewName( nombreP1_boton, NOMBRE_VACIO, false );
			viewName( nombreP2_boton, nombre2, true );
		}

	} else {

		if (textFocus == TEXT_NO_FOCUS) {	//ambos nulos, nadie en foco
			viewName( nombreP1_boton, NOMBRE_VACIO, false );
			viewName( nombreP2_boton, NOMBRE_VACIO, false );
		} else if (textFocus == TEXT_FOCUS_P1) {	//ambos nulos, 1-ro en foco
			viewName( nombreP1_boton, NOMBRE_VACIO, true );
			viewName( nombreP2_boton, NOMBRE_VACIO, false );
		} else {	//ambos nulos, 2-do en foco
			viewName( nombreP1_boton, NOMBRE_VACIO, false );
			viewName( nombreP2_boton, NOMBRE_VACIO, true );
		}

	}

	if (boton == BACK_BOTON) {
		this->back_boton->surfaceTexture = selected;
		this->play_boton->surfaceTexture = wallElement;
	}
	if (boton == PLAY_BOTON) {
		this->back_boton->surfaceTexture = wallElement;
		this->play_boton->surfaceTexture = selected;
	}
	this->back_boton->view();
	this->play_boton->view();

	descriptionPlace->renderScaled(this->_ventana->_ancho_px*2/8, this->_ventana->_alto_px*11/12, element_w*2, element_h);
	this->thisIsPVP->render(_ventana->_ancho_px/2 - thisIsPVP->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
}

void MainScreen::showPVE(int fila, int columna, int textFocus, string nombre, int boton) {
	showTraining(fila, columna, textFocus, nombre, boton);
}

void MainScreen::viewName(Boton* nombreBoton, string nombre, bool elegido) {
	if (elegido)	nombreBoton->surfaceTexture = textCampSelected;
	else	nombreBoton->surfaceTexture = textCamp;
	nombreBoton->view();

	TextureHandler* nombreTexture = new TextureHandler(this->_ventana->_gRenderer);
	nombreTexture->loadFromRenderedText(nombre,nameColor,fontSmall);
	if (nombreTexture->getWidth() <= nombreBoton->w)
		nombreTexture->render(nombreBoton->x, nombreBoton->y + (nombreBoton->h - nombreTexture->getHeight())/2);
	else {
		SDL_Rect parteRenderizar = { nombreTexture->getWidth() - nombreBoton->w, 0, nombreBoton->w, nombreTexture->getHeight() };
		nombreTexture->renderAnimation(false, nombreBoton->x, nombreBoton->y + (nombreBoton->h - nombreTexture->getHeight())/2, nombreBoton->w, nombreTexture->getHeight(), &parteRenderizar);
	};
	delete nombreTexture;
}

void MainScreen::showTraining(int fila, int columna, int textFocus, string nombre, int boton) {
	this->_ventana->clearScreen();

	this->gateLeft->renderScaled(gateLeftX, 0, _ventana->_ancho_px*GATE_ANCHO, _ventana->_alto_px);
	this->gateRight->renderScaled(gateRightX, 0, _ventana->_ancho_px*GATE_ANCHO, _ventana->_alto_px);

	veiwFaces();
	viewDemoUno(fila,columna);

	int selectedX = topLeftX + columna*faceW;
	int selectedY = topLeftY + fila*faceH;
	SDL_Rect selected1 = { selectedX, selectedY, faceW, faceH };
	SDL_Rect selected2 = { selectedX+1, selectedY+1, faceW-2, faceH-2 };
	SDL_Rect selected3 = { selectedX+2, selectedY+2, faceW-4, faceH-4 };
	SDL_SetRenderDrawColor( _ventana->_gRenderer, selected1Color.r, selected1Color.g, selected1Color.b, selected1Color.a );
	SDL_RenderDrawRect( _ventana->_gRenderer, &selected1 );
	SDL_RenderDrawRect( _ventana->_gRenderer, &selected2 );
	SDL_RenderDrawRect( _ventana->_gRenderer, &selected3 );

	if (nombre.length() > 0) {
		if (textFocus == TEXT_NO_FOCUS) viewName( nombreP1_boton, nombre, false );
		else if (textFocus == TEXT_FOCUS_P1) viewName( nombreP1_boton, nombre, true );
	} else {
		if (textFocus == TEXT_NO_FOCUS) viewName( nombreP1_boton, NOMBRE_VACIO, false );
		else if (textFocus == TEXT_FOCUS_P1) viewName( nombreP1_boton, NOMBRE_VACIO, true );
	}

	if (boton == BACK_BOTON) {
		this->back_boton->surfaceTexture = selected;
		this->play_boton->surfaceTexture = wallElement;
	}
	if (boton == PLAY_BOTON) {
		this->back_boton->surfaceTexture = wallElement;
		this->play_boton->surfaceTexture = selected;
	}
	this->back_boton->view();
	this->play_boton->view();

	descriptionPlace->renderScaled(this->_ventana->_ancho_px*2/8, this->_ventana->_alto_px*11/12, element_w*2, element_h);
	this->thisIsTraining->render(_ventana->_ancho_px/2 - thisIsTraining->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
}