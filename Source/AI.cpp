#include "AI.h"
using namespace std;

#define JMP_FACTOR 2
#define MOV_FACTOR2 0.8


void AI::HandlePlayer(Personaje* _personaje_AI, Personaje* _personaje_PLAYER)
{
	if(_personaje_PLAYER->track_movimientos.size() >= TRACK_MOV)
	{
		this->prediccion = this->ObtenerPrediccion(_personaje_PLAYER->track_movimientos);
		this->accion = this->ObtenerAccion(this->prediccion, this->CalcularDistacia(_personaje_AI->_pos_x, _personaje_PLAYER->_pos_x, _personaje_AI->_ancho_log));
		this->EjecutarMovimiento(_personaje_AI, this->accion);
	}
	else
	{
		//this->EjecutarMovimiento(_personaje_AI, FIRE);
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

	if(dist_entera < _ancho_log / 2)
		return 0;

	if((dist_entera > _ancho_log / 2) && (dist_entera < _ancho_log))
		return 1;

	if(dist_entera > _ancho_log)
		return 2;

	return 0;
}

void AI::EjecutarMovimiento(Personaje* _personaje, string _movimiento)
{
	if(_movimiento == IDLE)
		_personaje->idle();

	if(_movimiento == HIGH_PUNCH)
		_personaje->golpeAlto();

	if(_movimiento == HIGH_KICK)
		_personaje->patadaAlta();

	if(_movimiento == DUCK)
		_personaje->duck();

	if(_movimiento == BLOCK)
		_personaje->block();

	if(_movimiento == BARRE)
		_personaje->barrer();

	if(_movimiento == FIRE)
		_personaje->lanzarArma();

	if(_movimiento == WALK){
		if(!_personaje->_orientacion) {
			_personaje->moveRight(MOV_FACTOR2);
		}else{
			_personaje->moveLeft(MOV_FACTOR2);
		}
	}
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
	for(unsigned int i = 0; i < _movimientos.size() - 2; i++)
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
