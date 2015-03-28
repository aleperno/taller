#include "Nivel.h"
#include <Personaje.h>

using namespace std;

Nivel* Nivel::instance = NULL;

void Nivel::setearVentanaPorDefecto(Value defVentana){
	this->ventana.ancho_px = defVentana.get("anchopx",-1).asInt();
	this->ventana.alto_px = defVentana.get("altopx",-1).asInt();
	this->ventana.ancho = defVentana.get("ancho",-1).asFloat();
}

void Nivel::setearEscenarioPorDefecto(Value defEscenario){
	this->escenario.ancho = defEscenario.get("ancho",-1).asFloat();
	this->escenario.alto = defEscenario.get("alto",-1).asFloat();
	this->escenario.y_piso = defEscenario.get("ypiso",-1).asFloat();
}

void Nivel::setearPersonajePorDefecto(Value defPersonaje){
	this->personaje.ancho = defPersonaje.get("ancho",-1).asFloat();
	this->personaje.alto = defPersonaje.get("alto",-1).asFloat();
	this->personaje.z_index = defPersonaje.get("zindex",-1).asInt();
	this->personaje.orientacion = defPersonaje.get("orientacion",1).asBool();
}

void Nivel::setearCapasPorDefecto(Value defCapas){
	int nroCapas = defCapas.size();
	for (int i=0; i<nroCapas; i++) {
		capa capaLocal = {	defCapas[i].get("imagen_fondo","").asString(),
							defCapas[i].get("ancho",-1).asFloat(),
		};
		this->capas.push_back(capaLocal);
	}
}

Nivel::Nivel(Value defRoot) {
	setearVentanaPorDefecto(defRoot["ventana"]);
	setearEscenarioPorDefecto(defRoot["escenario"]);
	setearPersonajePorDefecto(defRoot["personaje"]);
	setearCapasPorDefecto(defRoot["capas"]);

	this->spriteSheetPath = SPRITE_SHEET_PATH;
}

