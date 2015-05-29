#ifndef HEADERS_BOTON_H_
#define HEADERS_BOTON_H_

#include <Ventana.h>
#include <SDL.h>
#include <SDL_image.h>

class Boton {
public:
	//boton con textura
	Boton(Ventana* _ventana, TextureHandler* textTexture, TextureHandler* surfaceTexture, int ejeCentralX, int y, int w, int h);
	//boton sin textura
	Boton(Ventana* _ventana, TextureHandler* surfaceTexture, int ejeCentralX, int y, int w, int h);
	~Boton();

	//atributos
	Ventana* _ventana;
	TextureHandler* textTexture;
	TextureHandler* surfaceTexture;
	int x;
	int y;
	int w;
	int h;

	//metodos
	void view();
	bool mouseOver();
};



#endif /* HEADERS_BOTON_H_ */