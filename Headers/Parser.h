#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <json.h>
#include <Logger.h>
#include <CaracterData.h>
#include <JugadorData.h>
#include <VentanaData.h>
#include <CapaData.h>
#include <EscenarioData.h>

using namespace Json;
using namespace std;

#define DEFAULT_CONFIG_PATH "Config/defconfig.json"
#define SPRITE_PARSE_PATH "Config/spriteparse.json"

class Parser{

	private:
		static Parser* instance;
		string _configPath;

		//Este se usa cuando o no se especifico archivo, o no existe, o tiene error de sintaxis
		Parser(Value defRoot);
		void setearVentanaPorDefecto(Value defVentana);
		void setearEscenarioPorDefecto(Value defEscenario);
		void setearCapasPorDefecto(Value defCapas);
		void setearJugadorPorDefecto(JugadorData* jugador, Value defJugador);

		//Este se usa cuando no se usa el de arriba, pero archivo indicado podria tener
		//errores semanticos lo cual implica que pueden ser necesarios los datos por defecto
		Parser(Value root, Value defRoot);
		bool asignarBoton(int* boton, bool (&teclasUsadas)[8], Value jugValue, int num, string valueString, string botonString);
		void corregirBoton(int* boton, bool (&teclasUsadas)[8], int num, string botonString);
		void setearDatosJugador(JugadorData* juagador, Value jugValue, Value jugDef, int num);

		//Eso levanta datos de spriteparse
		void setearCaracteres();
		void parsearSpriteCaracter(CaracterData* caracter, Value carValue);
		void parsearAccion(CaracterData* caracter, Value carValue, string accion);
		void revisarSuperposicionDeTomas(CaracterData* caracter);
			
	public:
		//Inicializa instancia usando ruta de *.json
		static void Initialize(string path = DEFAULT_CONFIG_PATH);

		//Referencia a instancia
		static Parser* Instance();

		//Destructor
		static void KillInstance();
		void reload();

		VentanaData ventana;
		EscenarioData escenario;
		vector<CapaData> capas;

		JugadorData jugador1;
		JugadorData jugador2;

		CaracterData liukang;
		CaracterData scorpion;
};

#endif // PARSER_H_INCLUDED
