/*
 * main.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: neo
 */
#include <SDL.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <json.h>
#include <Log.h>
#include <Nivel.h>

int main( int argc, char* args[] )
{
	if (argc == 1) {
			Nivel::Initialize(DEFAULT_CONFIG_PATH);
		} else {
			Nivel::Initialize(args[1]);
		}

		cout << Nivel::Instance()->ventana.ancho_px << endl;
		cout << Nivel::Instance()->ventana.alto_px << endl;
		cout << Nivel::Instance()->ventana.ancho << endl << endl;

		cout << Nivel::Instance()->escenario.ancho << endl;
		cout << Nivel::Instance()->escenario.alto << endl;
		cout << Nivel::Instance()->escenario.y_piso << endl << endl;

		cout << Nivel::Instance()->personaje.ancho << endl;
		cout << Nivel::Instance()->personaje.alto << endl;
		cout << Nivel::Instance()->personaje.z_index << endl;
		cout << Nivel::Instance()->personaje.orientacion << endl << endl;;

		cout << Nivel::Instance()->spriteSheetPath << endl << endl;

		for (unsigned int i=0; i<Nivel::Instance()->capas.size(); i++) {
			cout << Nivel::Instance()->capas[i].imagen_fondo << endl;
			cout << Nivel::Instance()->capas[i].ancho << endl;
		}

		Nivel::KillInstance();

	return 0;
}
