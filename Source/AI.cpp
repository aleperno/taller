#include "AI.h"
using namespace std;

#define JMP_FACTOR 2

#define IDLE "I"
#define UP_LEFT "UL"
#define UP_RIGHT "UR"
#define UP "U"
#define UP_KICK "UK"
#define UP_PUNCH "UH"
#define DUCK "D"
#define DUCK_BLOCK "DB"
#define DUCK_KICK "DK"
#define DUCK_PUNCH "DP"
#define LEFT "L"
#define RIGHT "R"
#define FIRE "F"
#define BLOCK "B"
#define HIGH_PUNCH "HP"
#define LOW_PUNCH "LP"
#define HIGH_KICK "HK"
#define LOW_KICK "LK"
#define WALK "W"
#define D_CLOSE 0
#define D_FAR 1
#define TRACK_MOV 100

void AI::HandlePlayer(Personaje* _personaje, vector<string> _movimientosJugador)
{
	//TODO: Resolver AI jugador
	//Antes de ver qué movimiento hacer, tengo que predecir qué movimiento es más probable haga el jugador
	if(_movimientosJugador.size() >= TRACK_MOV)
	{
		//1- Calculo probabilidad
		//2- Con la predicción veo la tabla
	}
	
	
	//Pruebas
	/*for(int i = 0; i < _movimientosJugador.size(); i++)
	{
		cout << _movimientosJugador[i] + ", ";
	}
	_personaje->jump(JMP_FACTOR);
	_personaje->lanzarArma();*/
}

AI::AI(int _difficulty)
{
	CrearTablasLookUp(_difficulty);
}

AI::~AI()
{

}

void AI::CrearTablasLookUp(int _difficulty)
{
	LookUpTable.push_back(RegistroLookUp(IDLE, D_FAR, WALK));
	LookUpTable.push_back(RegistroLookUp(IDLE, D_CLOSE, HIGH_PUNCH));
}