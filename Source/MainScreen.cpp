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

	delete this->modePVP;
	delete this->modePVE;
	delete this->modeTraining;
	delete this->thisIsMenu;

	delete this->thisIsPVP;
	delete this->thisIsPVE;
	delete this->thisIsTraining;

	delete this->liukangface;
	delete this->scorpionface;

	delete this->nombreP1;
	delete this->nombreP2;
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

	notSelected.r = 0x99;
	notSelected.g = 0x99;
	notSelected.b = 0x99;
	notSelected.a = 0xFF;

	//texturas de texto: news
	title = new TextureHandler( _ventana->_gRenderer );
	titleShadow = new TextureHandler( _ventana->_gRenderer );
	pressStart = new TextureHandler( _ventana->_gRenderer );
	
	modePVP = new TextureHandler( _ventana->_gRenderer );	//Elemento de menu modo
	modePVE = new TextureHandler( _ventana->_gRenderer );	//Elemento de menu modo
	modeTraining = new TextureHandler( _ventana->_gRenderer );	//Elemento de menu modo
	thisIsMenu = new TextureHandler( _ventana->_gRenderer );	//Descripcion en menu modo

	thisIsPVP = new TextureHandler( _ventana->_gRenderer );	//Descripcion en PVP
	thisIsPVE = new TextureHandler( _ventana->_gRenderer );	//Descripcion en PVE
	thisIsTraining = new TextureHandler( _ventana->_gRenderer );	//Descripcion en Training

	//texturas de texto: loads
	this->title->loadFromRenderedText("MORTAL TALLER", textColor, fontBig);
	this->titleShadow->loadFromRenderedText("MORTAL TALLER", shadowColor, fontBig);
	this->pressStart->loadFromRenderedText("press m", textColor, fontSmall);

	this->modePVP->loadFromRenderedText("2 players", textColor, fontMenu);
	this->modePVE->loadFromRenderedText("1 player", textColor, fontMenu);
	this->modeTraining->loadFromRenderedText("training", textColor, fontMenu);
	this->thisIsMenu->loadFromRenderedText("[Up], [Down], [Enter]", textColor, fontSmall);

	this->thisIsPVP->loadFromRenderedText("[b], [Enter], [Up,Down,Left,Right], [F1], [PGup,PGdown,Ins,Del], [F2]", textColor, fontSmall);
	this->thisIsPVE->loadFromRenderedText("PVE: [b] back, [g] game", textColor, fontSmall);
	this->thisIsTraining->loadFromRenderedText("Seleccion: [b], [Enter], [Tab]. Nombre: [BS], [Enter], [Tab].", textColor, fontSmall);
	
	//parametros del intro
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

	//perSelect
	liukangface = new TextureHandler( _ventana->_gRenderer );
	scorpionface = new TextureHandler( _ventana->_gRenderer );
	liukangface->loadFromFile(LIUKANG_FACE_PATH,false,0,0,0,true);
	scorpionface->loadFromFile(SCORPION_FACE_PATH,false,0,0,0,true);
	prepararPerSelect();

	//Nombres
	nombreP1 = new TextureHandler( _ventana->_gRenderer );
	nombreP2 = new TextureHandler( _ventana->_gRenderer );

	//Extras
	descriptionY = _ventana->_alto_px*9/10;
	nombreY = _ventana->_alto_px*8/10;
	nombreEjeX = _ventana->_ancho_px/2;
	nombre1EjeX = _ventana->_ancho_px/4;
	nombre2EjeX = _ventana->_ancho_px*3/4;
}

