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
}

MainScreen::MainScreen(Ventana* ventana) {
	this->_ventana = ventana;
	this-> fontBig = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/8);
	this-> fontSmall = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/20);
	this-> fontMenu = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/15);

	title = new TextureHandler( _ventana->_gRenderer );
	pressStart = new TextureHandler( _ventana->_gRenderer );
	titleShadow = new TextureHandler( _ventana->_gRenderer );
	modePVP = new TextureHandler( _ventana->_gRenderer );
	modePVE = new TextureHandler( _ventana->_gRenderer );
	modeTraining = new TextureHandler( _ventana->_gRenderer );

	shakeCount = SHAKE_COUNT;
	randomX = 0;
	randomY = 0;

	SDL_Color colorTitle = { 0xCC, 0x00, 0x00, 0xFF };
	this->title->loadFromRenderedText("MORTAL TALLER", colorTitle, fontBig);
	this->pressStart->loadFromRenderedText("press m", colorTitle, fontSmall);
	this->modePVP->loadFromRenderedText("2 players", colorTitle, fontMenu);
	this->modePVE->loadFromRenderedText("1 player", colorTitle, fontMenu);
	this->modeTraining->loadFromRenderedText("training", colorTitle, fontMenu);

	SDL_Color colorTitleShadow = { 0x00, 0x00, 0x00, 0xFF };
	this->titleShadow->loadFromRenderedText("MORTAL TALLER", colorTitleShadow, fontBig);

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

void MainScreen::showModeSelect() {
	this->_ventana->clearScreen();
	this->modePVP->render(_ventana->_ancho_px/2 - modePVP->getWidth()/2, _ventana->_alto_px*1/5);
	this->modePVE->render(_ventana->_ancho_px/2 - modePVE->getWidth()/2, _ventana->_alto_px*2/5);
	this->modeTraining->render(_ventana->_ancho_px/2 - modeTraining->getWidth()/2, _ventana->_alto_px*3/5);

	this->pressStart->render(pressStartX, pressStartY);
	this->_ventana->updateScreen();
}