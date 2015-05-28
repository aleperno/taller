#include "Boton.h"

Boton::Boton(Ventana* _ventana, TextureHandler* texture, int ejeCentralX, int y) {
	this->_ventana = _ventana;
	this->texture = texture;
	this->ejeCentralX = ejeCentralX;
	this->y = y;
	this->x = ejeCentralX - this->texture->getWidth()/2;
}

Boton::Boton(Ventana* _ventana, int w, int h, int ejeCentralX, int y) {
	this->_ventana = _ventana;
	this->ejeCentralX = ejeCentralX;
	this->y = y;
	this->x = ejeCentralX - w/2;
	this->w = w;
	this->h = h;
	this->texture = NULL;
}

Boton::~Boton() {
	delete this->texture;
}

void Boton::view() {
	if (this->texture != NULL) {
		this->x = ejeCentralX - this->texture->getWidth()/2;
		this->texture->render(x, y);
	}
}

void Boton::viewExternBox(SDL_Color* c) {
	SDL_Rect externBox;
	if (this->texture != NULL) {
		SDL_Rect box = { x-1, y-1, texture->getWidth()+2, texture->getHeight()+2 };
		externBox = box;
	} else {
		SDL_Rect box = { x-1, y-1, w+2, h+2 };
		externBox = box;
	}
	SDL_SetRenderDrawColor( _ventana->_gRenderer, c->r, c->g, c->b, c->a );
	SDL_RenderDrawRect( _ventana->_gRenderer, &externBox );
}

void Boton::viewHighlight(SDL_Color* c) {
	SDL_Rect highlight;
	if (this->texture != NULL) {
		SDL_Rect box = { x, y, texture->getWidth(), texture->getHeight() };
		highlight = box;
	} else {
		SDL_Rect box = { x, y, w, h };
		highlight = box;
	}
	SDL_SetRenderDrawColor( _ventana->_gRenderer, c->r, c->g, c->b, c->a );
	SDL_RenderFillRect( _ventana->_gRenderer, &highlight );
}

bool Boton::mouseOver() {
	int x,y;
	SDL_GetMouseState(&x, &y);
	if (this->texture != NULL)
		return ((x >= this->x) && (x <= this->x + this->texture->getWidth()) && (y >= this->y) && (y <= this->y + this->texture->getHeight()));
	else return ((x >= this->x) && (x <= this->x + this->w) && (y >= this->y) && (y <= this->y + this->h));
}