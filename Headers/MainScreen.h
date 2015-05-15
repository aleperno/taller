#ifndef HEADERS_MAINSCREEN_H_
#define HEADERS_MAINSCREEN_H_

#include <Ventana.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define FONT_PATH "Images/ardestine.ttf"

class MainScreen {
public:
	MainScreen(Ventana* ventana);
	~MainScreen();
	void actualizarPosiciones();
	void showIntro();

	TTF_Font* fontBig;
	TTF_Font* fontSmall;
	Ventana* _ventana;
	SDL_Rect gateLeft;
	SDL_Rect gateRight;
	int titleX;
	int titleY;
	int pressStartX;
	int pressStartY;

	int gateSpeed;
	int titleSpeed;
	int pressStartSpeed;

	TextureHandler* title;
	TextureHandler* pressStart;
};

#endif /* HEADERS_MAINSCREEN_H_ */