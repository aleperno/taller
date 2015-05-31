#include <CombosPersonaje.h>

CombosPersonaje::CombosPersonaje(vector<string>* combo1, vector<string>* combo2, vector<string>* fatality1, int tolerancia) {
	this->combo1 = combo1;
	this->combo2 = combo2;
	this->fatality1 = fatality1;
	this->tolerancia = tolerancia;
}

CombosPersonaje::~CombosPersonaje() {
	this->combo1 = NULL;
	this->combo2 = NULL;
	this->fatality1 = NULL;
}
void CombosPersonaje::imprimirCombos() {
	for (unsigned int i=0; i<this->combo1->size();++i) {
		//cout << this->combo1->at(i);
	}
	cout << endl;
	for (unsigned int i=0; i<this->combo2->size();++i) {
			//cout << this->combo2->at(i);
		}
	cout << endl;
	for (unsigned int i=0; i<this->fatality1->size();++i) {
			//cout << this->fatality1->at(i);
	}
	cout << endl;
}

bool CombosPersonaje::existeCombo(vector<string>* bufferBotones) {
	return false;
}
