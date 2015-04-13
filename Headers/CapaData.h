/*
 * Capa.h
 *
 *  Created on: Mar 28, 2015
 *      Author: neo
 */
#ifndef HEADERS_CAPA_H_
#define HEADERS_CAPA_H_
#include <string>
using namespace std;
#include <Logger.h>

class CapaData
{
	public:
		string imagen_fondo;
		float ancho;
		bool animado;
		int height_px;
		int sprites;
		int animDelay;
};



#endif /* HEADERS_CAPA_H_ */
