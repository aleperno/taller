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
	delete this->hudTiempo.timeTexture;
	TTF_CloseFont( fontNombres );
	fontNombres = NULL;
	TTF_CloseFont( fontTimer );
	fontTimer = NULL;
}

Hud::Hud(Ventana* ventana)
{
	this->colorExterno.r = 0x99;
	this->colorExterno.g = 0x99;
	this->colorExterno.b = 0x99;
	this->colorExterno.a = 0xFF;

	this->colorInterno.r = 0x00;
	this->colorInterno.g = 0x00;
	this->colorInterno.b = 0x00;
	this->colorInterno.a = 0xFF;

	this->colorVida.r = 0xCC;
	this->colorVida.g = 0x00;
	this->colorVida.b = 0x00;
	this->colorVida.a = 0xFF;
	
	this->colorVidaIlum.r = 0xFF;
	this->colorVidaIlum.g = 0x00;
	this->colorVidaIlum.b = 0x00;
	this->colorVidaIlum.a = 0xFF;

	this->colorNombres.r = 0xFF;
	this->colorNombres.g = 0xFF;
	this->colorNombres.b = 0xFF;
	this->colorNombres.a = 0xFF;

	this->colorTimer = colorVida;

	this->_ventana = ventana;

	int hudInternoH = _ventana->_alto_px/25;
	int hudInternoW = _ventana->_ancho_px/3;

	this->hud1.interno.h = hudInternoH;
	this->hud1.interno.w = hudInternoW;
	this->hud1.interno.x = hudInternoH/2;
	this->hud1.interno.y = hudInternoH/2;

	this->hud2.interno.h = hudInternoH;
	this->hud2.interno.w = hudInternoW;
	this->hud2.interno.x = _ventana->_ancho_px - hudInternoH/2 - hudInternoW;
	this->hud2.interno.y = hudInternoH/2;

	this->hud1.externo.h = hudInternoH + 4;
	this->hud1.externo.w = hudInternoW + 4;
	this->hud1.externo.x = this->hud1.interno.x - 2;
	this->hud1.externo.y = this->hud1.interno.y - 2;

	this->hud2.externo.h = hudInternoH + 4;
	this->hud2.externo.w = hudInternoW + 4;
	this->hud2.externo.x = this->hud2.interno.x - 2;
	this->hud2.externo.y = this->hud2.interno.y - 2;

	this->hud1.healthIlum.h = hudInternoH/2;
	this->hud1.healthIlum.w = hudInternoW;
	this->hud1.healthIlum.x = this->hud1.interno.x;
	this->hud1.healthIlum.y = this->hud1.interno.y;

	this->hud2.healthIlum.h = hudInternoH/2;
	this->hud2.healthIlum.w = hudInternoW;
	this->hud2.healthIlum.x = this->hud2.interno.x;
	this->hud2.healthIlum.y = this->hud2.interno.y;

	this->hud1.health = this->hud1.interno;
	this->hud2.health = this->hud2.interno;

	this->fontNombres = TTF_OpenFont(FONT_PATH,hudInternoH*4/5);
	this->fontTimer = TTF_OpenFont(FONT_PATH,hudInternoH);
	this->hud1.nombreTexture = new TextureHandler( _ventana->_gRenderer );
	this->hud2.nombreTexture = new TextureHandler( _ventana->_gRenderer );

}

void Hud::setearPersonajes(Personaje* personaje1, Personaje* personaje2) {
	this->_personaje1 = personaje1;
	this->_personaje2 = personaje2;
		
	this->hudTiempo.timeTexture = new TextureHandler( _ventana->_gRenderer );
	this->hud1.nombreTexture->loadFromRenderedText(_personaje1->_personajeData.nombre, colorNombres, fontNombres);
	this->hud2.nombreTexture->loadFromRenderedText(_personaje2->_personajeData.nombre, colorNombres, fontNombres);
}


void Hud::actualizarHealthbars() {
	this->hud1.health.w = this->hud1.interno.w * _personaje1->healthPoints / 100;
	this->hud1.healthIlum.w = this->hud1.interno.w * _personaje1->healthPoints / 100;

	this->hud2.health.w = this->hud2.interno.w * _personaje2->healthPoints / 100;
	this->hud2.healthIlum.w = this->hud2.interno.w * _personaje2->healthPoints / 100;
	this->hud2.health.x = _ventana->_ancho_px - this->hud2.interno.h/2 - this->hud2.health.w;
	this->hud2.healthIlum.x = _ventana->_ancho_px - this->hud2.interno.h/2 - this->hud2.health.w;
}

void Hud::printHUD() {

	SDL_SetRenderDrawColor( _ventana->_gRenderer, colorExterno.r, colorExterno.g, colorExterno.b, colorExterno.a );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.externo) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.externo) );

	SDL_SetRenderDrawColor( _ventana->_gRenderer, colorInterno.r, colorInterno.g, colorInterno.b, colorInterno.a );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.interno) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.interno) );

	this->actualizarHealthbars();

	SDL_SetRenderDrawColor( _ventana->_gRenderer, colorVida.r, colorVida.g, colorVida.b, colorVida.a );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.health) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.health) );

	SDL_SetRenderDrawColor( _ventana->_gRenderer, colorVidaIlum.r, colorVidaIlum.g, colorVidaIlum.b, colorVidaIlum.a );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud1.healthIlum) );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hud2.healthIlum) );
	
	this->hud1.nombreTexture->render(this->hud1.interno.x, this->hud1.interno.y);
	this->hud2.nombreTexture->render(_ventana->_ancho_px - this->hud2.nombreTexture->getWidth() - this->hud2.interno.h/2, this->hud2.interno.y);

}

void Hud::printHUD(int time) {
	printHUD();

	/*SDL_SetRenderDrawColor( _ventana->_gRenderer, colorExterno.r, colorExterno.g, colorExterno.b, colorExterno.a );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hudTiempo.externo) );
	SDL_SetRenderDrawColor( _ventana->_gRenderer, colorExternoIlum.r, colorExternoIlum.g, colorExternoIlum.b, colorExternoIlum.a );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hudTiempo.externoIlum) );
	SDL_SetRenderDrawColor( _ventana->_gRenderer, colorInterno.r, colorInterno.g, colorInterno.b, colorInterno.a );
	SDL_RenderFillRect( _ventana->_gRenderer, &(this->hudTiempo.interno) );*/

	this->hudTiempo.timeTexture->loadFromRenderedText(StringUtil::int2string(time), colorTimer, fontTimer);
	//this->hudTiempo.timeTexture->render(_ventana->_ancho_px/2 - this->hudTiempo.timeTexture->getWidth()/2, this->hudTiempo.interno.y);
	this->hudTiempo.timeTexture->render(_ventana->_ancho_px/2 - this->hudTiempo.timeTexture->getWidth()/2, this->hud1.interno.y);
}