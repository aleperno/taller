/*
 * HUD.h
 *
 *  Created on: May 2, 2015
 *      Author: neo
 */

#ifndef HEADERS_HUD_H_
#define HEADERS_HUD_H_

#include <Ventana.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <Personaje.h>
#include <StringUtil.h>

#define FONT_PATH "Images/mortalkombat3.ttf"
#define FINISH_Y_PATH "Images/finishYellow.png"
#define FINISH_R_PATH "Images/finishRed.png"
#define FINISH_CYCLES 50

using namespace std;

class Hud{
	public:
		Hud(Ventana* ventana, string* nombreP1, string* nombreP2);
		~Hud();
		void setearPersonajes(Personaje* personaje1, Personaje* personaje2);
		void recargarNombres();
		void actualizarRounds(int round, int p1wins, int p2wins);
		void prepararHUD();
		void actualizarHealthbars();
		void activateFinishHim();
		void showFinishHim();
		void printHUD();
		void printHUD(int time);
		void printHUD(vector<string>* bufferTeclas,bool hayCombo, vector<string>* combo, string nombreCombo);
		void printHUD(vector<teclaBuffer>* bufferTeclas, vector<string>* tomasNombres);
	private:
		TTF_Font* fontNombres;
		TTF_Font* fontTimer;
		Ventana* _ventana;
		string* _nombreP1;
		string* _nombreP2;
		Personaje* _personaje1;
		Personaje* _personaje2;
		SDL_Color colorExterno;
		SDL_Color colorInterno;
		SDL_Color colorVida;
		SDL_Color colorVidaIlum;
		SDL_Color colorNombres;

		int tomasY;
		int tomasMargen;
		bool _viewFinishHim;
		int finishX;
		int finishY;
		int finishW;
		int finishH;
		int finishCount;

		struct hudPersonaje {
			SDL_Rect externo;
			SDL_Rect interno;
			SDL_Rect healthIlum;
			SDL_Rect health;
			TextureHandler* nombreTexture;
		};
		TextureHandler* timeTexture;
		TextureHandler* roundTexture;
		TextureHandler* p1winsTexture;
		TextureHandler* p2winsTexture;
		TextureHandler* finishHimYellow;
		TextureHandler* finishHimRed;
		TextureHandler* bufferTexture;
		hudPersonaje hud1;
		hudPersonaje hud2;
};

#endif /* HEADERS_HUD_H_ */