Nivel::Nivel(Value root, Value defRoot){
	//FRENTE A AUSENCIA DE CUALQUIER COSA SE REEMPLAZA POR VALOR POR DEFECTO
	Value defVentana = defRoot["ventana"];
	Value defEscenario = defRoot["escenario"];
	Value defPersonaje = defRoot["personaje"];
	Value defCapas = defRoot["capas"];

	this->spriteSheetPath = SPRITE_SHEET_PATH;

	bool hayVentana = true;
	bool hayEscenario = true;
	bool hayPersonaje = true;
	bool hayCapas = true;

	//Primero reviso estructuras enteras
	Value ventana = root["ventana"];
	if (ventana.empty()) {
		setearVentanaPorDefecto(defVentana);
		hayVentana = false;
		Log::Instance()->log(WARNING,"Ventana no definida. Se usa ventana por defecto.");
	};

	Value escenario = root["escenario"];
	if (escenario.empty()) {
		setearEscenarioPorDefecto(defEscenario);
		hayEscenario = false;
		Log::Instance()->log(WARNING,"Escenario no definido. Se usa escenario por defecto.");
	};
	
	Value personaje = root["personaje"];
	if (personaje.empty()) {
		setearPersonajePorDefecto(defPersonaje);
		hayPersonaje = false;
		Log::Instance()->log(WARNING,"Personaje no definido. Se usa personaje por defecto.");
	};

	Value capasJson = root["capas"];
	if (capasJson.empty()) {
		setearCapasPorDefecto(defCapas);
		hayCapas = false;
		Log::Instance()->log(WARNING,"Capas no definidas. Se usan capas por defecto.");
	}

	//Aca ya se sabe cuales estructuras existen en archivo y cuales no (se pusieron por defecto ya)
	if (hayVentana) {
		this->ventana.ancho_px = ventana.get("anchopx",-1).asInt();
		if (this->ventana.ancho_px < 1) {
			this->ventana.ancho_px = defVentana.get("anchopx",-1).asInt();
			Log::Instance()->log(WARNING,"Ancho de la ventana en pixeles invalido o no definido. Se usa valor por defecto.");
		}

		this->ventana.alto_px = ventana.get("altopx",-1).asInt();
		if (this->ventana.alto_px < 1) {
			this->ventana.alto_px = defVentana.get("altopx",-1).asInt();
			Log::Instance()->log(WARNING,"Alto de la ventana en pixeles invalido o no definido. Se usa valor por defecto.");
		}
		try
		{
		this->ventana.ancho = ventana.get("ancho",-1).asFloat();
		}catch(...)
		{
			//Hay que agregar cual es el error y escribirlo en el log - Nik
			this->ventana.ancho = defVentana.get("ancho",-1).asFloat();
		}
		if (this->ventana.ancho < 1) {
			this->ventana.ancho = defVentana.get("ancho",-1).asFloat();
			Log::Instance()->log(WARNING,"Ancho logico de la ventana invalido o no definido. Se usa valor por defecto.");
		}
	}

	if (hayEscenario) {
		this->escenario.ancho = escenario.get("ancho",-1).asFloat();
		if (this->escenario.ancho < 1) {
			this->escenario.ancho = defEscenario.get("ancho",-1).asFloat();
			Log::Instance()->log(WARNING,"Ancho logico del escenario invalido o no definido. Se usa valor por defecto.");
		}

		this->escenario.alto = escenario.get("alto",-1).asFloat();
		if (this->escenario.alto < 1) {
			this->escenario.alto = defEscenario.get("alto",-1).asFloat();
			Log::Instance()->log(WARNING,"Alto logico del escenario invalido o no definido. Se usa valor por defecto.");
		}

		this->escenario.y_piso = escenario.get("ypiso",-1).asFloat();
		if (this->escenario.y_piso < 0) {
			this->escenario.y_piso = defEscenario.get("ypiso",-1).asFloat();
			Log::Instance()->log(WARNING,"Altura logica del piso invalida o no definida. Se usa valor por defecto.");
		}
	}

	if (hayPersonaje) {
		this->personaje.ancho = personaje.get("ancho",-1).asFloat();
		if (this->personaje.ancho < 1) {
			this->personaje.ancho = defPersonaje.get("ancho",-1).asFloat();
			Log::Instance()->log(WARNING,"Ancho logico del personaje invalido o no definido. Se usa valor por defecto.");
		}

		this->personaje.alto = personaje.get("alto",-1).asFloat();
		if (this->personaje.alto < 1) {
			this->personaje.alto = defPersonaje.get("alto",-1).asFloat();
			Log::Instance()->log(WARNING,"Alto logico del personaje invalido o no definido. Se usa valor por defecto.");
		}

		this->personaje.z_index = personaje.get("zindex",-1).asInt();
		if (this->personaje.z_index < 0) {
			this->personaje.z_index = defPersonaje.get("zindex",-1).asInt();
			Log::Instance()->log(WARNING,"Z-Index del personaje invalido o no definido. Se usa valor por defecto.");
		}
		try{
		this->personaje.orientacion = personaje.get("orientacion",1).asBool();
		}catch(...){
			this->personaje.orientacion = defPersonaje.get("orientacion",1).asBool();
			Log::Instance()->log(WARNING,"Orientacion del personaje invalida o no definida. Se usa valor por defecto.");
		}
		/*
		if ((this->personaje.orientacion != "left") && (this->personaje.orientacion != "right" )) {
			this->personaje.orientacion = defPersonaje.get("orientacion","").asString();
			Log::Instance()->log(WARNING,"Orientacion del personaje invalida o no definida. Se usa valor por defecto.");
		}*/

	}

	//Llega con capas por defecto o asegurado que hay por lo menos una capa en json
	if (hayCapas) {
		for (unsigned int i=0; i<capasJson.size(); i++) {
			bool capaSana = true;
			capa capaLocal = {	capasJson[i].get("imagen_fondo","").asString(),
								capasJson[i].get("ancho",-1).asFloat(),
			};
			ifstream imagenTest(capaLocal.imagen_fondo.c_str());
			if (!imagenTest.good()) {
				string msg = "Imagen de fondo para capa " + to_string(static_cast<long double>(i)) + " no es indicada o no existe.";
				Log::Instance()->log(WARNING,msg);
				capaSana = false;
				imagenTest.close();
			} else {
				if (capaLocal.ancho < 1) {
					string msg = "Ancho logico de la capa " + to_string(static_cast<long double>(i)) + " es invalido o no se define.";
					Log::Instance()->log(WARNING,msg);
					capaSana = false;
				}
			}
			if (capaSana) {
				this->capas.push_back(capaLocal);
			}
		}

		if (this->capas.size() == 0) {
			setearCapasPorDefecto(defCapas);
			Log::Instance()->log(WARNING,"No capas con todos parametros validos. Se usaran capas por defecto.");
		} else {
			if (this->capas.size() != capasJson.size()) {
				Log::Instance()->log(WARNING,"Hay capas con uno o mas parametros invalidos. No se usan.");
			}
		}
	}
}

Nivel* Nivel::Instance(){
	return instance;
}

void Nivel::Initialize(string path){
	Value root;
	Value defRoot;
	Reader reader;

	delete(instance);

	ifstream defaultConfigFile(DEFAULT_CONFIG_PATH);
	reader.parse(defaultConfigFile,defRoot);	//Queda parseado archivo por defecto. Siempre necesario.
	defaultConfigFile.close();

	if (path == DEFAULT_CONFIG_PATH) {
		instance = new Nivel(defRoot);	//No se ingreso ruta.
		Log::Instance()->log(WARNING,"No se ingreso ruta. Se usa configuracion por defecto.");

	} else {
		ifstream configFile(path);
		if (!configFile.good())	{
			instance = new Nivel(defRoot);	//No existe archivo en ruta ingresada.
			Log::Instance()->log(WARNING,"Archivo no existe. Se usa configuracion por defecto.");
			configFile.close();

		} else {
			bool parseoExitoso = reader.parse(configFile,root);
			configFile.close();
			if (!parseoExitoso) {
				instance = new Nivel(defRoot);	//Archivo existe, tiene errores sintacticos.
				Log::Instance()->log(WARNING,"Error de parseo. Se usa configuracion por defecto.\n" + reader.getFormattedErrorMessages());

			} else {
				//Archivo existe, no tiene errores sintacticos, pero puede tener semanticos.
				instance = new Nivel(root,defRoot);
			}
		}
	}
}

void Nivel::KillInstance(){
	delete(instance);
	instance = NULL;
}
