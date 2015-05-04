#include <Arma.h>

Arma::Arma(string path, bool orientacion, float alto_log, float ancho_log, float escala, Ventana* ventana, int zIndex, float pos_y, float pos_incial_x)
{
	this->_handler = new TextureHandler(ventana->_gRenderer);
	this->_handler->loadFromFile(path, false, 0, 0, 0, true);
	this->_orientacion = orientacion;
	this->_alto_log = alto_log;
	this->_ancho_log = ancho_log;
	this-> _factor_escala = escala / this->_ancho_log;
	this->_ancho_px = Arma::getWidth(ventana,_ancho_log);
	this->_alto_px = Arma::getHeight(ventana,_alto_log);
	this->_zIndex = zIndex;
	this->setBoundingBox();
	this->_pos_x = pos_incial_x;
	this->_pos_y = pos_y;
}

void Arma::setBoundingBox()
{
	boundingBox.x = this->get_x_px();
	boundingBox.y = this->get_y_px();
	boundingBox.w = this->_ancho_px;
	boundingBox.h = this->_alto_px;
}

void Arma::viewLanzar()
{
	SDL_Rect rect;
	rect.x = _pos_x;
	rect.y = _pos_y;
	rect.w = _ancho_px;
	rect.h = _alto_px;

	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,&rect);
}

int Arma::get_x_px()
{
	float pos_r = 0; //La posicion en pixeles en reales
	int pos_e = 0; //La posicion en pixeles verdadera (entero)
	float pos_rel = _pos_x - _ventana->_pos_log_x; //Posicion logica relativa a la ventana
	pos_r = ( pos_rel * _ancho_px) / _ancho_log;
	pos_e = (int) pos_r;
	return pos_e;
}

int Arma::get_y_px()
{
	float pos_r = 0; //La posicion en pixeles en reales
	int pos_e = 0; //La posicion en pixeles verdadera (entero)
	pos_r = ( (_ventana->_alto_log - _pos_y) * _ventana->_alto_px) / _ventana->_alto_log;
	pos_e = (int) pos_r;
	pos_e =  pos_e -_alto_px;
	return pos_e;
}

int Arma::getWidth(Ventana* ventana, float ancho_log_capa)
{
	int ancho = 0;
	ancho = (float)(ancho_log_capa * ventana->_ancho_px) / ventana->_ancho_log ;
	return ancho;
}

int Arma::getHeight(Ventana* ventana, float alto_log_capa)
{
	int alto = 0;
	alto = (float)(alto_log_capa * ventana->_alto_px) / ventana->_alto_log ;
	return alto;
}

Arma::~Arma()
{
	delete _handler;
}