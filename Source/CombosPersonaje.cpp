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

unsigned int CombosPersonaje::comboMasLargoSize() {
	unsigned int max = 0;
	if (combo1->size() > max)	max = combo1->size();
	if (combo2->size() > max)	max = combo2->size();
	if (fatality1->size() > max)	max = fatality1->size();
	return max;
}

void CombosPersonaje::imprimirCombos() {
	for (unsigned int i=0; i<this->combo1->size();++i) {
		cout << this->combo1->at(i);
	}
	cout << endl;
	for (unsigned int i=0; i<this->combo2->size();++i) {
			cout << this->combo2->at(i);
		}
	cout << endl;
	for (unsigned int i=0; i<this->fatality1->size();++i) {
			cout << this->fatality1->at(i);
	}
	cout << endl;
}

vector<string>* CombosPersonaje::giveMeCombo(string combo) {
	if (combo == ARROJABLE)	return combo1;
	else if (combo == BARRIDA)	return combo2;
	else if (combo == FATALITY)	return fatality1;
	else	return NULL;
}

bool CombosPersonaje::existeCombo(vector<string>* bufferBotones, string* nombreCombo) {
	unsigned int tamanioBuffer = bufferBotones->size();
	if (tamanioBuffer<this->combo1->size() && tamanioBuffer<this->combo2->size() && tamanioBuffer<this->fatality1->size()) return false;

	unsigned int i = 0;
	unsigned int j = 0;
	int contadorTolerancia = 0;
	bool hayMatch = false;
	while (i<bufferBotones->size() && j<combo1->size() && !hayMatch && contadorTolerancia <= this->tolerancia) {
		if (bufferBotones->at(i) == this->combo1->at(j)) {
			++i;
			++j;
		} else {
			if(j>0) ++contadorTolerancia;
			++i;
		}
		if (j==combo1->size()) hayMatch=true;
	}
	if (hayMatch) {
		(*nombreCombo) = ARROJABLE;
		return true;
	}

	i = 0;	j = 0; 	contadorTolerancia = 0; hayMatch = false;
	while (i<bufferBotones->size() && j<this->combo2->size() && !hayMatch && contadorTolerancia <= this->tolerancia) {
		if (bufferBotones->at(i) == this->combo2->at(j)) {
			++i;
			++j;
		} else {
			if(j>0) ++contadorTolerancia;
			++i;
		}
		if (j==combo2->size()) hayMatch=true;
	}
	if (hayMatch) {
		(*nombreCombo) = BARRIDA;
		return true;
	}

	i = 0;	j = 0; 	contadorTolerancia = 0; hayMatch = false;
	while (i<bufferBotones->size() && j<this->fatality1->size() && !hayMatch && contadorTolerancia <= this->tolerancia) {
		if (bufferBotones->at(i) == this->fatality1->at(j)) {
			++i;
			++j;
		} else {
			if(j>0) ++contadorTolerancia;
			++i;
		}
		if (j==this->fatality1->size()) hayMatch=true;
	}
	if (hayMatch) {
		(*nombreCombo) = FATALITY;
		return true;
	}

	return false;
}