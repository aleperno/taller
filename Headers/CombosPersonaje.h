#ifndef COMBOSPERSONAJE_H_
#define COMBOSPERSONAJE_H_

#include<iostream>
#include<vector>
#include<list>
#include<string>

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
	bool existeCombo(vector<string>* bufferBotones, vector<string>** comboAUX, string* nombreCombo);
};

#endif
