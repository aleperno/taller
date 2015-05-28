#include "AI.h"
using namespace std;

#define JMP_FACTOR 2

#define IDLE "ID"
#define UP_LEFT "UL"
#define UP_RIGHT "UR"
#define UP "UP"
#define UP_KICK "UK"
#define UP_PUNCH "UH"
#define DUCK "DW"
#define DUCK_BLOCK "DB"
#define DUCK_KICK "DK"
#define DUCK_PUNCH "DP"
#define BARRE "SW"
#define LEFT "LF"
#define RIGHT "RG"
#define FIRE "FI"
#define BLOCK "BL"
#define HIGH_PUNCH "GA"
#define LOW_PUNCH "GB"
#define HIGH_KICK "PA"
#define LOW_KICK "PB"
#define WALK "WK"
#define D_CLOSE 0
#define D_MED 1
#define D_FAR 2
#define TRACK_MOV 10

void AI::HandlePlayer(Personaje* _personaje_AI, Personaje* _personaje_PLAYER)
{
	if(_personaje_PLAYER->track_movimientos.size() >= TRACK_MOV)
	{
		this->prediccion = this->ObtenerPrediccion(_personaje_PLAYER->track_movimientos);
		this->accion = this->ObtenerAccion(this->prediccion, this->CalcularDistacia(_personaje_AI->_pos_x, _personaje_PLAYER->_pos_x, _personaje_AI->_ancho_log));
		//this->EjecutarMovimiento(_personaje_AI, this->accion);
	}
	else
	{
		this->EjecutarMovimiento(_personaje_AI, FIRE);
	}
}

AI::AI(int _difficulty)
{
	CrearTablasLookUp(_difficulty);
}

AI::~AI()
{
	delete &LookUpTable;
}

int AI::CalcularDistacia(float _pos_x_ai, float _pos_x_player, int _ancho_log)
{
	float dist = fabs(_pos_x_ai - _pos_x_player);
	int dist_entera = (int)dist;

	if(dist_entera < _ancho_log)
		return 0;

	if((dist_entera > _ancho_log) && (dist_entera < _ancho_log * 2))
		return 1;

	if(dist_entera > _ancho_log * 2)
		return 2;

	return 0;
}

void AI::EjecutarMovimiento(Personaje* _personaje, string _movimiento)
{
	//TODO: Ejecutar el movimiento
	/*Pruebas
	_personaje->jump(JMP_FACTOR);
	_personaje->lanzarArma();*/
}

string AI::ObtenerAccion(string _prediccion, int _distancia)
{
	for (vector<RegistroLookUp>::iterator it = this->LookUpTable.begin(); it != this->LookUpTable.end(); ++it)
	{
		if(it->prediccion == _prediccion && it->distancia == _distancia)
			return it->accion;
	}
	return BLOCK;
}

string AI::ObtenerPrediccion(vector<string> _movimientos)
{
	string ult_mov = _movimientos[_movimientos.size() - 1];
	string ant_ult_mov = _movimientos[_movimientos.size() - 2];
	vector<string> posibles;
	vector<Probabilidad> probables;
	int cuenta_apariciones = 0;
	bool ya_esta = false;
	float mayor_proba = 0;
	string accion_mas_probable = BLOCK;

	//Armo el vector con las acciones candidatas segun los ultimos movimientos
	for(int i = 0; i < _movimientos.size() - 2; i++)
	{
		if(_movimientos[i] == ant_ult_mov)
		{
			i++;
			if(_movimientos[i] == ult_mov)
			{
				cuenta_apariciones++;
				i++;
				posibles.push_back(_movimientos[i]);
				i--;
			}
			else
				i--;
		}
	}

	//Armo el vector de probabilidades con las apariciones de cada accion candidata
	for (vector<string>::iterator it = posibles.begin(); it != posibles.end(); it++)
	{
		ya_esta = false;
		for (vector<Probabilidad>::iterator ix = probables.begin(); ix != probables.end(); ix++)
		{
			if(ix->accion == *it)
			{
				ya_esta = true;
				ix->apariciones++;
				break;
			}
		}	
		if(!ya_esta)
			probables.push_back(Probabilidad(*it, 1, 1));
	}

	//Calculo las probabilidades de cada accion candidata y me quedo con la mayor
	for (vector<Probabilidad>::iterator ix = probables.begin(); ix != probables.end(); ix++)
	{
		ix->probabilidad = ix->apariciones / cuenta_apariciones;
		if(ix->probabilidad > mayor_proba)
		{
			mayor_proba = ix->probabilidad;
			accion_mas_probable = ix->accion;
		}
	}

	return accion_mas_probable;
}

