#include "Parser.h"

using namespace std;

Parser* Parser::instance = NULL;

void Parser::setearVentanaPorDefecto(Value defVentana){
	this->ventana.ancho_px = defVentana.get("anchopx",-1).asInt();
	this->ventana.alto_px = defVentana.get("altopx",-1).asInt();
	this->ventana.ancho = defVentana.get("ancho",-1).asFloat();
}

void Parser::setearEscenarioPorDefecto(Value defEscenario){
	this->escenario.ancho = defEscenario.get("ancho",-1).asFloat();
	this->escenario.alto = defEscenario.get("alto",-1).asFloat();
	this->escenario.y_piso = defEscenario.get("ypiso",-1).asFloat();
	this->escenario.z_index = defEscenario.get("zindex",-1).asInt();
}

void Parser::setearPersonajePorDefecto(PersonajeData* personaje,Value defPersonaje){
	personaje->ancho = defPersonaje.get("ancho",-1).asFloat();
	personaje->alto = defPersonaje.get("alto",-1).asFloat();
	personaje->nombre = defPersonaje.get("nombre",1).asString();

	personaje->golpe_alto = defPersonaje.get("golpe-alto",-1).asInt();
	personaje->golpe_bajo = defPersonaje.get("golpe-bajo",-1).asInt();
	personaje->patada_alta = defPersonaje.get("patada-alta",-1).asInt();
	personaje->patada_baja = defPersonaje.get("patada-baja",-1).asInt();
	personaje->defensa = defPersonaje.get("defensa",-1).asInt();
	personaje->arrojar = defPersonaje.get("arrojar",-1).asInt();
}

void Parser::setearCapasPorDefecto(Value defCapas){
	int nroCapas = defCapas.size();
	for (int i=0; i<nroCapas; i++) {
		CapaData capaLocal = {	defCapas[i].get("imagen_fondo","").asString(),
								defCapas[i].get("ancho",-1).asFloat(),
		};
		this->capas.push_back(capaLocal);
	}
}

void Parser::parsearSpritePersonaje(PersonajeData* personaje, Value persValue){
	personaje->height = persValue.get("height",-1).asInt();
	personaje->width = persValue.get("width",-1).asInt();
	personaje->size = persValue.get("size",-1).asInt();

	int i = 0;
	personaje->cantSprites[i] = persValue["walk"][1].asInt();
	personaje->anchoSprites[i] = persValue["walk"][0].asInt();
	i++;

	personaje->cantSprites[i] = persValue["idle"][1].asInt();
	personaje->anchoSprites[i] = persValue["idle"][0].asInt();
	i++;

	personaje->cantSprites[i] = persValue["jumpUp"][1].asInt();
	personaje->anchoSprites[i] = persValue["jumpUp"][0].asInt();
	i++;

	personaje->cantSprites[i] = persValue["jumpFwd"][1].asInt();
	personaje->anchoSprites[i] = persValue["jumpFwd"][0].asInt();
	i++;

	personaje->cantSprites[i] = persValue["jumpBwd"][1].asInt();
	personaje->anchoSprites[i] = persValue["jumpBwd"][0].asInt();
	i++;

	personaje->cantSprites[i] = persValue["duck"][1].asInt();
	personaje->anchoSprites[i] = persValue["duck"][0].asInt();
	i++;

	/*
	for(int j = 0; j < i; j++)
	{
		cout << j+1 << " - " << personaje->cantSprites[j] << endl;
	}
	*/
	personaje->imgPath = persValue["imgSrc"].asString();

	/*Segun lo que vimos en la clase, los valores de color pueden ser float o cosas invalidas.
	En caso de que hay valor invalido ("pepe") tambien dijeron varias veces que una opcion
	es logear que no se puede hacer transformacion y no cambiar color. A esos fines se
	devuelven '-1' en este caso.*/
	bool colorAlternativoValido = true;
	string str = "";
	try	{	personaje->h_inicial = persValue.get("color-alternativo",-1).get("h-inicial",-1).asFloat();	}
	catch(const exception &e) {
		str = e.what();
		colorAlternativoValido = false;
	}
	if (colorAlternativoValido) {
		if (personaje->h_inicial == -1)		colorAlternativoValido = false;
	}

	if (colorAlternativoValido) {
		try	{	personaje->h_final = persValue.get("color-alternativo",-1).get("h-final",-1).asFloat();	}
		catch(const exception &e) {
			str = e.what();
			colorAlternativoValido = false;
		}
	}
	if (colorAlternativoValido) {
		if (personaje->h_final == -1)		colorAlternativoValido = false;
	}
	
	if (colorAlternativoValido) {
		try	{	personaje->desplazamiento = persValue.get("color-alternativo",-1).get("desplazamiento",-1).asFloat();	}
		catch(const exception &e) {
			str = e.what();
			colorAlternativoValido = false;
		}
	}
	if (colorAlternativoValido) {
		if (personaje->desplazamiento == -1)		colorAlternativoValido = false;
	}

	if (colorAlternativoValido) {
	//estandar solo si [0;360), si no pertenece a ese rango le doy vueltas
		while (!(personaje->h_inicial < 360)) {
			personaje->h_inicial -= 360;
		}
		while (personaje->h_inicial < 0) {
			personaje->h_inicial += 360;
		}
		while (!(personaje->h_final < 360)) {
			personaje->h_final -= 360;
		}
		while (personaje->h_final < 0) {
			personaje->h_final += 360;
		}
	} else {
		personaje->h_inicial = -1;
		personaje->h_final = -1;
		personaje->desplazamiento = -1;
		string msg = "Color alternativo de " + personaje->nombre + " es invalido. No sera posible cambiar color en caso de ser necesario.";
		if (str == "") {
			Logger::Instance()->log(ERROR,msg);
		} else {
			Logger::Instance()->log(ERROR,str + " " + msg);
		}
	}
}

