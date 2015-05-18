#ifndef HEADERS_BOTON_H_
#define HEADERS_BOTON_H_

#include <Ventana.h>
#include <SDL.h>
#include <SDL_image.h>

class Boton {
public:
	Boton(Ventana* _ventana, TextureHandler* texture, int ejeCentralX, int y);
	~Boton();

	//atributos
	Ventana* _ventana;
	TextureHandler* texture;
	int ejeCentralX;
	int x;
	int y;

	//metodos
	void view();
	void viewExternBox(SDL_Color* c);
	void viewHighlight(SDL_Color* c);
};



#endif /* HEADERS_BOTON_H_ */