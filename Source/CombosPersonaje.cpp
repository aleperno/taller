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

int CombosPersonaje::comboMasLargoSize() {
	int max = 0;
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

bool CombosPersonaje::existeCombo(vector<string>* bufferBotones,vector<string>** comboAux, string* nombreCombo) {
	unsigned int tamanioBuffer = bufferBotones->size();
	if (tamanioBuffer<this->combo1->size() && tamanioBuffer<this->combo2->size() && tamanioBuffer<this->fatality1->size()) return false;

	unsigned int i = 0;
	unsigned int j = 0;
	int contadorTolerancia = 0;
	bool hayMatch = false;
	(*comboAux) = NULL;
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
		(*comboAux) = this->combo1;
		(*nombreCombo) = "Combo 1";
		return true;
	}

	i = 0;	j = 0; 	contadorTolerancia = 0; hayMatch = false; (*comboAux) = NULL;
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
		(*comboAux) = this->combo2;
		(*nombreCombo) = "Combo 2";
		return true;
	}

	i = 0;	j = 0; 	contadorTolerancia = 0; hayMatch = false; (*comboAux) = NULL;
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
		(*comboAux) = this->fatality1;
		(*nombreCombo) = "Fatality";
		return true;
	}

	return false;

	//recorrer buffer y combo1  e ir comparando cada posicion de los string
	//Si hay un match, guardar el combo1 en comboAux y devolver true
	//Si termina de recorrer el combo1 y no hay match, empezar a recorrer el combo2
	//con el mismo criterio
	//Si no hay match con el combo2, hacer lo mismo con la fatality
	//Si no hay match, guardar "" en comboAux y devolver false

}