void Parser::setearParseoDeSprite() {
	Value root;
	Reader reader;
	ifstream file(SPRITE_PARSE_PATH);

	if (!file.good())
	{
		Logger::Instance()->log(ERROR,"El archivo de parseo de sprites no existe.");
	}
	bool parseoExitoso = reader.parse(file,root);
	file.close();

	if(!parseoExitoso)
	{
		Logger::Instance()->log(ERROR,"El archivo de parseo de sprites contiene errores.");
	}

	Value persValue = root[this->personaje1.nombre];
	if (persValue.empty())
	{
		Logger::Instance()->log(ERROR,"El personaje \"" + this->personaje1.nombre + "\" (sprites) no existe. Se usa uno por defecto.");
		persValue = root["liukang"];
	}
	parsearSpritePersonaje(&(this->personaje1), persValue);

	persValue = root[this->personaje2.nombre];
	if (persValue.empty())
	{
		Logger::Instance()->log(ERROR,"El personaje \"" + this->personaje2.nombre + "\" (sprites) no existe. Se usa uno por defecto.");
		persValue = root["liukang"];
	}
	parsearSpritePersonaje(&(this->personaje2), persValue);

}

bool Parser::asignarBoton(int* boton, bool (&teclasUsadas)[6], Value persValue, int num, string valueString, string botonString) {
	try	{	*boton = persValue.get(valueString,-1).asInt();	}
		catch(const exception &e) {
			string str(e.what());
			string msg = " Boton de " + botonString + " del personaje " + to_string(static_cast<long double>(num)) + ".";
			Logger::Instance()->log(ERROR,str + msg);
			return false;
		}
		if ((*boton < 0) || (*boton > 5)) {
			string msg = "Boton de " + botonString + " del personaje " + to_string(static_cast<long double>(num)) + " es invalido o no definido.";
			Logger::Instance()->log(WARNING,msg);
			return false;
		} else {
			if (teclasUsadas[*boton] == true) {
				string msg = "Boton de " + botonString + " del personaje " + to_string(static_cast<long double>(num)) + " ya se usa.";
				Logger::Instance()->log(WARNING,msg);
				return false;
			} else {
				teclasUsadas[*boton] = true;
				return true;
			}
		}
}

void Parser::corregirBoton(int* boton, bool (&teclasUsadas)[6], int num, string botonString){
		int k = 0;
		while (teclasUsadas[k]){
			k++;
		}
		*boton = k;
		teclasUsadas[k] = true;
		string msg = "Personaje " + to_string(static_cast<long double>(num)) + ", boton de "
						+ botonString + ": " + to_string(static_cast<long double>(k)) + ".";
		Logger::Instance()->log(WARNING,msg);
}

