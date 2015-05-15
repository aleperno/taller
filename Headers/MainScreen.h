#ifndef HEADERS_MAINSCREEN_H_
#define HEADERS_MAINSCREEN_H_

#include <Ventana.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define FONT_PATH "Images/ardestine.ttf"

//Parametros constantes
#define GATE_SPEED 80
#define TITLE_SPEED 140
#define PRESS_START_SPEED 140
#define MARGIN 10
#define SHAKE_FACTOR 40
#define SHAKE_COUNT 20

class MainScreen {
public:
	MainScreen(Ventana* ventana);
	~MainScreen();
	void actualizarPosiciones();
	void showIntro();
	void showModeSelect();

	TTF_Font* fontBig;
	TTF_Font* fontSmall;
	Ventana* _ventana;
	SDL_Rect gateLeft;
	SDL_Rect gateRight;
	int titleX;
	int titleY;
	int pressStartX;
	int pressStartY;

	int shakeCount;
	int randomX;
	int randomY;

	TextureHandler* title;
	TextureHandler* titleShadow;
	TextureHandler* pressStart;
};

#endif /* HEADERS_MAINSCREEN_H_ */