void MainScreen::prepararPerSelect() {
	faceH = _ventana->_alto_px/5;
	faceW = _ventana->_ancho_px/8;
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

void MainScreen::showModeSelect(int modeSelected) {
	this->_ventana->clearScreen();
	SDL_Rect highlight;
	highlight.h = modePVP->getHeight();
	SDL_SetRenderDrawColor( _ventana->_gRenderer, selected1Color.r, selected1Color.g, selected1Color.b, selected1Color.a );

	switch (modeSelected) {
	case SELECTED_PVP:
		highlight.w = modePVP->getWidth();
		highlight.x = _ventana->_ancho_px/2 - modePVP->getWidth()/2;
		highlight.y = _ventana->_alto_px*1/5;
		break;
	case SELECTED_PVE:
		highlight.w = modePVE->getWidth();
		highlight.x = _ventana->_ancho_px/2 - modePVE->getWidth()/2;
		highlight.y = _ventana->_alto_px*2/5;
		break;
	case SELECTED_TRAINING:
		highlight.w = modeTraining->getWidth();
		highlight.x = _ventana->_ancho_px/2 - modeTraining->getWidth()/2;
		highlight.y = _ventana->_alto_px*3/5;
		break;
	}
	SDL_RenderFillRect( _ventana->_gRenderer, &highlight );

	this->modePVP->render(_ventana->_ancho_px/2 - modePVP->getWidth()/2, _ventana->_alto_px*1/5);
	this->modePVE->render(_ventana->_ancho_px/2 - modePVE->getWidth()/2, _ventana->_alto_px*2/5);
	this->modeTraining->render(_ventana->_ancho_px/2 - modeTraining->getWidth()/2, _ventana->_alto_px*3/5);

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

void MainScreen::showPVP(int fila1, int columna1, int fila2, int columna2, int textFocus, string nombre1, string nombre2) {
	this->_ventana->clearScreen();

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
			viewNameBoxNoFocus( nombreP1, viewName(nombreP1,nombre1,nombre1EjeX) );
			viewNameBoxNoFocus( nombreP2, viewName(nombreP2,nombre2,nombre2EjeX) );
		} else if (textFocus == TEXT_FOCUS_P1) {	//Ambos no nulos, 1-ro en foco
			viewNameBoxFocus( nombreP1, &selected1Color, viewName(nombreP1,nombre1,nombre1EjeX) );
			viewNameBoxNoFocus( nombreP2, viewName(nombreP2,nombre2,nombre2EjeX) );
		} else {	//Ambos no nulos, 2-do en foco
			viewNameBoxNoFocus( nombreP1, viewName(nombreP1,nombre1,nombre1EjeX) );
			viewNameBoxFocus( nombreP2, &selected2Color, viewName(nombreP2,nombre2,nombre2EjeX) );
		}

	} else if ((nombre1.length() > 0) && (nombre2.length() == 0)) {

		if (textFocus == TEXT_NO_FOCUS) {	//2-do nulo, nadie en foco
			viewNameBoxNoFocus( nombreP1, viewName(nombreP1,nombre1,nombre1EjeX) );
			viewNameBoxNoFocus( nombreP2, viewName(nombreP2," ",nombre2EjeX) );
		} else if (textFocus == TEXT_FOCUS_P1) {	//2-do nulo, 1-ro en foco
			viewNameBoxFocus(nombreP1, &selected1Color, viewName(nombreP1,nombre1,nombre1EjeX));
			viewNameBoxNoFocus( nombreP2, viewName(nombreP2," ",nombre2EjeX) );
		} else {	//2-do nulo, 2-do en foco
			viewNameBoxNoFocus( nombreP1, viewName(nombreP1,nombre1,nombre1EjeX) );
			viewNameBoxFocus( nombreP2, &selected2Color, viewName(nombreP2," ",nombre2EjeX) );
		}
	
	} else if ((nombre1.length() == 0) && (nombre2.length() > 0)) {

		if (textFocus == TEXT_NO_FOCUS) {	//1-ro nulo, nadie en foco
			viewNameBoxNoFocus( nombreP1, viewName(nombreP1," ",nombre1EjeX) );
			viewNameBoxNoFocus( nombreP2, viewName(nombreP2,nombre2,nombre2EjeX) );
		} else if (textFocus == TEXT_FOCUS_P1) {	//1-ro nulo, 1-ro en foco
			viewNameBoxFocus( nombreP1, &selected1Color, viewName(nombreP1," ",nombre1EjeX) );
			viewNameBoxNoFocus( nombreP2, viewName(nombreP2,nombre2,nombre2EjeX) );
		} else {	//1-ro nulo, 2-do en foco
			viewNameBoxNoFocus( nombreP1, viewName(nombreP1," ",nombre1EjeX) );
			viewNameBoxFocus( nombreP2, &selected2Color, viewName(nombreP2,nombre2,nombre2EjeX) );
		}

	} else {

		if (textFocus == TEXT_NO_FOCUS) {	//ambos nulos, nadie en foco
			viewNameBoxNoFocus( nombreP1, viewName(nombreP1," ",nombre1EjeX) );
			viewNameBoxNoFocus( nombreP2, viewName(nombreP2," ",nombre2EjeX) );
		} else if (textFocus == TEXT_FOCUS_P1) {	//ambos nulos, 1-ro en foco
			viewNameBoxFocus( nombreP1, &selected1Color, viewName(nombreP1," ",nombre1EjeX) );
			viewNameBoxNoFocus( nombreP2, viewName(nombreP2," ",nombre2EjeX) );
		} else {	//ambos nulos, 2-do en foco
			viewNameBoxNoFocus( nombreP1, viewName(nombreP1," ",nombre1EjeX) );
			viewNameBoxFocus( nombreP2, &selected2Color, viewName(nombreP2," ",nombre2EjeX) );
		}

	}

	this->thisIsPVP->render(_ventana->_ancho_px/2 - thisIsPVP->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
}

void MainScreen::showPVE() {
	this->_ventana->clearScreen();
	this->modePVE->render(_ventana->_ancho_px/2 - modePVE->getWidth()/2, _ventana->_alto_px*1/5);
	this->thisIsPVE->render(_ventana->_ancho_px/2 - thisIsPVE->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
}

int MainScreen::viewName(TextureHandler* nombreTexture, string nombre, int ejeX) {
	nombreTexture->loadFromRenderedText(nombre.c_str(), textColor, fontSmall);
	int nombreX = ejeX - nombreTexture->getWidth()/2;
	nombreTexture->render(nombreX, nombreY);
	return nombreX;
}

void MainScreen::viewNameBoxFocus(TextureHandler* nombre, SDL_Color* color, int x) {
	SDL_SetRenderDrawColor( _ventana->_gRenderer, color->r, color->g, color->b, color->a );
	SDL_Rect textBounds = { x-1, nombreY-1, nombre->getWidth()+2, nombre->getHeight()+2 };
	SDL_RenderDrawRect( _ventana->_gRenderer, &textBounds );
}

void MainScreen::viewNameBoxNoFocus(TextureHandler* nombre, int x) {
	SDL_SetRenderDrawColor( _ventana->_gRenderer, notSelected.r, notSelected.g, notSelected.b, notSelected.a );
	SDL_Rect textBounds = { x-1, nombreY-1, nombre->getWidth()+2, nombre->getHeight()+2 };
	SDL_RenderDrawRect( _ventana->_gRenderer, &textBounds );
}

void MainScreen::showTraining(int fila, int columna, int textFocus, string nombre) {
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
		if (textFocus == TEXT_NO_FOCUS) viewNameBoxNoFocus( nombreP1, viewName(nombreP1,nombre,nombreEjeX) );
		else if (textFocus == TEXT_FOCUS_P1) viewNameBoxFocus(nombreP1, &selected1Color, viewName(nombreP1,nombre,nombreEjeX));
	} else {
		if (textFocus == TEXT_NO_FOCUS) viewNameBoxNoFocus(nombreP1, viewName(nombreP1," ",nombreEjeX) );
		else if (textFocus == TEXT_FOCUS_P1) viewNameBoxFocus(nombreP1, &selected1Color, viewName(nombreP1," ",nombreEjeX) );
	}

	this->thisIsTraining->render(_ventana->_ancho_px/2 - thisIsTraining->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
}