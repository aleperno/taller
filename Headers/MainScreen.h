#ifndef HEADERS_MAINSCREEN_H_
#define HEADERS_MAINSCREEN_H_

#include <Ventana.h>
#include <Boton.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define FONT_PATH "Images/ardestine.ttf"
#define LIUKANG_FACE_PATH "Images/characters/liukangface.png"
#define SCORPION_FACE_PATH "Images/characters/scorpionface.png"

#define NOMBRE_VACIO " "

//Modo seleccionado en screen correspondiente
#define NOT_SELECTED -1
#define SELECTED_PVP 0
#define SELECTED_PVE 1
#define SELECTED_TRAINING 2

//Botones 'back' y 'play'
#define BACK_BOTON "back"
#define PLAY_BOTON "play"

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
	vector< vector< pair<int, int> > > posicionesCaras; //x,y por cada una

	int descriptionY;
	SDL_Color textColor;
	SDL_Color shadowColor;
	SDL_Color selected1Color;
	SDL_Color selected2Color;
	SDL_Color selectedBothColor;
	SDL_Color notSelectedColor;
	TTF_Font* fontBig;
	TTF_Font* fontSmall;
	TTF_Font* fontMenu;

	TextureHandler* liukangface;
	TextureHandler* scorpionface;

	Boton* nombreP1_boton;
	Boton* nombreP2_boton;

	Boton* back_boton;
	Boton* play_boton;

	int topLeftX;
	int topLeftY;
	int faceH;
	int faceW;

	void prepararPerSelect();
	void veiwFaces();
	void viewName(Boton* nombreBoton, string nombre, SDL_Color* color);

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

	//Mode select
	Boton* modePVP_boton;
	Boton* modePVE_boton;
	Boton* modeTraining_boton;
	TextureHandler* thisIsMenu;
	int mouseOverMode();
	void showModeSelect(int modeSelected);

	//PvP
	TextureHandler* thisIsPVP;
	void showPVP(int fila1, int columna1, int fila2, int columna2, int textFocus, string nombre1, string nombre2);

	//PvE & Training
	TextureHandler* thisIsPVE;
	TextureHandler* thisIsTraining;
	void showPVE(int fila, int columna, int textFocus, string nombre);
	void showTraining(int fila, int columna, int textFocus, string nombre);
};

#endif /* HEADERS_MAINSCREEN_H_ */