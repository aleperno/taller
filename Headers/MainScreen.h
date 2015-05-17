#ifndef HEADERS_MAINSCREEN_H_
#define HEADERS_MAINSCREEN_H_

#include <Ventana.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define FONT_PATH "Images/ardestine.ttf"
#define LIUKANG_FACE_PATH "Images/characters/liukangface.png"
#define SCORPION_FACE_PATH "Images/characters/scorpionface.png"

//Modo seleccionado en screen correspondiente
#define SELECTED_PVP 0
#define SELECTED_PVE 1
#define SELECTED_TRAINING 2

//Personajes
#define LIUKANG 0
#define SCORPION 1

//Foco en texto
#define TEXT_NO_FOCUS 0
#define TEXT_FOCUS_P1 1
#define TEXT_FOCUS_P2 2

//Parametros constantes
#define GATE_SPEED 80
#define TITLE_SPEED 140
#define PRESS_START_SPEED 210
#define MARGIN 10
#define SHAKE_FACTOR 40
#define SHAKE_COUNT 20

class MainScreen {
public:
	MainScreen(Ventana* ventana, vector< vector<int> >* perSelect);
	~MainScreen();

	//Globales
	Ventana* _ventana;
	vector< vector<int> >* _perSelect;
	int descriptionY;
	SDL_Color textColor;
	SDL_Color shadowColor;
	TTF_Font* fontBig;
	TTF_Font* fontSmall;
	TTF_Font* fontMenu;
	vector< vector< pair<int, int> > > posicionesCaras; //x,y por cada una
	TextureHandler* liukangface;
	TextureHandler* scorpionface;
	TextureHandler* nombreP1;
	TextureHandler* nombreP2;
	int topLeftX;
	int topLeftY;
	int faceH;
	int faceW;
	void prepararPerSelect();

	//Intro
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
	void showIntro();
	void actualizarPosiciones();

	//Mode
	TextureHandler* modePVP;
	TextureHandler* modePVE;
	TextureHandler* modeTraining;
	TextureHandler* thisIsMenu;
	void showModeSelect(int modeSelected);

	//PvP
	TextureHandler* thisIsPVP;
	void showPVP();

	//PvE & Training
	TextureHandler* thisIsPVE;
	TextureHandler* thisIsTraining;
	void showPVE();
	void showTraining(int fila, int columna, int textFocus, string nombre);

	int viewName(string nombre);
	void viewNameBoxFocus(int x);
	void viewNameBoxNoFocus(int x);




	int nombreY;
};

#endif /* HEADERS_MAINSCREEN_H_ */