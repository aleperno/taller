#include <Carteles.h>

Carteles::Carteles(Ventana* ventana)
{
	this->_ventana = ventana;
	
	this->_handlerFight	   = new TextureHandler(this->_ventana->_gRenderer);
	this->_handlerFatality = new TextureHandler(this->_ventana->_gRenderer);
	this->_handlerBabality = new TextureHandler(this->_ventana->_gRenderer);
	this->_handlerWinner   = new TextureHandler(this->_ventana->_gRenderer);

	this->_handlerFight->loadFromFile(PATH_FIGHT,false,0,0,0,true);
	this->_handlerFatality->loadFromFile(PATH_FATALITY,false,0,0,0,true);
	this->_handlerBabality->loadFromFile(PATH_BABALITY,false,0,0,0,true);

	this->fontNombres = TTF_OpenFont(PATH_FONT,(this->_ventana->_alto_px/15));
	this->colorNombres.r = 0xFF;
	this->colorNombres.g = 0xFF;
	this->colorNombres.b = 0xFF;
	this->colorNombres.a = 0xFF;
}

Carteles::~Carteles()
{
	delete this->_handlerFight;
	delete this->_handlerFatality;
	delete this->_handlerBabality;
	delete this->_handlerWinner;
}

void Carteles::viewFigth()
{
	int height = 46 * 2;
	int width = 170 * 2;
	int x = (this->_ventana->_ancho_px/2) - (width/2);
	int y = (this->_ventana->_alto_px/3) - (height/2);
	this->_handlerFight->renderScaled(x, y, width, height);
	this->_ventana->updateScreen();
	SDL_Delay(DELAY_SHOWING_FIGHT);
}

void Carteles::viewFatality()
{
	int height = 48 * 3;
	int width = 135 * 3;
	int x = (this->_ventana->_ancho_px/2) - (width/2);
	int y = (this->_ventana->_alto_px/3) - (height/3);
	this->_handlerFatality->renderScaled(x, y, width, height);
	this->_ventana->updateScreen();
	SDL_Delay(DELAY_SHOWING_FATALITY);
}

void Carteles::viewBabality()
{
	int height = 27 * 2;
	int width = 245 * 2;
	int x = (this->_ventana->_ancho_px/2) - (width/2);
	int y = (this->_ventana->_alto_px/3) - (height/2);
	this->_handlerBabality->renderScaled(x, y, width, height);
	this->_ventana->updateScreen();
	SDL_Delay(DELAY_SHOWING_FATALITY);
}

void Carteles::viewWinner(string nombre)
{
	string cartel = nombre + "  wins";
	int ancho = cartel.size() * ((this->_ventana->_alto_px/15)/2);
	this->_handlerWinner->loadFromRenderedText(cartel, this->colorNombres, this->fontNombres);
	int x = (this->_ventana->_ancho_px/2) - ancho/2;
	int y = (this->_ventana->_alto_px/6);
	this->_handlerWinner->render(x, y);
	this->_ventana->updateScreen();
	SDL_Delay(DELAY_SHOWING_FATALITY);
}
