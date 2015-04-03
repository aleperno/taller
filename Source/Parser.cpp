#include "Nivel.h"

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

	//Ventana
	if (hayVentana) {
		//-----anchopx-----
		try	{	this->ventana.ancho_px = ventana.get("anchopx",-1).asInt();	}
		catch(const exception &e) {
			string str(e.what());
			Log::Instance()->log(WARNING,str + " Se usara valor por defecto de ancho de la ventana en pixeles.");
			this->ventana.ancho_px = defVentana.get("anchopx",-1).asInt();
		}
		if (this->ventana.ancho_px < 2) {
			this->ventana.ancho_px = defVentana.get("anchopx",-1).asInt();
			Log::Instance()->log(WARNING,"Ancho de la ventana en pixeles invalido o no definido. Se usa valor por defecto.");
		}

		//-----altopx-----
		try {	this->ventana.alto_px = ventana.get("altopx",-1).asInt();	}
		catch(const exception &e) {
			string str(e.what());
			Log::Instance()->log(WARNING,str + " Se usara valor por defecto de alto de la ventana en pixeles.");
			this->ventana.alto_px = defVentana.get("altopx",-1).asInt();
		}
		if (this->ventana.alto_px < 2) {
			this->ventana.alto_px = defVentana.get("altopx",-1).asInt();
			Log::Instance()->log(WARNING,"Alto de la ventana en pixeles invalido o no definido. Se usa valor por defecto.");
		}

		//-----ancho-----
		try	{	this->ventana.ancho = ventana.get("ancho",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			Log::Instance()->log(WARNING,str + " Se usara valor por defecto de ancho logico de la ventana.");
			this->ventana.ancho = defVentana.get("ancho",-1).asFloat();
		}
		if (this->ventana.ancho < 2) {
			this->ventana.ancho = defVentana.get("ancho",-1).asFloat();
			Log::Instance()->log(WARNING,"Ancho logico de la ventana invalido o no definido. Se usa valor por defecto.");
		}
	}

	//Escenario
	if (hayEscenario) {
		//-----ancho-----
		try {	this->escenario.ancho = escenario.get("ancho",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			Log::Instance()->log(WARNING,str + " Se usara valor por defecto de ancho logico del escenario.");
			this->escenario.ancho = defEscenario.get("ancho",-1).asFloat();
		}
		if (this->escenario.ancho < 2) {
			this->escenario.ancho = defEscenario.get("ancho",-1).asFloat();
			Log::Instance()->log(WARNING,"Ancho logico del escenario invalido o no definido. Se usa valor por defecto.");
		}

		//-----alto-----
		try {	this->escenario.alto = escenario.get("alto",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			Log::Instance()->log(WARNING,str + " Se usara valor por defecto de alto logico del escenario.");
			this->escenario.alto = defEscenario.get("alto",-1).asFloat();
		}
		if (this->escenario.alto < 2) {
			this->escenario.alto = defEscenario.get("alto",-1).asFloat();
			Log::Instance()->log(WARNING,"Alto logico del escenario invalido o no definido. Se usa valor por defecto.");
		}

		//-----ypiso-----
		try {	this->escenario.y_piso = escenario.get("ypiso",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			Log::Instance()->log(WARNING,str + " Se usara valor por defecto de altura del piso.");
			this->escenario.y_piso = defEscenario.get("ypiso",-1).asFloat();
		}
		if (this->escenario.y_piso < 0) {
			this->escenario.y_piso = defEscenario.get("ypiso",-1).asFloat();
			Log::Instance()->log(WARNING,"Altura logica del piso invalida o no definida. Se usa valor por defecto.");
		}
	}

	//Personaje
	if (hayPersonaje) {
		//-----ancho-----
		try {	this->personaje.ancho = personaje.get("ancho",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			Log::Instance()->log(WARNING,str + " Se usara valor por defecto de ancho del personaje.");
			this->personaje.ancho = defPersonaje.get("ancho",-1).asFloat();
		}
		if (this->personaje.ancho < 2) {
			this->personaje.ancho = defPersonaje.get("ancho",-1).asFloat();
			Log::Instance()->log(WARNING,"Ancho logico del personaje invalido o no definido. Se usa valor por defecto.");
		}

		//-----alto-----
		try {	this->personaje.alto = personaje.get("alto",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			Log::Instance()->log(WARNING,str + " Se usara valor por defecto de alto del personaje.");
			this->personaje.alto = defPersonaje.get("alto",-1).asFloat();
		}
		if (this->personaje.alto < 2) {
			this->personaje.alto = defPersonaje.get("alto",-1).asFloat();
			Log::Instance()->log(WARNING,"Alto logico del personaje invalido o no definido. Se usa valor por defecto.");
		}

		//-----zindex-----
		try {	this->personaje.z_index = personaje.get("zindex",-1).asInt();	}
		catch(const exception &e) {
			string str(e.what());
			Log::Instance()->log(WARNING,str + " Se usara valor por defecto de z-index del personaje.");
			this->personaje.z_index = defPersonaje.get("zindex",-1).asInt();
		}
		if (this->personaje.z_index < 0) {
			this->personaje.z_index = defPersonaje.get("zindex",-1).asInt();
			Log::Instance()->log(WARNING,"Z-Index del personaje invalido o no definido. Se usa valor por defecto.");
		}

		//-----orientacion-----
		/*Es un poco distinto de los casos anteriores, ya que true/false/numero siempre es interpretable como bool y el
		unico caso cuando no lo puede levantar es cuando es un string. O sea, si lo levanta asBool, es correcto siempre. */
		try{	this->personaje.orientacion = personaje.get("orientacion",1).asBool();	}
		catch(const exception &e) {
			string str(e.what());
			Log::Instance()->log(WARNING,str + " Se usara valor por defecto de orientacion del personaje.");
			this->personaje.orientacion = defPersonaje.get("orientacion",1).asBool();
		}
	}

	/*Llega con capas por defecto o asegurado que hay por lo menos una capa en json. Si las capas no son por defecto,
	analiza una por una y descarta las capas que tienen valores invalidos. Si al final se queda por lo menos 1 capa - usa esa.
	Y si son todas invalidas - carga las capas por defecto.*/
	if (hayCapas) {
		for (unsigned int i=0; i<capasJson.size(); i++) {

			bool capaSana = true;
			string rutaLocal;
			float anchoLocal;
			capa capaLocal;

			/*Es levantable como string la ruta a imagen? En todas pruebas, si no hubo error de parseo,
			logro levantar cualquier verdura como string. Por las dudas lo dejo.*/
			try {	rutaLocal = capasJson[i].get("imagen_fondo","").asString();	}
			catch(const exception &e) {
				string str(e.what());
				string msg = str + " Fondo de capa " + to_string(static_cast<long double>(i)) + ".";
				Log::Instance()->log(WARNING,msg);
				capaSana = false;
			}

			//Es leantable como float su ancho?
			if (capaSana) {
				try {	anchoLocal = capasJson[i].get("ancho",-1).asFloat();	}
				catch(const exception &e) {
					string str(e.what());
					string msg = str + " Ancho de capa " + to_string(static_cast<long double>(i)) + ".";
					Log::Instance()->log(WARNING,msg);
					capaSana = false;
				}
			}

			if (capaSana) {
				capaLocal.imagen_fondo = rutaLocal;
				capaLocal.ancho = anchoLocal;

				ifstream imagenTest(capaLocal.imagen_fondo.c_str());
				//Chequeo si el archivo existe.
				if (!imagenTest.good()) {
					string msg = "Imagen de fondo para capa " + to_string(static_cast<long double>(i)) + " no es indicada o no existe.";
					Log::Instance()->log(WARNING,msg);
					capaSana = false;
					imagenTest.close();
				} else {
					//Chequeo si el ancho es valido.
					if (capaLocal.ancho < 2) {
						string msg = "Ancho logico de la capa " + to_string(static_cast<long double>(i)) + " es invalido o no se define.";
						Log::Instance()->log(WARNING,msg);
						capaSana = false;
					}
				}
			}

			if (capaSana) {
				this->capas.push_back(capaLocal);
			}
		}

		if (this->capas.size() == 0) {
			//No hay capas validas, agarro los que son por defecto.
			setearCapasPorDefecto(defCapas);
			Log::Instance()->log(WARNING,"No hay capas con todos parametros validos. Se usaran capas por defecto.");
		} else {
			//Hay por lo menos una capa valida. Veo si son todas validas, si no es asi - aviso.
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
	if (instance != NULL) {
		delete(instance);
		instance = NULL;
	}
}