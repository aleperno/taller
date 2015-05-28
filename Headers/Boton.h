#ifndef HEADERS_BOTON_H_
#define HEADERS_BOTON_H_

#include <Ventana.h>
#include <SDL.h>
#include <SDL_image.h>

class Boton {
public:
	//boton con textura
	Boton(Ventana* _ventana, TextureHandler* texture, int ejeCentralX, int y);
	//boton sin textura
	Boton(Ventana* _ventana, int w, int h, int ejeCentralX, int y);
	~Boton();

	//atributos
	Ventana* _ventana;
	TextureHandler* texture;
	int ejeCentralX;
	int x;
	int y;
	int w;
	int h;

	//metodos
	void view();
	void viewExternBox(SDL_Color* c);
	void viewHighlight(SDL_Color* c);
	bool mouseOver();
};



#endif /* HEADERS_BOTON_H_ */