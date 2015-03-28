#ifndef NIVEL_H_INCLUDED
#define NIVEL_H_INCLUDED

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <json.h>
#include <Log.h>
#include <Personaje.h>
#include <Ventana.h>
#include <Capa.h>
#include <Escritorio.h>

using namespace Json;
using namespace std;

#define DEFAULT_CONFIG_PATH "Config/defconfig.json"
#define SPRITE_SHEET_PATH "Config/spritesheet.png"

using namespace std;

class Nivel{

	private:
		//Este se usa cuando o no se especifico archivo, o no existe o tiene error de sintaxis
		Nivel(Value defRoot);

		//Este se usa cuando no se usa el de arriba, pero archivo indicado podria tener
		//errores simanticos lo cual implica que pueden ser necesarios los datos por defecto
		Nivel(Value root, Value defRoot);

		void setearVentanaPorDefecto(Value defVentana);
		void setearEscenarioPorDefecto(Value defEscenario);
		void setearPersonajePorDefecto(Value defPersonaje);
		void setearCapasPorDefecto(Value defCapas);

		static Nivel* instance;

	public:
		//Inicializa instancia usando ruta de *.json
		static void Initialize(string path);

		//Referencia a instancia
		static Nivel* Instance();

		//Destructor
		static void KillInstance();

		//Los datos de Nivel incluyen parametros de ventana, escenario, capas y personaje
		//Ruta a sprite sheet es unica para todas ejecuciones
		Ventana ventana;
		Escenario escenario;
		vector<capa> capas;
		Personaje personaje;
		string spriteSheetPath;
	
};

#endif // NIVEL_H_INCLUDED
