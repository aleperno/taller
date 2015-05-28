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

struct Probabilidad
{
	string accion;
	int apariciones;
	float probabilidad;
	Probabilidad(string _accion, int _apariciones, float _probabilidad):accion(_accion), apariciones(_apariciones), probabilidad(_probabilidad){};
};

class AI
{
	private:
		string prediccion;
		string accion;
		void CrearTablasLookUp(int _difficulty);
		vector<RegistroLookUp> LookUpTable;
		string ObtenerAccion(string _prediccion, int _distancia);
		string ObtenerPrediccion(vector<string> _movimientos);
		void EjecutarMovimiento(Personaje* _personaje, string _movimiento);
		int CalcularDistacia(float _pos_x_ai, float _pos_x_player, int _ancho_log);

	public:
		AI(int _difficulty);
		~AI();
		void HandlePlayer(Personaje* _personaje_AI, Personaje* _personaje_PLAYER);
};

#endif /*AI_H_ */