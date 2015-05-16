#ifndef HEADERS_MAINSCREEN_H_
#define HEADERS_MAINSCREEN_H_

#include <Ventana.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define FONT_PATH "Images/ardestine.ttf"

#define SELECTED_PVP 0
#define SELECTED_PVE 1
#define SELECTED_TRAINING 2

//Parametros constantes
#define GATE_SPEED 80
#define TITLE_SPEED 140
#define PRESS_START_SPEED 210
#define MARGIN 10
#define SHAKE_FACTOR 40
#define SHAKE_COUNT 20

class MainScreen {
public:
	MainScreen(Ventana* ventana);
	~MainScreen();
	void actualizarPosiciones();
	void showIntro();
	void showModeSelect(int modeSelected);
	void showPVP();
	void showPVE();
	void showTraining();

	TTF_Font* fontBig;
	TTF_Font* fontSmall;
	TTF_Font* fontMenu;
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

	TextureHandler* modePVP;
	TextureHandler* modePVE;
	TextureHandler* modeTraining;
	TextureHandler* press;

	TextureHandler* thisIsPVP;
	TextureHandler* thisIsPVE;
	TextureHandler* thisIsTraining;
};

#endif /* HEADERS_MAINSCREEN_H_ */