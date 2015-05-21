#include "MainScreen.h"

MainScreen::~MainScreen()
{
	TTF_CloseFont( fontBig );
	fontBig = NULL;
	TTF_CloseFont( fontSmall );
	fontSmall = NULL;

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
}

MainScreen::MainScreen(Ventana* ventana, vector< vector<int> >* perSelect) {
	//atributos pasados como parametro
	this->_ventana = ventana;
	this->_perSelect = perSelect;

	//fuentes
	this->fontBig = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/8);	//Titulo
	this->fontSmall = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/25);	//Descripciones
	this->fontMenu = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/15);

	//colores
	textColor.r = 0xCC;
	textColor.g = 0x00;
	textColor.b = 0x00;
	textColor.a = 0xFF;

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

	notSelectedColor.r = 0x99;
	notSelectedColor.g = 0x99;
	notSelectedColor.b = 0x99;
	notSelectedColor.a = 0xFF;

	//otro
	descriptionY = _ventana->_alto_px*9/10;

	//showIntro
	title = new TextureHandler( _ventana->_gRenderer );
	titleShadow = new TextureHandler( _ventana->_gRenderer );
	pressStart = new TextureHandler( _ventana->_gRenderer );

	this->title->loadFromRenderedText("MORTAL TALLER", textColor, fontBig);
	this->titleShadow->loadFromRenderedText("MORTAL TALLER", shadowColor, fontBig);
	this->pressStart->loadFromRenderedText("press m", textColor, fontSmall);

	shakeCount = SHAKE_COUNT;
	randomX = 0;
	randomY = 0;

	titleX = _ventana->_ancho_px/2 - title->getWidth()/2;
	titleY = -title->getHeight();
	pressStartX = _ventana->_ancho_px/2 - pressStart->getWidth()/2;
	pressStartY = _ventana->_alto_px;

	int gateAncho = _ventana->_ancho_px/2 + MARGIN;
	int gateAlto = _ventana->_alto_px;

	gateLeft.x = -gateAncho;
	gateLeft.y = 0;
	gateLeft.w = gateAncho;
	gateLeft.h = gateAlto;

	gateRight.x = _ventana->_ancho_px;
	gateRight.y = 0;
	gateRight.w = gateAncho;
	gateRight.h = gateAlto;
	
	//modeSelect
	TextureHandler* modePVP = new TextureHandler( _ventana->_gRenderer );	//Elemento de menu modo
	TextureHandler* modePVE = new TextureHandler( _ventana->_gRenderer );	//Elemento de menu modo
	TextureHandler* modeTraining = new TextureHandler( _ventana->_gRenderer );	//Elemento de menu modo

	modePVP->loadFromRenderedText("2 players", textColor, fontMenu);
	modePVE->loadFromRenderedText("1 player", textColor, fontMenu);
	modeTraining->loadFromRenderedText("training", textColor, fontMenu);

	this->modePVP_boton = new Boton(this->_ventana, modePVP, this->_ventana->_ancho_px/2, this->_ventana->_alto_px*1/5);
	this->modePVE_boton = new Boton(this->_ventana, modePVE, this->_ventana->_ancho_px/2, this->_ventana->_alto_px*2/5);
	this->modeTraining_boton = new Boton(this->_ventana, modeTraining, this->_ventana->_ancho_px/2, this->_ventana->_alto_px*3/5);

	thisIsMenu = new TextureHandler( _ventana->_gRenderer );	//Descripcion en menu modo
	thisIsMenu->loadFromRenderedText("[Up], [Down], [Enter]", textColor, fontSmall);
	
	//perSelect
	liukangface = new TextureHandler( _ventana->_gRenderer );
	scorpionface = new TextureHandler( _ventana->_gRenderer );
	liukangface->loadFromFile(LIUKANG_FACE_PATH,false,0,0,0,true);
	scorpionface->loadFromFile(SCORPION_FACE_PATH,false,0,0,0,true);
	prepararPerSelect();
	
	//PVP
	thisIsPVP = new TextureHandler( _ventana->_gRenderer );	//Descripcion en PVP
	this->thisIsPVP->loadFromRenderedText("[b], [Enter], [Arrows], [F1], [PGup,PGdown,Ins,Del], [F2], [BS]", textColor, fontSmall);

	//PVE
	thisIsPVE = new TextureHandler( _ventana->_gRenderer );	//Descripcion en PVE
	this->thisIsPVE->loadFromRenderedText("[b], [Enter], [Up,Down,Left,Right], [Tab], [BS]", textColor, fontSmall);

	//Training
	thisIsTraining = new TextureHandler( _ventana->_gRenderer );	//Descripcion en Training
	this->thisIsTraining->loadFromRenderedText("[b], [Enter], [Up,Down,Left,Right], [Tab], [BS]", textColor, fontSmall);

	//nombres
	TextureHandler* nombreP1 = new TextureHandler( _ventana->_gRenderer );
	TextureHandler* nombreP2 = new TextureHandler( _ventana->_gRenderer );
	nombreP1->loadFromRenderedText(NOMBRE_VACIO, textColor, fontMenu);
	nombreP2->loadFromRenderedText(NOMBRE_VACIO, textColor, fontMenu);
	nombreP1_boton = new Boton(this->_ventana, nombreP1, _ventana->_ancho_px*1/4, _ventana->_alto_px*7/10);
	nombreP2_boton = new Boton(this->_ventana, nombreP2, _ventana->_ancho_px*3/4, _ventana->_alto_px*7/10);

	//botones 'back' y 'play'
	TextureHandler* back = new TextureHandler( _ventana->_gRenderer );
	TextureHandler* play = new TextureHandler( _ventana->_gRenderer );
	back->loadFromRenderedText("back", textColor, fontMenu);
	play->loadFromRenderedText("play", textColor, fontMenu);
	back_boton = new Boton(this->_ventana, back, _ventana->_ancho_px*1/4, _ventana->_alto_px*8/10);
	play_boton = new Boton(this->_ventana, play, _ventana->_ancho_px*3/4, _ventana->_alto_px*8/10);
}

