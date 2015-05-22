#ifndef HEADERS_JUGADORDATA_H_
#define HEADERS_JUGADORDATA_H_

using namespace std;

//Esos datos se levantan desde los jsons de mapa.
class JugadorData {
public:
	float ancho;
	float alto;
	float arma_speed;
	
	//joyconfig
	int golpe_alto;
	int golpe_bajo;
	int patada_alta;
	int patada_baja;
	int defensa;
	int arrojar;

};

#endif /* HEADERS_JUGADORDATA_H_ */