void AI::CrearTablasLookUp(int _difficulty)
{
	//TODO: Armar distintas tablas para distintas dificultades

	//Prediccion: Idle
	LookUpTable.push_back(RegistroLookUp(IDLE, D_CLOSE, HIGH_PUNCH));
	LookUpTable.push_back(RegistroLookUp(IDLE, D_MED, FIRE));
	LookUpTable.push_back(RegistroLookUp(IDLE, D_FAR, WALK));

	//Prediccion: Salto izquierda
	LookUpTable.push_back(RegistroLookUp(UP_LEFT, D_CLOSE, DUCK));
	LookUpTable.push_back(RegistroLookUp(UP_LEFT, D_MED, BLOCK));
	LookUpTable.push_back(RegistroLookUp(UP_LEFT, D_FAR, FIRE));

	//Prediccion: Salto derecha
	LookUpTable.push_back(RegistroLookUp(UP_RIGHT, D_CLOSE, DUCK));
	LookUpTable.push_back(RegistroLookUp(UP_RIGHT, D_MED, BLOCK));
	LookUpTable.push_back(RegistroLookUp(UP_RIGHT, D_FAR, FIRE));
	
	//Prediccion: Salto
	LookUpTable.push_back(RegistroLookUp(UP, D_CLOSE, HIGH_KICK));
	LookUpTable.push_back(RegistroLookUp(UP, D_MED, FIRE));
	LookUpTable.push_back(RegistroLookUp(UP, D_FAR, FIRE));

	//Prediccion: Agacha
	LookUpTable.push_back(RegistroLookUp(DUCK, D_CLOSE, BARRE));
	LookUpTable.push_back(RegistroLookUp(DUCK, D_MED, WALK));
	LookUpTable.push_back(RegistroLookUp(DUCK, D_FAR, WALK));

	//Prediccion: Agacha y bloquea
	LookUpTable.push_back(RegistroLookUp(DUCK_BLOCK, D_CLOSE, BARRE));
	LookUpTable.push_back(RegistroLookUp(DUCK_BLOCK, D_MED, WALK));
	LookUpTable.push_back(RegistroLookUp(DUCK_BLOCK, D_FAR, WALK));

	//Prediccion: Camina a izquierda
	LookUpTable.push_back(RegistroLookUp(LEFT, D_CLOSE, HIGH_PUNCH));
	LookUpTable.push_back(RegistroLookUp(LEFT, D_MED, WALK));
	LookUpTable.push_back(RegistroLookUp(LEFT, D_FAR, FIRE));

	//Prediccion: Camina a derecha
	LookUpTable.push_back(RegistroLookUp(RIGHT, D_CLOSE, HIGH_PUNCH));
	LookUpTable.push_back(RegistroLookUp(RIGHT, D_MED, WALK));
	LookUpTable.push_back(RegistroLookUp(RIGHT, D_FAR, FIRE));

	//Prediccion: Dispara
	LookUpTable.push_back(RegistroLookUp(FIRE, D_CLOSE, BLOCK));
	LookUpTable.push_back(RegistroLookUp(FIRE, D_MED, BLOCK));
	LookUpTable.push_back(RegistroLookUp(FIRE, D_FAR, DUCK));

	//Prediccion: Bloquea
	LookUpTable.push_back(RegistroLookUp(BLOCK, D_CLOSE, HIGH_KICK));
	LookUpTable.push_back(RegistroLookUp(BLOCK, D_MED, FIRE));
	LookUpTable.push_back(RegistroLookUp(BLOCK, D_FAR, FIRE));

	//Prediccion: Golpe alto
	LookUpTable.push_back(RegistroLookUp(HIGH_PUNCH, D_CLOSE, BLOCK));
	LookUpTable.push_back(RegistroLookUp(HIGH_PUNCH, D_MED, FIRE));
	LookUpTable.push_back(RegistroLookUp(HIGH_PUNCH, D_FAR, FIRE));

	//Prediccion: Golpe bajo
	LookUpTable.push_back(RegistroLookUp(LOW_PUNCH, D_CLOSE, BLOCK));
	LookUpTable.push_back(RegistroLookUp(LOW_PUNCH, D_MED, FIRE));
	LookUpTable.push_back(RegistroLookUp(LOW_PUNCH, D_FAR, FIRE));

	//Prediccion: Patada alta
	LookUpTable.push_back(RegistroLookUp(HIGH_KICK, D_CLOSE, BLOCK));
	LookUpTable.push_back(RegistroLookUp(HIGH_KICK, D_MED, FIRE));
	LookUpTable.push_back(RegistroLookUp(HIGH_KICK, D_FAR, FIRE));

	//Prediccion: Patada baja
	LookUpTable.push_back(RegistroLookUp(LOW_KICK, D_CLOSE, BLOCK));
	LookUpTable.push_back(RegistroLookUp(LOW_KICK, D_MED, FIRE));
	LookUpTable.push_back(RegistroLookUp(LOW_KICK, D_FAR, FIRE));

	//Prediccion: Patada barre
	LookUpTable.push_back(RegistroLookUp(BARRE, D_CLOSE, BLOCK));
	LookUpTable.push_back(RegistroLookUp(BARRE, D_MED, FIRE));
	LookUpTable.push_back(RegistroLookUp(BARRE, D_FAR, FIRE));
}