void MainScreen::prepararPerSelect() {
	faceH = _ventana->_alto_px/6;
	faceW = _ventana->_ancho_px/9;
	topLeftX = _ventana->_ancho_px/2 - (faceW * _perSelect->at(0).size())/2;
	topLeftY = _ventana->_alto_px/15;

	for (unsigned int fila=0; fila<_perSelect->size(); fila++) {
		vector< pair<int, int> > vectorLocal(_perSelect->at(fila).size());
		for (unsigned int columna=0; columna<_perSelect->at(fila).size(); columna++)
			vectorLocal.at(columna) = make_pair(topLeftX + columna*faceW, topLeftY + fila*faceH);
		posicionesCaras.push_back(vectorLocal);
	}
}

void MainScreen::actualizarPosiciones() {
	if (gateLeft.x < -MARGIN) {
		gateLeft.x = gateLeft.x + gateLeft.w/GATE_SPEED;
		gateRight.x = gateRight.x - gateRight.w/GATE_SPEED;
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

	if (pressStartY > _ventana->_alto_px*4/5)
		pressStartY = pressStartY - _ventana->_alto_px/PRESS_START_SPEED;
}

void MainScreen::showIntro() {
	actualizarPosiciones();

	this->_ventana->clearScreen();

	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0x77, 0x77, 0x77, 0xFF );
	SDL_RenderFillRect( _ventana->_gRenderer, &gateLeft );
	SDL_RenderFillRect( _ventana->_gRenderer, &gateRight );

	this->titleShadow->render(titleX + _ventana->_ancho_px/300 + randomX, titleY + _ventana->_alto_px/300 + randomY);
	this->title->render(titleX + randomX, titleY + randomY);
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

	switch (modeSelected) {
	case SELECTED_PVP:
		this->modePVP_boton->viewHighlight(&selected1Color);
		break;
	case SELECTED_PVE:
		this->modePVE_boton->viewHighlight(&selected1Color);
		break;
	case SELECTED_TRAINING:
		this->modeTraining_boton->viewHighlight(&selected1Color);
		break;
	}

	this->modePVP_boton->view();
	this->modePVE_boton->view();
	this->modeTraining_boton->view();

	this->thisIsMenu->render(_ventana->_ancho_px/2 - thisIsMenu->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
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

	int fila1 = pair1.first;
	int columna1 = pair1.second;
	int fila2 = pair2.first;
	int columna2 = pair2.second;

	veiwFaces();

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
			viewName( nombreP1_boton, nombre1, &notSelectedColor );
			viewName( nombreP2_boton, nombre2, &notSelectedColor );
		} else if (textFocus == TEXT_FOCUS_P1) {	//Ambos no nulos, 1-ro en foco
			viewName( nombreP1_boton, nombre1, &selected1Color );
			viewName( nombreP2_boton, nombre2, &notSelectedColor );
		} else {	//Ambos no nulos, 2-do en foco
			viewName( nombreP1_boton, nombre1, &notSelectedColor );
			viewName( nombreP2_boton, nombre2, &selected2Color );
		}

	} else if ((nombre1.length() > 0) && (nombre2.length() == 0)) {

		if (textFocus == TEXT_NO_FOCUS) {	//2-do nulo, nadie en foco
			viewName( nombreP1_boton, nombre1, &notSelectedColor );
			viewName( nombreP2_boton, NOMBRE_VACIO, &notSelectedColor );
		} else if (textFocus == TEXT_FOCUS_P1) {	//2-do nulo, 1-ro en foco
			viewName( nombreP1_boton, nombre1, &selected1Color );
			viewName( nombreP2_boton, NOMBRE_VACIO, &notSelectedColor );
		} else {	//2-do nulo, 2-do en foco
			viewName( nombreP1_boton, nombre1, &notSelectedColor );
			viewName( nombreP2_boton, NOMBRE_VACIO, &selected2Color );
		}
	
	} else if ((nombre1.length() == 0) && (nombre2.length() > 0)) {

		if (textFocus == TEXT_NO_FOCUS) {	//1-ro nulo, nadie en foco
			viewName( nombreP1_boton, NOMBRE_VACIO, &notSelectedColor );
			viewName( nombreP2_boton, nombre2, &notSelectedColor );
		} else if (textFocus == TEXT_FOCUS_P1) {	//1-ro nulo, 1-ro en foco
			viewName( nombreP1_boton, NOMBRE_VACIO, &selected1Color );
			viewName( nombreP2_boton, nombre2, &notSelectedColor );
		} else {	//1-ro nulo, 2-do en foco
			viewName( nombreP1_boton, NOMBRE_VACIO, &notSelectedColor );
			viewName( nombreP2_boton, nombre2, &selected2Color );
		}

	} else {

		if (textFocus == TEXT_NO_FOCUS) {	//ambos nulos, nadie en foco
			viewName( nombreP1_boton, NOMBRE_VACIO, &notSelectedColor );
			viewName( nombreP2_boton, NOMBRE_VACIO, &notSelectedColor );
		} else if (textFocus == TEXT_FOCUS_P1) {	//ambos nulos, 1-ro en foco
			viewName( nombreP1_boton, NOMBRE_VACIO, &selected1Color );
			viewName( nombreP2_boton, NOMBRE_VACIO, &notSelectedColor );
		} else {	//ambos nulos, 2-do en foco
			viewName( nombreP1_boton, NOMBRE_VACIO, &notSelectedColor );
			viewName( nombreP2_boton, NOMBRE_VACIO, &selected2Color );
		}

	}

	if (boton == BACK_BOTON)
		this->back_boton->viewHighlight(&selected1Color);
	if (boton == PLAY_BOTON)
		this->play_boton->viewHighlight(&selected1Color);
	this->back_boton->view();
	this->play_boton->view();

	this->thisIsPVP->render(_ventana->_ancho_px/2 - thisIsPVP->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
}

