#ifndef HEADERS_CARACTERDATA_H_
#define HEADERS_CARACTERDATA_H_

#define CANTIDAD_MAXIMA_SPRITES 20

#include <vector>
using namespace std;

//Esos datos se levantan desde spriteparse.
class CaracterData {
public:
	string nombre;

	//sprite data
	int height;
	int width;
	int size;
	vector<int> cantSprites;
	vector<int> anchoSprites;
	vector<int> velSprites;
	string imgPath;

	//color alternativo
	float h_inicial;
	float h_final;
	float desplazamiento;

	//tomas
	vector<string> toma1;
	vector<string> toma2;
	vector<string> fatality;
};

#endif /* HEADERS_CARACTERDATA_H_ */