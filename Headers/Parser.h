#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <json.h>
#include <Logger.h>
#include <PersonajeData.h>
#include <VentanaData.h>
#include <CapaData.h>
#include <EscenarioData.h>

using namespace Json;
using namespace std;

#define DEFAULT_CONFIG_PATH "Config/defconfig.json"
#define SPRITE_PARSE_PATH "Config/spriteparse.json"

using namespace std;

class Parser{

	private:
		void setearParseoDeSprite();
		void parsearSpritePersonaje(PersonajeData* personaje, Value persValue);
		bool asignarBoton(int* boton, bool (&teclasUsadas)[6], Value persValue, int num, string valueString, string botonString);
		void corregirBoton(int* boton, bool (&teclasUsadas)[6], int num, string botonString);
		void setearDatosPersonaje(PersonajeData* personaje, Value persValue, Value persDef, int num);

		//Este se usa cuando o no se especifico archivo, o no existe o tiene error de sintaxis
		Parser(Value defRoot);

		//Este se usa cuando no se usa el de arriba, pero archivo indicado podria tener
		//errores semanticos lo cual implica que pueden ser necesarios los datos por defecto
		Parser(Value root, Value defRoot);
		
		void setearVentanaPorDefecto(Value defVentana);
		void setearEscenarioPorDefecto(Value defEscenario);
		void setearPersonajePorDefecto(PersonajeData* personaje,Value defPersonaje);
		void setearCapasPorDefecto(Value defCapas);

		static Parser* instance;
		string _configPath;

	public:
		//Inicializa instancia usando ruta de *.json
		static void Initialize(string path = DEFAULT_CONFIG_PATH);

		//Referencia a instancia
		static Parser* Instance();

		//Destructor
		static void KillInstance();
		void reload();

		//Los datos de Parser incluyen parametros de ventana, escenario, capas y personaje
		//Ruta a sprite sheet es unica para todas ejecuciones
		VentanaData ventana;
		EscenarioData escenario;
		vector<CapaData> capas;
		PersonajeData personaje1;
		PersonajeData personaje2;
	
};

#endif // PARSER_H_INCLUDED
