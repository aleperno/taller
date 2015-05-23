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

void Parser::setearJugadorPorDefecto(JugadorData* jugador, Value defJugador){
	jugador->ancho = defJugador.get("ancho",-1).asFloat();
	jugador->alto = defJugador.get("alto",-1).asFloat();
	jugador->arma_speed = defJugador.get("arma_speed",-1).asFloat();
	jugador->golpe_alto = defJugador.get("golpe-alto",-1).asInt();
	jugador->golpe_bajo = defJugador.get("golpe-bajo",-1).asInt();
	jugador->patada_alta = defJugador.get("patada-alta",-1).asInt();
	jugador->patada_baja = defJugador.get("patada-baja",-1).asInt();
	jugador->defensa = defJugador.get("defensa",-1).asInt();
	jugador->arrojar = defJugador.get("arrojar",-1).asInt();
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

void Parser::parsearSpriteCaracter(CaracterData* caracter, Value carValue) {
	caracter->height = carValue.get("height",-1).asInt();
	caracter->width = carValue.get("width",-1).asInt();
	caracter->size = carValue.get("size",-1).asInt();

	caracter->velSprites.push_back(carValue["walk"][2].asInt());
	caracter->cantSprites.push_back(carValue["walk"][1].asInt());
	caracter->anchoSprites.push_back(carValue["walk"][0].asInt());

	caracter->velSprites.push_back(carValue["idle"][2].asInt());
	caracter->cantSprites.push_back(carValue["idle"][1].asInt());
	caracter->anchoSprites.push_back(carValue["idle"][0].asInt());

	caracter->velSprites.push_back(carValue["jumpUp"][2].asInt());
	caracter->cantSprites.push_back(carValue["jumpUp"][1].asInt());
	caracter->anchoSprites.push_back(carValue["jumpUp"][0].asInt());

	caracter->velSprites.push_back(carValue["jumpFwd"][2].asInt());
	caracter->cantSprites.push_back(carValue["jumpFwd"][1].asInt());
	caracter->anchoSprites.push_back(carValue["jumpFwd"][0].asInt());

	caracter->velSprites.push_back(carValue["jumpBwd"][2].asInt());
	caracter->cantSprites.push_back(carValue["jumpBwd"][1].asInt());
	caracter->anchoSprites.push_back(carValue["jumpBwd"][0].asInt());

	caracter->velSprites.push_back(carValue["duck"][2].asInt());
	caracter->cantSprites.push_back(carValue["duck"][1].asInt());
	caracter->anchoSprites.push_back(carValue["duck"][0].asInt());

	caracter->velSprites.push_back(carValue["block"][2].asInt());
	caracter->cantSprites.push_back(carValue["block"][1].asInt());
	caracter->anchoSprites.push_back(carValue["block"][0].asInt());

	caracter->velSprites.push_back(carValue["blockDuck"][2].asInt());
	caracter->cantSprites.push_back(carValue["blockDuck"][1].asInt());
	caracter->anchoSprites.push_back(carValue["blockDuck"][0].asInt());

	caracter->velSprites.push_back(carValue["dizzy"][2].asInt());
	caracter->cantSprites.push_back(carValue["dizzy"][1].asInt());
	caracter->anchoSprites.push_back(carValue["dizzy"][0].asInt());

	caracter->velSprites.push_back(carValue["hittedDuck"][2].asInt());
	caracter->cantSprites.push_back(carValue["hittedDuck"][1].asInt());
	caracter->anchoSprites.push_back(carValue["hittedDuck"][0].asInt());

	caracter->velSprites.push_back(carValue["hitted"][2].asInt());
	caracter->cantSprites.push_back(carValue["hitted"][1].asInt());
	caracter->anchoSprites.push_back(carValue["hitted"][0].asInt());

	caracter->velSprites.push_back(carValue["duckHiKick"][2].asInt());
	caracter->cantSprites.push_back(carValue["duckHiKick"][1].asInt());
	caracter->anchoSprites.push_back(carValue["duckHiKick"][0].asInt());

	caracter->velSprites.push_back(carValue["hiKick"][2].asInt());
	caracter->cantSprites.push_back(carValue["hiKick"][1].asInt());
	caracter->anchoSprites.push_back(carValue["hiKick"][0].asInt());

	caracter->velSprites.push_back(carValue["duckLoKick"][2].asInt());
	caracter->cantSprites.push_back(carValue["duckLoKick"][1].asInt());
	caracter->anchoSprites.push_back(carValue["duckLoKick"][0].asInt());

	caracter->velSprites.push_back(carValue["loKick"][2].asInt());
	caracter->cantSprites.push_back(carValue["loKick"][1].asInt());
	caracter->anchoSprites.push_back(carValue["loKick"][0].asInt());

	caracter->velSprites.push_back(carValue["airKick"][2].asInt());
	caracter->cantSprites.push_back(carValue["airKick"][1].asInt());
	caracter->anchoSprites.push_back(carValue["airKick"][0].asInt());

	caracter->velSprites.push_back(carValue["hiPunch"][2].asInt());
	caracter->cantSprites.push_back(carValue["hiPunch"][1].asInt());
	caracter->anchoSprites.push_back(carValue["hiPunch"][0].asInt());

	caracter->velSprites.push_back(carValue["loPunch"][2].asInt());
	caracter->cantSprites.push_back(carValue["loPunch"][1].asInt());
	caracter->anchoSprites.push_back(carValue["loPunch"][0].asInt());

	caracter->velSprites.push_back(carValue["airPunch"][2].asInt());
	caracter->cantSprites.push_back(carValue["airPunch"][1].asInt());
	caracter->anchoSprites.push_back(carValue["airPunch"][0].asInt());

	caracter->velSprites.push_back(carValue["fallDead"][2].asInt());
	caracter->cantSprites.push_back(carValue["fallDead"][1].asInt());
	caracter->anchoSprites.push_back(carValue["fallDead"][0].asInt());

	caracter->velSprites.push_back(carValue["falling"][2].asInt());
	caracter->cantSprites.push_back(carValue["falling"][1].asInt());
	caracter->anchoSprites.push_back(carValue["falling"][0].asInt());

	caracter->velSprites.push_back(carValue["gancho"][2].asInt());
	caracter->cantSprites.push_back(carValue["gancho"][1].asInt());
	caracter->anchoSprites.push_back(carValue["gancho"][0].asInt());

	caracter->velSprites.push_back(carValue["barrido"][2].asInt());
	caracter->cantSprites.push_back(carValue["barrido"][1].asInt());
	caracter->anchoSprites.push_back(carValue["barrido"][0].asInt());

	caracter->velSprites.push_back(carValue["arma"][2].asInt());
	caracter->cantSprites.push_back(carValue["arma"][1].asInt());
	caracter->anchoSprites.push_back(carValue["arma"][0].asInt());

	caracter->velSprites.push_back(carValue["duckLoPunch"][2].asInt());
	caracter->cantSprites.push_back(carValue["duckLoPunch"][1].asInt());
	caracter->anchoSprites.push_back(carValue["duckLoPunch"][0].asInt());

	/*
	for(int j = 0; j < i; j++)
	{
		cout << j+1 << " - " << caracter->cantSprites[j] << endl;
	}
	*/
	caracter->imgPath = carValue["imgSrc"].asString();

	/*Segun lo que vimos en la clase, los valores de color pueden ser float o cosas invalidas.
	En caso de que hay valor invalido ("pepe") tambien dijeron varias veces que una opcion
	es logear que no se puede hacer transformacion y no cambiar color. A esos fines se
	devuelven '-1' en este caso.*/
	bool colorAlternativoValido = true;
	string str = "";
	try	{	caracter->h_inicial = carValue.get("color-alternativo",-1).get("h-inicial",-1).asFloat();	}
	catch(const exception &e) {
		str = e.what();
		colorAlternativoValido = false;
	}
	if (colorAlternativoValido) {
		if (caracter->h_inicial == -1)		colorAlternativoValido = false;
	}

	if (colorAlternativoValido) {
		try	{	caracter->h_final = carValue.get("color-alternativo",-1).get("h-final",-1).asFloat();	}
		catch(const exception &e) {
			str = e.what();
			colorAlternativoValido = false;
		}
	}
	if (colorAlternativoValido) {
		if (caracter->h_final == -1)		colorAlternativoValido = false;
	}
	
	if (colorAlternativoValido) {
		try	{	caracter->desplazamiento = carValue.get("color-alternativo",-1).get("desplazamiento",-1).asFloat();	}
		catch(const exception &e) {
			str = e.what();
			colorAlternativoValido = false;
		}
	}
	if (colorAlternativoValido) {
		if (caracter->desplazamiento == -1)		colorAlternativoValido = false;
	}

	if (colorAlternativoValido) {
	//estandar solo si [0;360), si no pertenece a ese rango le doy vueltas
		while (!(caracter->h_inicial < 360)) {
			caracter->h_inicial -= 360;
		}
		while (caracter->h_inicial < 0) {
			caracter->h_inicial += 360;
		}
		while (!(caracter->h_final < 360)) {
			caracter->h_final -= 360;
		}
		while (caracter->h_final < 0) {
			caracter->h_final += 360;
		}
	} else {
		caracter->h_inicial = -1;
		caracter->h_final = -1;
		caracter->desplazamiento = -1;
		string msg = "Color alternativo de " + caracter->nombre + " es invalido. No sera posible cambiar color en caso de ser necesario.";
		if (str == "") {
			Logger::Instance()->log(ERROR,msg);
		} else {
			Logger::Instance()->log(ERROR,str + " " + msg);
		}
	}
}

void Parser::setearCaracteres() {
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

	Value carValue = root["liukang"];
	this->liukang.nombre = "liukang";
	parsearSpriteCaracter(&(this->liukang), carValue);

	carValue = root["scorpion"];
	this->scorpion.nombre = "scorpion";
	parsearSpriteCaracter(&(this->scorpion), carValue);

}

bool Parser::asignarBoton(int* boton, bool (&teclasUsadas)[8], Value jugValue, int num, string valueString, string botonString) {
	try	{	*boton = jugValue.get(valueString,-1).asInt();	}
		catch(const exception &e) {
			string str(e.what());
			string msg = " Boton de " + botonString + " del jugador " + to_string(static_cast<long double>(num)) + ".";
			Logger::Instance()->log(ERROR,str + msg);
			return false;
		}
		if ((*boton < 0) || (*boton > 7)) {
			string msg = "Boton de " + botonString + " del jugador " + to_string(static_cast<long double>(num)) + " es invalido o no definido.";
			Logger::Instance()->log(WARNING,msg);
			return false;
		} else {
			if (teclasUsadas[*boton] == true) {
				string msg = "Boton de " + botonString + " del jugador " + to_string(static_cast<long double>(num)) + " ya se usa.";
				Logger::Instance()->log(WARNING,msg);
				return false;
			} else {
				teclasUsadas[*boton] = true;
				return true;
			}
		}
}

void Parser::corregirBoton(int* boton, bool (&teclasUsadas)[8], int num, string botonString){
		int k = 0;
		while (teclasUsadas[k]){
			k++;
		}
		*boton = k;
		teclasUsadas[k] = true;
		string msg = "Jugador " + to_string(static_cast<long double>(num)) + ", boton de "
						+ botonString + ": " + to_string(static_cast<long double>(k)) + ".";
		Logger::Instance()->log(WARNING,msg);
}

void Parser::setearDatosJugador(JugadorData* jugador, Value jugValue, Value jugDef, int num){
	//-----ancho-----
	try {	jugador->ancho = jugValue.get("ancho",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			string msg = " Se usara valor por defecto de ancho del jugador " + to_string(static_cast<long double>(num)) + ".";
			Logger::Instance()->log(ERROR,str + msg);
			jugador->ancho = jugDef.get("ancho",-1).asFloat();
		}
		if (jugador->ancho < 2) {
			jugador->ancho = jugDef.get("ancho",-1).asFloat();
			string msg = "Ancho logico del jugador " + to_string(static_cast<long double>(num)) + " invalido o no definido. Se usa valor por defecto.";
			Logger::Instance()->log(WARNING,msg);
		}

	//-----alto-----
	try {	jugador->alto = jugValue.get("alto",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			string msg = " Se usara valor por defecto de alto del jugador " + to_string(static_cast<long double>(num)) + ".";
			Logger::Instance()->log(ERROR,str + msg);
			jugador->alto = jugDef.get("alto",-1).asFloat();
		}
		if (jugador->alto < 2) {
			jugador->alto = jugDef.get("alto",-1).asFloat();
			string msg = "Alto logico del jugador " + to_string(static_cast<long double>(num)) + " invalido o no definido. Se usa valor por defecto.";
			Logger::Instance()->log(WARNING,msg);
		}

	//-----arma_speed-----
	try {	jugador->arma_speed = jugValue.get("arma_speed",-1).asFloat();	}
		catch(const exception &e) {
			string str(e.what());
			string msg = " Se usara valor por defecto para la velocidad del arma del jugador " + to_string(static_cast<long double>(num)) + ".";
			Logger::Instance()->log(ERROR,str + msg);
			jugador->arma_speed = jugDef.get("arma_speed",-1).asFloat();
		}
		if (jugador->arma_speed < 0) {
			jugador->arma_speed = jugDef.get("arma_speed",-1).asFloat();
			string msg = "Velocidad del arma del jugador " + to_string(static_cast<long double>(num)) + " invalido o no definido. Se usa valor por defecto.";
			Logger::Instance()->log(WARNING,msg);
		}

	//-----joyconfig-----
	//Teclas validas: 0, 1, 2, 3, 4, 5, 6, 7
	//Primero asigna todos valores validos. Luego a los que son invalidos les pone teclas desocupadas en orden creciente.
	bool teclasUsadas[8] = { false };

	bool BotonGolpeAlto = asignarBoton(&(jugador->golpe_alto), teclasUsadas, jugValue, num, "golpe-alto", "golpe alto");
	bool BotonGolpeBajo = asignarBoton(&(jugador->golpe_bajo), teclasUsadas, jugValue, num, "golpe-bajo", "golpe bajo");
	bool BotonPatadaAlta = asignarBoton(&(jugador->patada_alta), teclasUsadas, jugValue, num, "patada-alta", "patada alta");
	bool BotonPatadaBaja = asignarBoton(&(jugador->patada_baja), teclasUsadas, jugValue, num, "patada-baja", "patada baja");
	bool BotonDefensa = asignarBoton(&(jugador->defensa), teclasUsadas, jugValue, num, "defensa", "defensa");
	bool BotonArrojar = asignarBoton(&(jugador->arrojar), teclasUsadas, jugValue, num, "arrojar", "arrojar");
	
	if (!BotonGolpeAlto)	corregirBoton(&(jugador->golpe_alto), teclasUsadas, num, "golpe alto");
	if (!BotonGolpeBajo)	corregirBoton(&(jugador->golpe_bajo), teclasUsadas, num, "golpe bajo");
	if (!BotonPatadaAlta)	corregirBoton(&(jugador->patada_alta), teclasUsadas, num, "patada alta");
	if (!BotonPatadaBaja)	corregirBoton(&(jugador->patada_baja), teclasUsadas, num, "patada baja");
	if (!BotonDefensa)	corregirBoton(&(jugador->defensa), teclasUsadas, num, "defensa");
	if (!BotonArrojar)	corregirBoton(&(jugador->arrojar), teclasUsadas, num, "arrojar");

}

Parser::Parser(Value defRoot) {

	setearVentanaPorDefecto(defRoot["ventana"]);
	setearEscenarioPorDefecto(defRoot["escenario"]);
	setearCapasPorDefecto(defRoot["capas"]);
	
	setearJugadorPorDefecto(&(this->jugador1),defRoot["jugador1"]);
	setearJugadorPorDefecto(&(this->jugador2),defRoot["jugador2"]);
	
	setearCaracteres();
}

Parser::Parser(Value root, Value defRoot){
	//FRENTE A AUSENCIA DE CUALQUIER COSA SE REEMPLAZA POR VALOR POR DEFECTO
	Value defVentana = defRoot["ventana"];
	Value defEscenario = defRoot["escenario"];
	Value defJugador1 = defRoot["jugador1"];
	Value defJugador2 = defRoot["jugador2"];
	Value defCapas = defRoot["capas"];

	bool hayVentana = true;
	bool hayEscenario = true;
	bool hayJugador1 = true;
	bool hayJugador2 = true;
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
	
	Value jugador1 = root["jugador1"];
	if (jugador1.empty()) {
		setearJugadorPorDefecto(&(this->jugador1),defJugador1);
		hayJugador1 = false;
		Logger::Instance()->log(ERROR,"Jugador 1 no definido. Se usa jugador 1 por defecto.");
	};

	Value jugador2 = root["jugador2"];
	if (jugador2.empty()) {
		setearJugadorPorDefecto(&(this->jugador2),defJugador2);
		hayJugador2 = false;
		Logger::Instance()->log(ERROR,"Jugador 2 no definido. Se usa jugador 2 por defecto.");
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
			Logger::Instance()->log(ERROR,str + " Se usara valor por defecto de z-index para jugadores.");
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

	//Jugador1
	if (hayJugador1) {
		setearDatosJugador(&(this->jugador1), jugador1, defJugador1, 1);
	}

	//Jugador2
	if (hayJugador2) {
		setearDatosJugador(&(this->jugador2), jugador2, defJugador2, 2);
	}

	setearCaracteres();

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

