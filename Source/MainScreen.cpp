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
}

MainScreen::MainScreen(Ventana* ventana, vector< vector<int> >* perSelect) {
	//atributos pasados como parametro
	this->_ventana = ventana;
	this->_perSelect = perSelect;

	//fuentes
	this-> fontBig = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/8);
	this-> fontSmall = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/20);
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
	this->thisIsTraining->loadFromRenderedText("Training: [b] back, [g] game", textColor, fontSmall);
	
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

	this->press->render(_ventana->_ancho_px/2 - press->getWidth()/2, pressStartY);
	this->_ventana->updateScreen();
}

void MainScreen::showPVP() {
	this->_ventana->clearScreen();
	this->modePVP->render(_ventana->_ancho_px/2 - modePVP->getWidth()/2, _ventana->_alto_px*1/5);
	this->thisIsPVP->render(_ventana->_ancho_px/2 - thisIsPVP->getWidth()/2, pressStartY);
	this->_ventana->updateScreen();
}

void MainScreen::showPVE() {
	this->_ventana->clearScreen();
	this->modePVE->render(_ventana->_ancho_px/2 - modePVE->getWidth()/2, _ventana->_alto_px*1/5);
	this->thisIsPVE->render(_ventana->_ancho_px/2 - thisIsPVE->getWidth()/2, pressStartY);
	this->_ventana->updateScreen();
}

void MainScreen::showTraining(int fila, int columna) {
	this->_ventana->clearScreen();
	this->modeTraining->render(_ventana->_ancho_px/2 - modeTraining->getWidth()/2, _ventana->_alto_px*1/5);
	this->thisIsTraining->render(_ventana->_ancho_px/2 - thisIsTraining->getWidth()/2, pressStartY);
	this->_ventana->updateScreen();
}