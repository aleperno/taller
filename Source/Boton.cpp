#include "Boton.h"

Boton::Boton(Ventana* _ventana, TextureHandler* texture, int ejeCentralX, int y) {
	this->_ventana = _ventana;
	this->texture = texture;
	this->ejeCentralX = ejeCentralX;
	this->y = y;
	this->x = ejeCentralX - this->texture->getWidth()/2;
}

Boton::~Boton() {
	delete this->texture;
}

void Boton::view() {
	this->x = ejeCentralX - this->texture->getWidth()/2;
	this->texture->render(x, y);
}

void Boton::viewExternBox(SDL_Color* c) {
	SDL_Rect externBox = { x-1, y-1, texture->getWidth()+2, texture->getHeight()+2 };
	SDL_SetRenderDrawColor( _ventana->_gRenderer, c->r, c->g, c->b, c->a );
	SDL_RenderDrawRect( _ventana->_gRenderer, &externBox );
}

void Boton::viewHighlight(SDL_Color* c) {
	SDL_Rect highlight = { x, y, texture->getWidth(), texture->getHeight() };
	SDL_SetRenderDrawColor( _ventana->_gRenderer, c->r, c->g, c->b, c->a );
	SDL_RenderFillRect( _ventana->_gRenderer, &highlight );
}

bool Boton::mouseOver() {
	int x,y;
	SDL_GetMouseState(&x, &y);
	return ((x >= this->x) && (x <= this->x + this->texture->getWidth()) && (y >= this->y) && (y <= this->y + this->texture->getHeight()));
}