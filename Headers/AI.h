#ifndef AI_H_
#define AI_H_

#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#include <Logger.h>
#include <Personaje.h>
#include <vector>

struct RegistroLookUp
{
	string prediccion;
	int distancia;
	string accion;
	RegistroLookUp(string _prediccion, int _distancia, string _accion):prediccion(_prediccion), distancia(_distancia), accion(_accion){};
};

class AI
{
	private:
		void CrearTablasLookUp(int _difficulty);
		vector<RegistroLookUp> LookUpTable;

	public:
		AI(int _difficulty);
		~AI();
		void HandlePlayer(Personaje* _personaje, vector<string> _movimientosJugador);
};

#endif /*AI_H_ */