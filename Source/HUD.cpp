/*
 * HUD.cpp
 *
 *  Created on: May 2, 2015
 *      Author: neo
 */

#include <HUD.h>

Hud::~Hud()
{
	delete this->hud1.nombreTexture;
	delete this->hud2.nombreTexture;
	TTF_CloseFont( font );
	font = NULL;
	TTF_Quit();
}

Hud::Hud(Ventana* ventana, Personaje* personaje1, Personaje* personaje2)
{
	this-> _personaje1 = personaje1;
	this-> _personaje2 = personaje2;
	this-> _ventana = ventana;

	int hudExternH = _ventana->_alto_px/15;
	int hudExternW = _ventana->_ancho_px*2/5;
	int hudInternH = _ventana->_alto_px/20;
	int hudInternW = _ventana->_ancho_px*20/53;

	this->hud1.externo.h = hudExternH;
	this->hud1.externo.w = hudExternW;
	this->hud1.externo.x = 0;
	this->hud1.externo.y = 0;

	this->hud2.externo.h = hudExternH;
	this->hud2.externo.w = hudExternW;
	this->hud2.externo.x = _ventana->_ancho_px - hudExternW;
	this->hud2.externo.y = 0;

	this->hud1.interno.h = hudInternH;
	this->hud1.interno.w = hudInternW;
	this->hud1.interno.x = (hudExternW-hudInternW)/2;
	this->hud1.interno.y = (hudExternH-hudInternH)/2;

	this->hud2.interno.h = hudInternH;
	this->hud2.interno.w = hudInternW;
	this->hud2.interno.x = _ventana->_ancho_px - (hudExternW-hudInternW)/2 - hudInternW;
	this->hud2.interno.y = (hudExternH-hudInternH)/2;

	this->hud1.externoIlum.h = hudExternH/2;
	this->hud1.externoIlum.w = hudExternW;
	this->hud1.externoIlum.x = 0;
	this->hud1.externoIlum.y = 0;

	this->hud1.healthIlum.h = hudExternH/2 - (hudExternH-hudInternH)/2;
	this->hud1.healthIlum.w = hudInternW;
	this->hud1.healthIlum.x = this->hud1.interno.x;
	this->hud1.healthIlum.y = this->hud1.interno.y;

	this->hud2.externoIlum.h = hudExternH/2;
	this->hud2.externoIlum.w = hudExternW;
	this->hud2.externoIlum.x = this->hud2.externo.x;
	this->hud2.externoIlum.y = 0;

	this->hud2.healthIlum.h = hudExternH/2 - (hudExternH-hudInternH)/2;
	this->hud2.healthIlum.w = hudInternW;
	this->hud2.healthIlum.x = this->hud2.interno.x;
	this->hud2.healthIlum.y = this->hud2.interno.y;

	this->hud1.health = this->hud1.interno;
	this->hud2.health = this->hud2.interno;

	TTF_Init();

	this-> font = TTF_OpenFont(FONT_PATH,hudExternH);
	SDL_Color textColor = { 0xCC, 0, 0 };
	this->hud1.nombreTexture = new TextureHandler( _ventana->_gRenderer );
	this->hud2.nombreTexture = new TextureHandler( _ventana->_gRenderer );
	this->hud1.nombreTexture->loadFromRenderedText(_personaje1->_personajeData.nombre, textColor, font);
	this->hud2.nombreTexture->loadFromRenderedText(_personaje2->_personajeData.nombre, textColor, font);

	this->hud1.nombre.h = this->hud1.nombreTexture->getHeight();
	this->hud1.nombre.w = this->hud1.nombreTexture->getWidth();
	this->hud1.nombre.x = 0;
	this->hud1.nombre.y = hudExternH;

	this->hud2.nombre.h = this->hud2.nombreTexture->getHeight();
	this->hud2.nombre.w = this->hud2.nombreTexture->getWidth();
	this->hud2.nombre.x = _ventana->_ancho_px - this->hud2.nombreTexture->getWidth();
	this->hud2.nombre.y = hudExternH;
}

void Hud::actualizarHealthbars() {
	this->hud1.health.w = this->hud1.interno.w * _personaje1->healthPoints / 100;
	this->hud1.healthIlum.w = this->hud1.interno.w * _personaje1->healthPoints / 100;

	this->hud2.health.w = this->hud2.interno.w * _personaje2->healthPoints / 100;
	this->hud2.healthIlum.w = this->hud2.interno.w * _personaje2->healthPoints / 100;
	this->hud2.health.x = _ventana->_ancho_px - (this->hud2.externo.w-this->hud2.interno.w)/2 - this->hud2.health.w;
	this->hud2.healthIlum.x = _ventana->_ancho_px - (this->hud2.externo.w-this->hud2.interno.w)/2 - this->hud2.health.w;
}


void Hud::printHUD() {

	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0x99, 0x99, 0x99, 0xFF );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.externo) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.externo) );

	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0xBB, 0xBB, 0xBB, 0xFF );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.externoIlum) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.externoIlum) );

	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0x00, 0x00, 0x00, 0xFF );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.interno) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.interno) );

	this->actualizarHealthbars();

	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0xCC, 0x00, 0x00, 0xFF );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.health) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.health) );

	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0xFF, 0x00, 0x00, 0xFF );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.healthIlum) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.healthIlum) );

	SDL_SetRenderDrawColor( _ventana->_gRenderer, 0x99, 0x99, 0x99, 0xFF );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.nombre) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.nombre) );

	this->hud1.nombreTexture->render(0,this->hud1.externo.h);
	this->hud2.nombreTexture->render(_ventana->_ancho_px - this->hud2.nombreTexture->getWidth() ,this->hud2.externo.h);
}
