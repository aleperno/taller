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

#define FONT_PATH "Images/ardestine.ttf"

class Hud{
	public:
		Hud(Ventana* ventana, Personaje* _personaje1, Personaje* _personaje2);
		~Hud();
		void prepararHUD();
		void actualizarHealthbars();
		void printHUD();
		void printHUD(int time);
	private:
		TTF_Font* fontNombres;
		TTF_Font* fontTimer;
		Ventana* _ventana;
		Personaje* _personaje1;
		Personaje* _personaje2;
		//SDL_Color colorExterno;
		//SDL_Color colorExternoIlum;
		SDL_Color colorInterno;
		SDL_Color colorVida;
		SDL_Color colorVidaIlum;
		SDL_Color colorTexto;
		struct hudPersonaje {
			//SDL_Rect externo;
			//SDL_Rect externoIlum;
			SDL_Rect interno;
			SDL_Rect healthIlum;
			SDL_Rect health;
			TextureHandler* nombreTexture;
			//SDL_Rect nombre;
		};
		struct hudTime {
			//SDL_Rect externo;
			//SDL_Rect externoIlum;
			//SDL_Rect interno;
			TextureHandler* timeTexture;
		};
		hudTime hudTiempo;
		hudPersonaje hud1;
		hudPersonaje hud2;
};



#endif /* HEADERS_HUD_H_ */
