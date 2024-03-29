/*
 * HUD.cpp
 *
 *  Created on: May 2, 2015
 *      Author: neo
 */

#include <HUD.h>

using namespace std;

Hud::~Hud()
{
	delete this->hud1.nombreTexture;
	delete this->hud2.nombreTexture;
	delete this->timeTexture;
	delete this->roundTexture;
	delete this->p1winsTexture;
	delete this->p2winsTexture;
	delete this->bufferTexture;
	delete this->finishHimYellow;
	delete this->finishHimRed;

	TTF_CloseFont( fontNombres );
	fontNombres = NULL;
	TTF_CloseFont( fontTimer );
	fontTimer = NULL;
}

Hud::Hud(Ventana* ventana, string* nombreP1, string* nombreP2)
{
	this->_nombreP1 = nombreP1;
	this->_nombreP2 = nombreP2;

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
	this->timeTexture = new TextureHandler( _ventana->_gRenderer );
	this->roundTexture = new TextureHandler( _ventana->_gRenderer );
	this->p1winsTexture = new TextureHandler( _ventana->_gRenderer );
	this->p2winsTexture = new TextureHandler( _ventana->_gRenderer );
	this->bufferTexture = new TextureHandler(_ventana->_gRenderer);

	this->tomasY = _ventana->_alto_px/8;

	this->finishHimRed = new TextureHandler(_ventana->_gRenderer);
	this->finishHimRed->loadFromFile(FINISH_R_PATH,false,0,0,0,true);
	this->finishHimYellow = new TextureHandler(_ventana->_gRenderer);
	this->finishHimYellow->loadFromFile(FINISH_Y_PATH,false,0,0,0,true);

	this->_viewFinishHim = false;
	this->finishH = (_ventana->_alto_px/5) * 3;
	this->finishW = (_ventana->_ancho_px/5) * 3;
	this->finishX = _ventana->_ancho_px/2 - finishW/2;
	this->finishY = (_ventana->_alto_px/3) - (this->finishH/2); ;// _ventana->_alto_px/3;
	this->finishCount = 0;
}

void Hud::activateFinishHim()
{
	this->_viewFinishHim = true;
}

void Hud::setearPersonajes(Personaje* personaje1, Personaje* personaje2) {
	this->_personaje1 = personaje1;
	this->_personaje2 = personaje2;
}

void Hud::recargarNombres() {
	this->hud1.nombreTexture->loadFromRenderedText(*_nombreP1, colorNombres, fontNombres);
	this->hud2.nombreTexture->loadFromRenderedText(*_nombreP2, colorNombres, fontNombres);
}

void Hud::actualizarRounds(int round, int p1wins, int p2wins) {
	this->roundTexture->loadFromRenderedText("round " + StringUtil::int2string(round), colorVida, fontTimer);
	if (p1wins == 1)
		this->p1winsTexture->loadFromRenderedText("I", colorVida, fontTimer);
	else if (p1wins == 0)
		this->p1winsTexture->free();
	if (p2wins == 1)
		this->p2winsTexture->loadFromRenderedText("I", colorVida, fontTimer);
	else if (p2wins == 0)
		this->p2winsTexture->free();
}

void Hud::actualizarHealthbars() {
	this->hud1.health.w = this->hud1.interno.w * _personaje1->healthPoints / HEALTH;
	this->hud1.healthIlum.w = this->hud1.interno.w * _personaje1->healthPoints / HEALTH;

	this->hud2.health.w = this->hud2.interno.w * _personaje2->healthPoints / HEALTH;
	this->hud2.healthIlum.w = this->hud2.interno.w * _personaje2->healthPoints / HEALTH;
	this->hud2.health.x = _ventana->_ancho_px - this->hud2.interno.h/2 - this->hud2.health.w;
	this->hud2.healthIlum.x = _ventana->_ancho_px - this->hud2.interno.h/2 - this->hud2.health.w;
}

void Hud::showFinishHim() {

	if (this->finishCount < 0) this->finishCount = 0;
	bool flag = (this->finishCount%2==0)?true:false;

	if (flag) {
		//cout << "Muestro finishHim rojo" << endl;
		this->finishHimRed->renderScaled(finishX, finishY, finishW, finishH);
		this->finishCount++;
	} else if (!flag) {
		//cout << "Muestro finishHim amarillo" << endl;
		this->finishHimYellow->renderScaled(finishX, finishY, finishW, finishH);
		this->finishCount++;
	} 
	if (finishCount > FINISH_CYCLES) {
		this->finishCount = 0;
		this->_viewFinishHim = false;
	}
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

	if ( this->_viewFinishHim )
	{
		this->showFinishHim();
	}
}

void Hud::printHUD(int time) {
	printHUD();

	this->p1winsTexture->render(hud1.externo.x, this->hud1.externo.y + this->hud1.externo.h);
	this->p2winsTexture->render(hud2.externo.x + hud2.externo.w - this->p2winsTexture->getWidth(), this->hud2.externo.y + this->hud2.externo.h);

	this->timeTexture->loadFromRenderedText(StringUtil::int2string(time), colorVida, fontTimer);
	this->timeTexture->render(_ventana->_ancho_px/2 - this->timeTexture->getWidth()/2, this->hud1.interno.y);

	this->roundTexture->render(_ventana->_ancho_px/2 - this->roundTexture->getWidth()/2, this->hud1.interno.y + this->timeTexture->getHeight());
}

void Hud::printHUD(vector<string>* bufferTeclas, bool hayCombo, vector<string>* combo, string nombreCombo) {
	this->printHUD();
	string cadena;
	cadena = "";
	if (hayCombo) {
			for (unsigned int i=0;i<combo->size();++i) {
				cadena += combo->at(i);
				cadena += " ";
			}
			cadena += nombreCombo;
			this->bufferTexture->loadFromRenderedText(cadena,colorVida,fontTimer);
			this->bufferTexture->render(this->hud1.interno.x,tomasY);
	} else {
		if (!bufferTeclas->empty()) {
			for (unsigned int i=0;i<bufferTeclas->size();++i) {
				cadena += bufferTeclas->at(i);
				cadena += " ";
			}
			this->bufferTexture->loadFromRenderedText(cadena,colorNombres,fontNombres);
			this->bufferTexture->render(this->hud1.interno.x,tomasY);
		}
	}
}
