#include <Carteles.h>

Carteles::Carteles(Ventana* ventana)
{
	this->_ventana = ventana;
	
	this->_handlerFight	   = new TextureHandler(this->_ventana->_gRenderer);
	this->_handlerFatality = new TextureHandler(this->_ventana->_gRenderer);
	this->_handlerBabality = new TextureHandler(this->_ventana->_gRenderer);

	this->_handlerFight->loadFromFile(PATH_FIGHT,false,0,0,0,true);
	this->_handlerFatality->loadFromFile(PATH_FATALITY,false,0,0,0,true);
	this->_handlerBabality->loadFromFile(PATH_BABALITY,false,0,0,0,true);
	
}

Carteles::~Carteles()
{
	delete this->_handlerFight;
	delete this->_handlerFatality;
	delete this->_handlerBabality;
}

void Carteles::viewFigth()
{
	int height = 46 * 2;
	int width = 170 * 2;
	int x = (this->_ventana->_ancho_px/2) - (width/2);
	int y = (this->_ventana->_alto_px/3) - (height/2);
	this->_handlerFight->renderScaled(x, y, width, height);
	this->_ventana->updateScreen();
}

void Carteles::viewFatality()
{
	int height = 48 * 2;
	int width = 135 * 2;
	int x = (this->_ventana->_ancho_px/2) - (width/2);
	int y = (this->_ventana->_alto_px/3) - (height/2);
	this->_handlerFatality->renderScaled(x, y, width, height);
	this->_ventana->updateScreen();
}

void Carteles::viewBabality()
{
	int height = 27 * 2;
	int width = 245 * 2;
	int x = (this->_ventana->_ancho_px/2) - (width/2);
	int y = (this->_ventana->_alto_px/3) - (height/2);
	this->_handlerBabality->renderScaled(x, y, width, height);
	this->_ventana->updateScreen();
}