void MainScreen::showPVE(int fila, int columna, int textFocus, string nombre, int boton) {
	showTraining(fila,columna,textFocus,nombre, boton);
}

void MainScreen::viewName(Boton* nombreBoton, string nombre, SDL_Color* color) {
	nombreBoton->texture->loadFromRenderedText(nombre.c_str(), textColor, fontSmall);
	nombreBoton->view();
	nombreBoton->viewExternBox(color);
}

void MainScreen::showTraining(int fila, int columna, int textFocus, string nombre, int boton) {
	this->_ventana->clearScreen();

	veiwFaces();

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
		if (textFocus == TEXT_NO_FOCUS) viewName( nombreP1_boton, nombre, &notSelectedColor );
		else if (textFocus == TEXT_FOCUS_P1) viewName( nombreP1_boton, nombre, &selected1Color );
	} else {
		if (textFocus == TEXT_NO_FOCUS) viewName( nombreP1_boton, NOMBRE_VACIO, &notSelectedColor );
		else if (textFocus == TEXT_FOCUS_P1) viewName( nombreP1_boton, NOMBRE_VACIO, &selected1Color );
	}

	if (boton == BACK_BOTON)
		this->back_boton->viewHighlight(&selected1Color);
	if (boton == PLAY_BOTON)
		this->play_boton->viewHighlight(&selected1Color);
	this->back_boton->view();
	this->play_boton->view();

	this->thisIsTraining->render(_ventana->_ancho_px/2 - thisIsTraining->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
}