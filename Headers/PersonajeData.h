/*
 * Personaje.h
 *
 *  Created on: Mar 28, 2015
 *      Author: neo
 */

#ifndef HEADERS_PERSONAJEDATA_H_
#define HEADERS_PERSONAJEDATA_H_

#include <string>
#include <JugadorData.h>
#include <CaracterData.h>

using namespace std;

class PersonajeData
{
	public:
		void llenarDatos(JugadorData* jugador, CaracterData* caracter);

		float ancho;
		float alto;
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

		vector<int> cantSprites;
		vector<int> anchoSprites;
		vector<int> velSprites;

		float arma_speed;

		string imgPath;
		
		int cantSpritesBabyStance;
		int cantSpritesBaby;
		int cantSpritesSkeletonStance;
		int anchoSpriteFatalities;
		int altoSpriteFatalities;
		string imgFatalities;

		int cantSpritesSkeleton;
		int anchoSpriteSkeleton;
		int altoSpriteSkeleton;
		string imgSkeleton;

		vector<string> toma1;
		vector<string> toma2;
		vector<string> fatality;

		int tomasTiempoLimite;
		int tomasTolerancia;

		int getGB() { return this->golpe_bajo;}
		int getGA() { return this->golpe_alto;}
		int getPB() { return this->patada_baja;}
		int getPA() { return this->patada_alta;}
		int getBL() { return this->defensa;}
		int getAR() { return this->arrojar;}

};

#endif /* HEADERS_PERSONAJEDATA_H_ */