void Parser::setearDatosPersonaje(PersonajeData* personaje, Value persValue, Value persDef, int num){
	//-----ancho-----
	try {	personaje->ancho = persValue.get("ancho",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			string msg = " Se usara valor por defecto de ancho del personaje " + to_string(static_cast<long double>(num)) + ".";
			Logger::Instance()->log(ERROR,str + msg);
			personaje->ancho = persDef.get("ancho",-1).asFloat();
		}
		if (personaje->ancho < 2) {
			personaje->ancho = persDef.get("ancho",-1).asFloat();
			string msg = "Ancho logico del personaje " + to_string(static_cast<long double>(num)) + " invalido o no definido. Se usa valor por defecto.";
			Logger::Instance()->log(WARNING,msg);
		}

	//-----alto-----
	try {	personaje->alto = persValue.get("alto",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			string msg = " Se usara valor por defecto de alto del personaje " + to_string(static_cast<long double>(num)) + ".";
			Logger::Instance()->log(ERROR,str + msg);
			personaje->alto = persDef.get("alto",-1).asFloat();
		}
		if (personaje->alto < 2) {
			personaje->alto = persDef.get("alto",-1).asFloat();
			string msg = "Alto logico del personaje " + to_string(static_cast<long double>(num)) + " invalido o no definido. Se usa valor por defecto.";
			Logger::Instance()->log(WARNING,msg);
		}

	//-----nombre-----
	//Mientras no hay error de parseo, todo es levantable como string.
	try {	personaje->nombre = persValue.get("nombre",-1).asString();	}
		catch(const exception &e) {
			string str(e.what());
			string msg =  " Se usara personaje (sprites) " + to_string(static_cast<long double>(num)) + " por defecto.";
			Logger::Instance()->log(ERROR,str + " Se usara personaje (sprites) por defecto.");
			personaje->nombre = persDef.get("nombre",-1).asString();
		}

	//-----joyconfig-----
	//Teclas validas: 0, 1, 2, 3, 4, 5
	//Primero asigna todos valores validos. Luego a los que son invalidos les pone teclas desocupadas en orden creciente.
	bool teclasUsadas[6] = { false };

	bool BotonGolpeAlto = asignarBoton(&(personaje->golpe_alto), teclasUsadas, persValue, num, "golpe-alto", "golpe alto");
	bool BotonGolpeBajo = asignarBoton(&(personaje->golpe_bajo), teclasUsadas, persValue, num, "golpe-bajo", "golpe bajo");
	bool BotonPatadaAlta = asignarBoton(&(personaje->patada_alta), teclasUsadas, persValue, num, "patada-alta", "patada alta");
	bool BotonPatadaBaja = asignarBoton(&(personaje->patada_baja), teclasUsadas, persValue, num, "patada-baja", "patada baja");
	bool BotonDefensa = asignarBoton(&(personaje->defensa), teclasUsadas, persValue, num, "defensa", "defensa");
	bool BotonArrojar = asignarBoton(&(personaje->arrojar), teclasUsadas, persValue, num, "arrojar", "arrojar");
	
	if (!BotonGolpeAlto)	corregirBoton(&(personaje->golpe_alto), teclasUsadas, num, "golpe alto");
	if (!BotonGolpeBajo)	corregirBoton(&(personaje->golpe_bajo), teclasUsadas, num, "golpe bajo");
	if (!BotonPatadaAlta)	corregirBoton(&(personaje->patada_alta), teclasUsadas, num, "patada alta");
	if (!BotonPatadaBaja)	corregirBoton(&(personaje->patada_baja), teclasUsadas, num, "patada baja");
	if (!BotonDefensa)	corregirBoton(&(personaje->defensa), teclasUsadas, num, "defensa");
	if (!BotonArrojar)	corregirBoton(&(personaje->arrojar), teclasUsadas, num, "arrojar");

}

Parser::Parser(Value defRoot) {

	setearVentanaPorDefecto(defRoot["ventana"]);
	setearEscenarioPorDefecto(defRoot["escenario"]);
	setearPersonajePorDefecto(&(this->personaje1),defRoot["personaje1"]);
	this->personaje1.orientacion = false;	//presonaje 1 es el que aparece al lado izquierdo, mirando a la derecha
	setearPersonajePorDefecto(&(this->personaje2),defRoot["personaje2"]);
	this->personaje2.orientacion = true;	// [ P1->		<-P2 ]
	setearCapasPorDefecto(defRoot["capas"]);

	setearParseoDeSprite();
}

Parser::Parser(Value root, Value defRoot){
	//FRENTE A AUSENCIA DE CUALQUIER COSA SE REEMPLAZA POR VALOR POR DEFECTO
	Value defVentana = defRoot["ventana"];
	Value defEscenario = defRoot["escenario"];
	Value defPersonaje1 = defRoot["personaje1"];
	Value defPersonaje2 = defRoot["personaje2"];
	Value defCapas = defRoot["capas"];

	bool hayVentana = true;
	bool hayEscenario = true;
	bool hayPersonaje1 = true;
	bool hayPersonaje2 = true;
	bool hayCapas = true;

	//Primero reviso estructuras enteras
	Value ventana = root["ventana"];
	if (ventana.empty()) {
		setearVentanaPorDefecto(defVentana);
		hayVentana = false;
		Logger::Instance()->log(ERROR,"Ventana no definida. Se usa ventana por defecto.");
	};

	Value escenario = root["escenario"];
	if (escenario.empty()) {
		setearEscenarioPorDefecto(defEscenario);
		hayEscenario = false;
		Logger::Instance()->log(ERROR,"Escenario no definido. Se usa escenario por defecto.");
	};
	
	Value personaje1 = root["personaje1"];
	if (personaje1.empty()) {
		setearPersonajePorDefecto(&(this->personaje1),defPersonaje1);
		hayPersonaje1 = false;
		Logger::Instance()->log(ERROR,"Personaje 1 no definido. Se usa personaje 1 por defecto.");
	};

	Value personaje2 = root["personaje2"];
	if (personaje2.empty()) {
		setearPersonajePorDefecto(&(this->personaje2),defPersonaje2);
		hayPersonaje2 = false;
		Logger::Instance()->log(ERROR,"Personaje 2 no definido. Se usa personaje 2 por defecto.");
	};

	Value capasJson = root["capas"];
	if (capasJson.empty()) {
		setearCapasPorDefecto(defCapas);
		hayCapas = false;
		Logger::Instance()->log(ERROR,"Capas no definidas. Se usan capas por defecto.");
	}

	//Aca ya se sabe cuales estructuras existen en archivo y cuales no (se pusieron por defecto ya)

	//Ventana
	if (hayVentana) {
		//-----anchopx-----
		try	{	this->ventana.ancho_px = ventana.get("anchopx",-1).asInt();	}
		catch(const exception &e) {
			string str(e.what());
			Logger::Instance()->log(ERROR,str + " Se usara valor por defecto de ancho de la ventana en pixeles.");
			this->ventana.ancho_px = defVentana.get("anchopx",-1).asInt();
		}
		if (this->ventana.ancho_px < 2) {
			this->ventana.ancho_px = defVentana.get("anchopx",-1).asInt();
			Logger::Instance()->log(WARNING,"Ancho de la ventana en pixeles invalido o no definido. Se usa valor por defecto.");
		}

		//-----altopx-----
		try {	this->ventana.alto_px = ventana.get("altopx",-1).asInt();	}
		catch(const exception &e) {
			string str(e.what());
			Logger::Instance()->log(ERROR,str + " Se usara valor por defecto de alto de la ventana en pixeles.");
			this->ventana.alto_px = defVentana.get("altopx",-1).asInt();
		}
		if (this->ventana.alto_px < 2) {
			this->ventana.alto_px = defVentana.get("altopx",-1).asInt();
			Logger::Instance()->log(WARNING,"Alto de la ventana en pixeles invalido o no definido. Se usa valor por defecto.");
		}

		//-----ancho-----
		try	{	this->ventana.ancho = ventana.get("ancho",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			Logger::Instance()->log(ERROR,str + " Se usara valor por defecto de ancho logico de la ventana.");
			this->ventana.ancho = defVentana.get("ancho",-1).asFloat();
		}
		if (this->ventana.ancho < 2) {
			this->ventana.ancho = defVentana.get("ancho",-1).asFloat();
			Logger::Instance()->log(WARNING,"Ancho logico de la ventana invalido o no definido. Se usa valor por defecto.");
		}
	}

	//Escenario
	if (hayEscenario) {
		//-----ancho-----
		try {	this->escenario.ancho = escenario.get("ancho",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			Logger::Instance()->log(ERROR,str + " Se usara valor por defecto de ancho logico del escenario.");
			this->escenario.ancho = defEscenario.get("ancho",-1).asFloat();
		}
		if (this->escenario.ancho < 2) {
			this->escenario.ancho = defEscenario.get("ancho",-1).asFloat();
			Logger::Instance()->log(WARNING,"Ancho logico del escenario invalido o no definido. Se usa valor por defecto.");
		}

		//-----alto-----
		try {	this->escenario.alto = escenario.get("alto",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			Logger::Instance()->log(ERROR,str + " Se usara valor por defecto de alto logico del escenario.");
			this->escenario.alto = defEscenario.get("alto",-1).asFloat();
		}
		if (this->escenario.alto < 2) {
			this->escenario.alto = defEscenario.get("alto",-1).asFloat();
			Logger::Instance()->log(WARNING,"Alto logico del escenario invalido o no definido. Se usa valor por defecto.");
		}

		//-----ypiso-----
		try {	this->escenario.y_piso = escenario.get("ypiso",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			Logger::Instance()->log(ERROR,str + " Se usara valor por defecto de altura del piso.");
			this->escenario.y_piso = defEscenario.get("ypiso",-1).asFloat();
		}
		if (this->escenario.y_piso < 0) {
			this->escenario.y_piso = defEscenario.get("ypiso",-1).asFloat();
			Logger::Instance()->log(WARNING,"Altura logica del piso invalida o no definida. Se usa valor por defecto.");
		}

		//-----zindex-----
		try {	this->escenario.z_index = escenario.get("zindex",-1).asInt();	}
		catch(const exception &e) {
			string str(e.what());
			Logger::Instance()->log(ERROR,str + " Se usara valor por defecto de z-index para personajes.");
			this->escenario.z_index = defEscenario.get("zindex",-1).asInt();
		}
		if (this->escenario.z_index < 0) {
			this->escenario.z_index = defEscenario.get("zindex",-1).asInt();
			Logger::Instance()->log(WARNING,"Z-index invalido o no definido. Se usa valor por defecto.");
		}
	}

	if (this->ventana.ancho > this->escenario.ancho) {
		Logger::Instance()->log(WARNING,"Ancho logico de la ventana es mas grande que el escenario. Se achica el ancho de la ventana.");
		this->ventana.ancho = this-> escenario.ancho;
	}

	//Personaje1
	if (hayPersonaje1) {
		setearDatosPersonaje(&(this->personaje1), personaje1, defPersonaje1, 1);
	}
	this->personaje1.orientacion = false;

	//Personaje2
	if (hayPersonaje2) {
		setearDatosPersonaje(&(this->personaje2), personaje2, defPersonaje2, 2);
	}
	this->personaje2.orientacion = true;

	setearParseoDeSprite();

	/*Llega con capas por defecto o asegurado que hay por lo menos una capa en json. Si las capas no son por defecto,
	analiza una por una y descarta las capas que tienen valores invalidos, salvo anchos numericos invalidos.
	En este caso estira ancho de capa si es mas angosta que ventana y comprime si supera ancho de escenario.
	Si al final se queda por lo menos 1 capa - usa esa. Y si son todas invalidas - carga las capas por defecto.*/
	if (hayCapas) {
		for (unsigned int i=0; i<capasJson.size(); i++) {

			bool capaSana = true;
			string rutaLocal;
			float anchoLocal;
			CapaData capaLocal;
			/*Es levantable como string la ruta a imagen? En todas pruebas, si no hubo error de parseo,
			logro levantar cualquier verdura como string. Por las dudas lo dejo.*/
			try {	rutaLocal = capasJson[i].get("imagen_fondo","").asString();	}
			catch(const exception &e) {
				string str(e.what());
				string msg = str + " Fondo de capa " + to_string(static_cast<long double>(i)) + ".";
				Logger::Instance()->log(ERROR,msg);
				capaSana = false;
			}

			//Es leantable como float su ancho?
			if (capaSana) {
				try {	anchoLocal = capasJson[i].get("ancho",-1).asFloat();	}
				catch(const exception &e) {
					string str(e.what());
					string msg = str + " Ancho de capa " + to_string(static_cast<long double>(i)) + ".";
					Logger::Instance()->log(ERROR,msg);
					capaSana = false;
				}
			}

			if (capaSana) {
				capaLocal.imagen_fondo = rutaLocal;
				capaLocal.ancho = anchoLocal;
				try{
					capaLocal.animado = capasJson[i].get("animado",false).asBool();
					capaLocal.height_px = capasJson[i].get("height_px",-1).asInt();
					capaLocal.sprites = capasJson[i].get("spritesCant",-1).asInt();
					capaLocal.animDelay = capasJson[i].get("animDelay",-1).asInt();
					//Logger::Instance()->log(ERROR,"TEST TRY");
				}catch(const exception &e){
					capaLocal.animado = false;
					//Logger::Instance()->log(ERROR,"TEST CATCH");
					capaLocal.height_px = 0;
					capaLocal.sprites = 0;
					capaLocal.animDelay = 0;
				}

				ifstream imagenTest(capaLocal.imagen_fondo.c_str());
				//Chequeo si el archivo existe.
				if (!imagenTest.good()) {
					string msg = "Imagen de fondo para capa " + to_string(static_cast<long double>(i)) + " no es indicada o no existe.";
					Logger::Instance()->log(ERROR,msg);
					capaSana = false;
					imagenTest.close();
				} else {
					//Chequeo si el ancho es valido.
					if (capaLocal.ancho < this->ventana.ancho) {
						string msg = "Ancho logico de la capa " + to_string(static_cast<long double>(i)) + " menor que ancho de la ventana. Se estira.";
						capaLocal.ancho = this->ventana.ancho;
						Logger::Instance()->log(WARNING,msg);
					}
					//if (capaLocal.ancho > this->escenario.ancho) {
					//	string msg = "Ancho logico de la capa " + to_string(static_cast<long double>(i)) + " mayor que ancho del escenario. Se comprime.";
					//	capaLocal.ancho = this->escenario.ancho;
					//	Logger::Instance()->log(WARNING,msg);
					//}
				}
			}

			if (capaSana) {
				this->capas.push_back(capaLocal);
			}
		}

		if (this->capas.size() == 0) {
			//No hay capas validas, agarro los que son por defecto.
			setearCapasPorDefecto(defCapas);
			Logger::Instance()->log(WARNING,"No hay capas con todos parametros validos. Se usaran capas por defecto.");
		} else {
			//Hay por lo menos una capa valida. Veo si son todas validas, si no es asi - aviso.
			if (this->capas.size() != capasJson.size()) {
				Logger::Instance()->log(WARNING,"Hay capas con uno o mas parametros invalidos. No se usan.");
			}
		}
	}

}

Parser* Parser::Instance(){
	return instance;
}

void Parser::Initialize(string path){
	Value root;
	Value defRoot;
	Reader reader;
	delete(instance);

	Logger::Instance()->log(DEBUG,"Configuracion a levantar: " + path);

	ifstream defaultConfigFile(DEFAULT_CONFIG_PATH);
	reader.parse(defaultConfigFile,defRoot);	//Queda parseado archivo por defecto. Siempre necesario.
	defaultConfigFile.close();

	if (path == DEFAULT_CONFIG_PATH) {
		instance = new Parser(defRoot);	//No se ingreso ruta.
		Logger::Instance()->log(WARNING,"No se ingreso ruta. Se usa configuracion por defecto.");

	} else {
		ifstream configFile(path);
		if (!configFile.good())	{
			instance = new Parser(defRoot);	//No existe archivo en ruta ingresada.
			Logger::Instance()->log(ERROR,"Archivo no existe. Se usa configuracion por defecto.");
			configFile.close();

		} else {
			bool parseoExitoso = reader.parse(configFile,root);
			configFile.close();
			if (!parseoExitoso) {
				instance = new Parser(defRoot);	//Archivo existe, tiene errores sintacticos.
				Logger::Instance()->log(ERROR,"Error de parseo. Se usa configuracion por defecto.\n" + reader.getFormattedErrorMessages());

			} else {
				//Archivo existe, no tiene errores sintacticos, pero puede tener semanticos.
				instance = new Parser(root,defRoot);
			}
		}
	}
	instance->_configPath = path;
}

void Parser::KillInstance(){
	if (instance != NULL) {
		delete(instance);
		instance = NULL;
	}
	Logger::Instance()->log(DEBUG,"Se destruye el Parser.");
}

void Parser::reload()
{
	string path = this->_configPath;
	Initialize(path);
}

