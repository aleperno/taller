#include <PersonajeData.h>
#include <iostream>

void PersonajeData::llenarDatos(JugadorData* jugador, CaracterData* caracter) {
	this->nombre = caracter->nombre;

	this->ancho = jugador->ancho;
	this->alto = jugador->alto;
	this->arma_speed = jugador->arma_speed;

	//joyconfig
	this->golpe_alto = jugador->golpe_alto;
	this->golpe_bajo = jugador->golpe_bajo;
	this->patada_alta = jugador->patada_alta;
	this->patada_baja = jugador->patada_baja;
	this->defensa = jugador->defensa;
	this->arrojar = jugador->arrojar;

	//sprite data
	this->height = caracter->height;
	this->width = caracter->width;
	this->size = caracter->size;
	this->cantSprites = caracter->cantSprites;
	this->anchoSprites = caracter->anchoSprites;
	this->velSprites = caracter->velSprites;
	this->imgPath = caracter->imgPath;

	this->cantSpritesBabyStance		= caracter->cantSpritesBabyStance;
	this->cantSpritesBaby			= caracter->cantSpritesBaby;
	this->cantSpritesSkeletonStance = caracter->cantSpritesSkeletonStance;
	this->anchoSpriteFatalities		= caracter->anchoSpriteFatalities;
	this->altoSpriteFatalities		= caracter->altoSpriteFatalities;
	this->imgFatalities				= caracter->imgFatalities;

	this->cantSpritesSkeleton = caracter->cantSpritesSkeleton;
	this->anchoSpriteSkeleton = caracter->anchoSpriteSkeleton;
	this->altoSpriteSkeleton  = caracter->altoSpriteSkeleton;
	this->imgSkeleton		  = caracter->imgSkeleton;

	//color alternativo
	this->h_inicial = caracter->h_inicial;
	this->h_final = caracter->h_final;
	this->desplazamiento = caracter->desplazamiento;

	//tomas
	this->toma1 = caracter->toma1;
	this->toma2 = caracter->toma2;
	this->fatality = caracter->fatality;
	this->tomasTiempoLimite = jugador->tomasTiempoLimite;
	this->tomasTolerancia = jugador->tomasTolerancia;
}