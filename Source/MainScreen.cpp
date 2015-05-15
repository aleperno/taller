#include "MainScreen.h"

MainScreen::~MainScreen()
{
	TTF_CloseFont( fontBig );
	fontBig = NULL;
	TTF_CloseFont( fontSmall );
	fontSmall = NULL;

	delete this->title;
	delete this->pressStart;
}

MainScreen::MainScreen(Ventana* ventana) {
	this->_ventana = ventana;
	this-> fontBig = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/5);
	this-> fontSmall = TTF_OpenFont(FONT_PATH, _ventana->_alto_px/20);

	title = new TextureHandler( _ventana->_gRenderer );
	pressStart = new TextureHandler( _ventana->_gRenderer );

	SDL_Color colorTitle = { 0xCC, 0x00, 0x00, 0xFF };
	gateSpeed = 80;
	titleSpeed = 140;
	pressStartSpeed = 140;

	this->title->loadFromRenderedText("JOAKIN PUTO", colorTitle, fontBig);
	this->pressStart->loadFromRenderedText("insert coin (m)", colorTitle, fontSmall);

	titleX = _ventana->_ancho_px/2 - title->getWidth()/2;
	titleY = -title->getHeight();
	pressStartX = _ventana->_ancho_px/2 - pressStart->getWidth()/2;
	pressStartY = _ventana->_alto_px;

	int gateAncho = _ventana->_ancho_px/2;
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
	if (gateLeft.x < 0) {
		gateLeft.x = gateLeft.x + gateLeft.w/gateSpeed;
		gateRight.x = gateRight.x - gateRight.w/gateSpeed;
	}

	if (titleY < _ventana->_alto_px/5)
		titleY = titleY + _ventana->_alto_px/titleSpeed;

	if (pressStartY > _ventana->_alto_px/2)
		pressStartY = pressStartY - _ventana->_alto_px/pressStartSpeed;
}

void MainScreen::showIntro() {
	actualizarPosiciones();

	this->_ventana->clearScreen();

	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0x77, 0x77, 0x77, 0xFF );
	SDL_RenderFillRect( _ventana->_gRenderer, &gateLeft );
	SDL_RenderFillRect( _ventana->_gRenderer, &gateRight );

	this->title->render(titleX, titleY);
	this->pressStart->render(pressStartX, pressStartY);

	this->_ventana->updateScreen();

}