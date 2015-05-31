#ifndef COMBOSPERSONAJE_H_
#define COMBOSPERSONAJE_H_

#include<iostream>
#include<vector>

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
	bool existeCombo(vector<string>* bufferBotones);
};

#endif
