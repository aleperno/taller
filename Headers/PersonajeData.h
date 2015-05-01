/*
 * Personaje.h
 *
 *  Created on: Mar 28, 2015
 *      Author: neo
 */

#ifndef HEADERS_PERSONAJE_H_
#define HEADERS_PERSONAJE_H_

using namespace std;

class PersonajeData
{
	public:
		float ancho;
		float alto;
		bool orientacion;
		string nombre;

		//color alternativo
		float h_inicial;
		float h_final;
		float desplazamiento;

		//joyconfig
		int golpe_alto;
		int golpe_bajo;
		int patada_alta;
		int patada_baja;
		int defensa;
		int arrojar;

		//sprite data
		int height;
		int width;
		int size;
		int walk[2];
		int idle[2];
		int jumpUp[2];
		int jumpFwd[2];
		int jumpBwd[2];
		int duck[2];

		string imgPath;
};




#endif /* HEADERS_PERSONAJE_H_ */
