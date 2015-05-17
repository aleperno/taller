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
	delete this->press;

	delete this->thisIsPVP;
	delete this->thisIsPVE;
	delete this->thisIsTraining;

	delete this->liukangface;
	delete this->scorpionface;
}

MainScreen::MainScreen(Ventana* ventana, vector< vector<int> >* perSelect) {
	//atributos pasados como parametro
	this->_ventana = ventana;
	this->_perSelect = perSelect;

	//fuentes
	this-> fontBig = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/8);
	this-> fontSmall = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/25);
	this-> fontMenu = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/15);

	//texturas de texto: news
	title = new TextureHandler( _ventana->_gRenderer );
	titleShadow = new TextureHandler( _ventana->_gRenderer );
	pressStart = new TextureHandler( _ventana->_gRenderer );
	
	modePVP = new TextureHandler( _ventana->_gRenderer );
	modePVE = new TextureHandler( _ventana->_gRenderer );
	modeTraining = new TextureHandler( _ventana->_gRenderer );
	press = new TextureHandler( _ventana->_gRenderer );

	thisIsPVP = new TextureHandler( _ventana->_gRenderer );
	thisIsPVE = new TextureHandler( _ventana->_gRenderer );
	thisIsTraining = new TextureHandler( _ventana->_gRenderer );

	//texturas de texto: loads
	SDL_Color textColor = { 0xCC, 0x00, 0x00, 0xFF };
	this->textColor = textColor;
	SDL_Color shadowColor = { 0x00, 0x00, 0x00, 0xFF };

	this->title->loadFromRenderedText("MORTAL TALLER", textColor, fontBig);
	this->titleShadow->loadFromRenderedText("MORTAL TALLER", shadowColor, fontBig);
	this->pressStart->loadFromRenderedText("press m", textColor, fontSmall);

	this->modePVP->loadFromRenderedText("2 players", textColor, fontMenu);
	this->modePVE->loadFromRenderedText("1 player", textColor, fontMenu);
	this->modeTraining->loadFromRenderedText("training", textColor, fontMenu);
	this->press->loadFromRenderedText("Up, Down and Enter", textColor, fontSmall);

	this->thisIsPVP->loadFromRenderedText("PVP: [b] back, [g] game", textColor, fontSmall);
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

	//Extras
	descriptionY = _ventana->_alto_px*9/10;
	nombreY = _ventana->_alto_px*8/10;

	//Nombres
	nombreP1 = new TextureHandler( _ventana->_gRenderer );
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
	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0x00, 0x44, 0x99, 0xFF );

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

	this->press->render(_ventana->_ancho_px/2 - press->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
}

void MainScreen::showPVP() {
	this->_ventana->clearScreen();
	this->modePVP->render(_ventana->_ancho_px/2 - modePVP->getWidth()/2, _ventana->_alto_px*1/5);
	this->thisIsPVP->render(_ventana->_ancho_px/2 - thisIsPVP->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
}

void MainScreen::showPVE() {
	this->_ventana->clearScreen();
	this->modePVE->render(_ventana->_ancho_px/2 - modePVE->getWidth()/2, _ventana->_alto_px*1/5);
	this->thisIsPVE->render(_ventana->_ancho_px/2 - thisIsPVE->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
}

int MainScreen::viewName(string nombre) {
	nombreP1->loadFromRenderedText(nombre.c_str(), textColor, fontSmall);
	int nombreP1X = _ventana->_ancho_px/2 - nombreP1->getWidth()/2;
	nombreP1->render(nombreP1X, nombreY);
	return nombreP1X;
}

void MainScreen::viewNameBoxFocus(int x) {
	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0x00, 0x00, 0xFF, 0xFF );
	SDL_Rect textBounds = { x-1, nombreY-1, nombreP1->getWidth()+2, nombreP1->getHeight()+2 };
	SDL_RenderDrawRect( _ventana->_gRenderer, &textBounds );
}

void MainScreen::viewNameBoxNoFocus(int x) {
	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0x99, 0x99, 0x99, 0xFF );
	SDL_Rect textBounds = { x-1, nombreY-1, nombreP1->getWidth()+2, nombreP1->getHeight()+2 };
	SDL_RenderDrawRect( _ventana->_gRenderer, &textBounds );
}

void MainScreen::showTraining(int fila, int columna, int textFocus, string nombre) {
	this->_ventana->clearScreen();

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
	int selectedX = topLeftX + columna*faceW;
	int selectedY = topLeftY + fila*faceH;
	SDL_Rect selected1 = { selectedX, selectedY, faceW, faceH };
	SDL_Rect selected2 = { selectedX+1, selectedY+1, faceW-2, faceH-2 };
	SDL_Rect selected3 = { selectedX+2, selectedY+2, faceW-4, faceH-4 };
	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0x00, 0x00, 0xFF, 0xFF );
	SDL_RenderDrawRect( _ventana->_gRenderer, &selected1 );
	SDL_RenderDrawRect( _ventana->_gRenderer, &selected2 );
	SDL_RenderDrawRect( _ventana->_gRenderer, &selected3 );

	if (nombre.length() > 0) {
		if (textFocus == TEXT_NO_FOCUS) viewNameBoxNoFocus(viewName(nombre));
		else if (textFocus == TEXT_FOCUS_P1) viewNameBoxFocus(viewName(nombre));
	} else {
		if (textFocus == TEXT_NO_FOCUS) viewNameBoxNoFocus(viewName(" "));
		else if (textFocus == TEXT_FOCUS_P1) viewNameBoxFocus(viewName(" "));
	}

	this->thisIsTraining->render(_ventana->_ancho_px/2 - thisIsTraining->getWidth()/2, descriptionY);
	this->_ventana->updateScreen();
}