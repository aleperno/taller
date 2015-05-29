#include "Boton.h"

Boton::Boton(Ventana* _ventana, TextureHandler* textTexture, TextureHandler* surfaceTexture, int ejeCentralX, int y, int w, int h) {
	this->_ventana = _ventana;
	this->textTexture = textTexture;
	this->surfaceTexture = surfaceTexture;
	this->y = y;
	this->x = ejeCentralX - w/2;
	this->w = w;
	this->h = h;
}

Boton::Boton(Ventana* _ventana, TextureHandler* surfaceTexture, int ejeCentralX, int y, int w, int h) {
	this->_ventana = _ventana;
	this->surfaceTexture = surfaceTexture;
	this->y = y;
	this->x = ejeCentralX - w/2;
	this->w = w;
	this->h = h;
	this->textTexture = NULL;
}

Boton::~Boton() {
	delete this->textTexture;
}

void Boton::view() {
	this->surfaceTexture->renderScaled(x,y,w,h);
	if (textTexture != NULL)
		this->textTexture->render( x + (w - textTexture->getWidth())/2, y + (h - textTexture->getHeight())/2);
}

bool Boton::mouseOver() {
	int x,y;
	SDL_GetMouseState(&x, &y);
	return ((x >= this->x) && (x <= this->x + w) && (y >= this->y) && (y <= this->y + h));
}