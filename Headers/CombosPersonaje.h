#ifndef COMBOSPERSONAJE_H_
#define COMBOSPERSONAJE_H_

#include<iostream>
#include<vector>
#include<list>
#include<string>

#define ARROJABLE "arrojable"
#define BARRIDA "barrida"
#define FATALITY "fatality"

using namespace std;

class CombosPersonaje {
	private:
	vector<string>*	combo1;
	vector<string>*	combo2;
	vector<string>*	fatality1;
	int tolerancia;
	public:
	CombosPersonaje(vector<string>* combo1, vector<string>* combo2, vector<string>* fatality1, int tolerancia);
	~CombosPersonaje();
	void imprimirCombos();
	unsigned int comboMasLargoSize();
	vector<string>* giveMeCombo(string combo);
	bool existeCombo(vector<string>* bufferBotones, string* nombreCombo);
};

#endif
